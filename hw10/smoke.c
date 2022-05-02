#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create(agent->mutex);
  agent->match   = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke   = uthread_cond_create(agent->mutex);
  return agent;
}

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

// # of threads waiting for a signal. Used to ensure that the agent
// only signals once all other threads are ready.
int num_active_threads = 0;

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

int resource_pair = 0;

struct Smoker {
  struct Agent *agent;
  int resource;
};

struct Smoker* createSmoker(enum Resource r, void *av) {
  struct Smoker* smoker = malloc(sizeof (struct Smoker));
  smoker->agent = (struct Agent *) av;
  smoker->resource = r;
  return smoker;
}

void waitForResource(void * sv) {
  struct Smoker* s = sv;
  switch(s->resource) {
    case MATCH:
      uthread_cond_wait(s->agent->match);
      break;
    case PAPER:
      uthread_cond_wait(s->agent->paper);
      break;
    case TOBACCO:
      uthread_cond_wait(s->agent->tobacco);
      break;
  }
}

void* smoker(void *sv) {
  struct Smoker* s = sv;
  num_active_threads++;
  
  // Wait for agent first to begin cycle below
  uthread_mutex_lock(s->agent->mutex);
  waitForResource(s);
  uthread_mutex_unlock(s->agent->mutex);

  int i=0;
  while (1) {
    uthread_mutex_lock(s->agent->mutex);

    resource_pair += s->resource;
    switch(resource_pair) {
      case MATCH|PAPER:
        uthread_cond_signal(s->agent->tobacco);
        break;
      case MATCH|TOBACCO:
        uthread_cond_signal(s->agent->paper);
        break;
      case PAPER|TOBACCO:
        uthread_cond_signal(s->agent->match);
        break;
    }

    // Only runs if last smoker is signalled by 2nd smoker who got resource.
    if(resource_pair == (MATCH|PAPER|TOBACCO)) {
      smoke_count[s->resource]++;
      resource_pair = 0;
      uthread_cond_signal(s->agent->smoke);
    }
    
    if(i >= NUM_ITERATIONS) {
      uthread_mutex_unlock(s->agent->mutex);
      break;
    }

    // Wait for the agent before last smoker signals smoke.
    waitForResource(s);
    uthread_mutex_unlock(s->agent->mutex);
  }
}

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can modify it if you like, but be sure that all it does
 * is choose 2 random resources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  srandom(time(NULL));
  
  uthread_mutex_lock (a->mutex);
  // Wait until all other threads are waiting for a signal
  while (num_active_threads < 3)
    uthread_cond_wait (a->smoke);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 6;
    switch(r) {
    case 0:
      signal_count[TOBACCO]++;
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    case 1:
      signal_count[PAPER]++;
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 2:
      signal_count[MATCH]++;
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      break;
    case 3:
      signal_count[TOBACCO]++;
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 4:
      signal_count[PAPER]++;
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
      break;
    case 5:
      signal_count[MATCH]++;
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
      break;
    }
    VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
    uthread_cond_wait (a->smoke);
  }
  
  VERBOSE_PRINT("agent done");
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  
  struct Agent* a = createAgent();
  uthread_t agent_thread;

  uthread_init(5);
  
  struct Smoker* m = createSmoker(MATCH, a);
  struct Smoker* p = createSmoker(PAPER, a);
  struct Smoker* t = createSmoker(TOBACCO, a);
  uthread_t match_thread;
  uthread_t paper_thread;
  uthread_t tobacco_thread;

  match_thread = uthread_create(smoker, m);
  paper_thread = uthread_create(smoker, p);
  tobacco_thread = uthread_create(smoker, t);

  agent_thread = uthread_create(agent, a);
  uthread_join(agent_thread, NULL);

  uthread_detach(match_thread);
  uthread_detach(paper_thread);
  uthread_detach(tobacco_thread);

  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);

  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);

  return 0;
}

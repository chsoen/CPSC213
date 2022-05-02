#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define MAX_THINKING_TIME 25000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

typedef struct fork {
  uthread_mutex_t lock;
  uthread_cond_t forfree;
  long free;
} fork_t;

int num_phils, num_meals;    
fork_t *forks;

void deep_thoughts() {
  usleep(random() % MAX_THINKING_TIME);
}

void initfork(int i) {
  forks[i].lock    = uthread_mutex_create();
  forks[i].forfree = uthread_cond_create(forks[i].lock);
  forks[i].free    = 1;
}

long getfork(long i) {
  if(!forks[i].free) {
    uthread_mutex_lock(forks[i].lock);
    uthread_cond_wait(forks[i].forfree);
    uthread_mutex_unlock(forks[i].lock);
  }
  forks[i].free = 0;
  return 1;
}

void putfork(long i) {
  uthread_mutex_lock(forks[i].lock);
  uthread_cond_signal(forks[i].forfree);
  forks[i].free = 1;
  uthread_mutex_unlock(forks[i].lock);
}

int leftfork(long i) {
  return i;
}

int rightfork(long i) {
  return (i + 1) % num_phils;
}

void *phil_thread(void *arg) {
  uintptr_t id = (uintptr_t) arg;
  int meals = 0;
  
  while (meals < num_meals) {
    int left = leftfork(id);
    int right = rightfork(id);

    if(left < right) {
      usleep(random() % MAX_THINKING_TIME);
      getfork(left);
      VERBOSE_PRINT("\tPhil %ld: Got left fork\n", id);
      
      usleep(random() % MAX_THINKING_TIME);
      getfork(right);
      VERBOSE_PRINT("Phil %ld: Got right fork\n", id);
    } else {
      usleep(random() % MAX_THINKING_TIME);
      getfork(right);
      VERBOSE_PRINT("Phil %ld: Got right fork\n", id);

      usleep(random() % MAX_THINKING_TIME);
      getfork(left);
      VERBOSE_PRINT("\tPhil %ld: Got left fork\n", id);
    }

    usleep(random() % MAX_THINKING_TIME);
    meals++;
    VERBOSE_PRINT("\t\tPhil %ld: Eating\n", id);

    usleep(random() % MAX_THINKING_TIME);
    putfork(right);
    VERBOSE_PRINT("\t\t\tPhil %ld: Put right fork\n", id);

    usleep(random() % MAX_THINKING_TIME);
    putfork(left);
    VERBOSE_PRINT("\t\t\t\tPhil %ld: Put left fork\n", id);
  }
  
  return 0;
}

int main(int argc, char **argv) {

  uthread_t *p;
  uintptr_t i;
  
  if (argc != 3) {
    fprintf(stderr, "Usage: %s num_philosophers num_meals\n", argv[0]);
    return 1;
  }

  num_phils = strtol(argv[1], 0, 0);
  num_meals = strtol(argv[2], 0, 0);
  
  forks = malloc(num_phils * sizeof(fork_t));
  p = malloc(num_phils * sizeof(uthread_t));

  uthread_init(num_phils);
  
  srandom(time(0));
  for (i = 0; i < num_phils; ++i) {
    initfork(i);
  }

  for (i = 0; i < num_phils; i++) {
    p[i] = uthread_create(phil_thread, (void *) i);
  }

  for (i = 0; i < num_phils; i++) {
    uthread_join(p[i], NULL);
  }
  
  return 0;
}

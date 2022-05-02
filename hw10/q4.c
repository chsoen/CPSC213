#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

uthread_t t0, t1, t2;
uthread_mutex_t mx;
uthread_cond_t cond0, cond1, cond2;
int done[3] = {0, 0, 0};

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void* p0(void* v) {
  randomStall();
  uthread_mutex_lock(mx);
    done[0] = 1;
    uthread_cond_signal(cond0);
  uthread_mutex_unlock(mx);
  printf("zero\n");
  return NULL;
}

void* p1(void* v) {
  randomStall();
  uthread_mutex_lock(mx);
    if(!done[0]) {
      uthread_cond_wait(cond0);
    }
    done[1] = 1;
    uthread_cond_signal(cond1);
  uthread_mutex_unlock(mx);
  printf("one\n");
  return NULL;
}

void* p2(void* v) {
  randomStall();
  uthread_mutex_lock(mx);
    if(!done[1]) {
      uthread_cond_wait(cond1);
    }
    done[2] = 1;
    uthread_cond_signal(cond2);
  uthread_mutex_unlock(mx);
  printf("two\n");
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  mx = uthread_mutex_create();
  cond0 = uthread_cond_create(mx);
  cond1 = uthread_cond_create(mx);
  cond2 = uthread_cond_create(mx);
  t0 = uthread_create(p0, NULL);
  t1 = uthread_create(p1, NULL);
  t2 = uthread_create(p2, NULL);
  randomStall();
  uthread_mutex_lock(mx);
    if(!done[2]) {
      uthread_cond_wait(cond2);
    }
  uthread_mutex_unlock(mx);
  uthread_join(t0, NULL);
  uthread_join(t1, NULL);
  uthread_join(t2, NULL);
  
  printf("three\n");
  printf("------\n");
  
  uthread_cond_destroy(cond0);
  uthread_cond_destroy(cond1);
  uthread_cond_destroy(cond2);
  uthread_mutex_destroy(mx);
}

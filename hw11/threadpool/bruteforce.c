#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <crypt.h>

#include "uthread.h"
#include "threadpool.h"

#define KEY_LENGTH 8

char *hash_string;

void my_task(tpool_t pool, void *arg) {
  char *key = (char *) arg;
  struct crypt_data data;
  char* result = crypt_r(key, hash_string, &data);
  if(strcmp(result, hash_string) == 0) {
    printf("%s\n", key);
    free(key);
    exit(1);
  } else {
    for (int i = 0; i < 10; i++) {
      char *str = malloc(2*KEY_LENGTH * sizeof(char));
      if(sprintf(str, "%s%d", key, i) > KEY_LENGTH) {
        return;
      }
      tpool_schedule_task(pool, my_task, (void *) str);
    }
  }
  free(key);
}

int main(int argc, char *argv[]) {

  tpool_t pool;
  int num_threads;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s NUM_THREADS HASH_STRING\n", argv[0]);
    return -1;
  }
  
  num_threads = strtol(argv[1], NULL, 10);
  hash_string = argv[2];
  
  uthread_init(8);
  pool = tpool_create(num_threads);

  for (int i = 0; i < 10; i++) {
    char *str = malloc(2*KEY_LENGTH * sizeof(char));
    sprintf(str, "%d", i);
    tpool_schedule_task(pool, my_task, (void *) str);
  }

  tpool_join(pool);
  
  return 0;
}

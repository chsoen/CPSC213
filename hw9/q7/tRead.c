#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t      pending_read_queue;
unsigned int sum = 0;

void interrupt_service_routine () {
  uthread_t read_thread;
  queue_dequeue(pending_read_queue, (void **) &read_thread, NULL, NULL);
  uthread_unblock(read_thread);
}

void* read_block (void* blocknov) {
  unsigned int *result = malloc(sizeof(int));
  disk_schedule_read(result, *(int *) blocknov);
  free(blocknov);
  uthread_block();
  return result;
}

int main (int argc, char** argv) {

  // Command Line Arguments
  static char* usage = "usage: tRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();
  queue_t queue = queue_create();

  // Sum Blocks
  for(int blockno=0; blockno < num_blocks; blockno++) {
    int *temp = malloc(sizeof(int));
    *temp = blockno;
    uthread_t read_thread = uthread_create(&read_block, temp);
    queue_enqueue(pending_read_queue, (void *) read_thread, NULL, NULL);
    queue_enqueue(queue, (void *) read_thread, NULL, NULL);
  }

  for(int blockno=0; blockno < num_blocks; blockno++) {
    uthread_t read_thread;
    queue_dequeue(queue, (void **) &read_thread, NULL, NULL);
    unsigned int *result;
    uthread_join(read_thread, (void **) &result);
    sum += *result;
    free(result);
  }

  printf("%d\n", sum);
}


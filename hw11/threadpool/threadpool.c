#include <stdlib.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"
#include "uthread_sem.h"

#include "threadpool.h"

typedef struct worker* worker_t;
typedef struct task* task_t;

struct worker {
  uthread_t thread;
  worker_t next;
};

struct task {
  void (*fun)(tpool_t, void *);
  void *arg;
  task_t next;
};

struct queue {
  void *head;
  void *tail;
};

struct tpool {
  uthread_mutex_t mutex;
  uthread_sem_t hasTask;
  uthread_sem_t join;
  int joining;
  struct queue tasks;
};

/* Function executed by each pool worker thread. This function is
 * responsible for running individual tasks. The function continues
 * running as long as either the pool is not yet joined, or there are
 * unstarted tasks to run. If there are no tasks to run, and the pool
 * has not yet been joined, the worker thread must be blocked.
 * 
 * Parameter: param: The pool associated to the thread.
 * Returns: nothing.
 */
static void *worker_thread(void *param) {
  tpool_t pool = param;

  while(!pool->joining || pool->tasks.head != 0) {
    uthread_sem_wait(pool->hasTask);
    uthread_mutex_lock(pool->mutex);
    task_t task = (task_t) pool->tasks.head;
    pool->tasks.head = task->next;
    if(task->next == 0) {
      pool->tasks.tail = 0;
    }
    uthread_mutex_unlock(pool->mutex);

    task->fun(pool, task->arg);
  }

  uthread_detach(uthread_self());
  uthread_sem_signal(pool->join);
  return NULL;
}

/* Creates (allocates) and initializes a new thread pool. Also creates
 * `num_threads` worker threads associated to the pool, so that
 * `num_threads` tasks can run in parallel at any given time.
 *
 * Parameter: num_threads: Number of worker threads to be created.
 * Returns: a pointer to the new thread pool object.
 */
tpool_t tpool_create(unsigned int num_threads) {
  tpool_t pool = malloc(sizeof(struct tpool));
  pool->mutex = uthread_mutex_create();
  pool->hasTask = uthread_sem_create(0);
  pool->join = uthread_sem_create(-num_threads + 1);

  pool->joining = 0;

  for(int i=0; i < num_threads; i++) {
    uthread_create(worker_thread, pool);
  }

  pool->tasks.head = 0;
  pool->tasks.tail = 0;
  return pool;
}

/* Queues a new task, to be executed by one of the worker threads
 * associated to the pool. The task is represented by function `fun`,
 * which receives the pool and a generic pointer as parameters. If any
 * of the worker threads is available, `fun` is started immediately by
 * one of the worker threads. If all of the worker threads are busy,
 * `fun` is scheduled to be executed when a worker thread becomes
 * available. Tasks are retrieved by individual worker threads in the
 * order in which they are scheduled, though due to the nature of
 * concurrency they may not start exactly in the same order. This
 * function returns immediately, and does not wait for `fun` to
 * complete.
 *
 * Parameters: pool: the pool that is expected to run the task.
 *             fun: the function that should be executed.
 *             arg: the argument to be passed to fun.
 */
void tpool_schedule_task(tpool_t pool, void (*fun)(tpool_t, void *),
                         void *arg) {
  task_t task = malloc(sizeof(struct task));
  task->fun = fun;
  task->arg = arg;
  task->next = 0;

  uthread_mutex_lock(pool->mutex);
  if(pool->tasks.tail)
    ((task_t) pool->tasks.tail)->next = task;
  pool->tasks.tail = task;
  if(pool->tasks.head == 0)
    pool->tasks.head = pool->tasks.tail;
  uthread_mutex_unlock(pool->mutex);
  uthread_sem_signal(pool->hasTask);
}

/* Blocks until the thread pool has no more scheduled tasks; then,
 * joins all worker threads, and frees the pool and all related
 * resources. Once this function returns, the pool cannot be used
 * anymore.
 *
 * Parameters: pool: the pool to be joined.
 */
void tpool_join(tpool_t pool) {
  pool->joining = 1;
  uthread_sem_wait(pool->join);
  uthread_mutex_destroy(pool->mutex);
  uthread_sem_destroy(pool->hasTask);
  uthread_sem_destroy(pool->join);
}

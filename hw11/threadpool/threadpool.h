#pragma once

typedef struct tpool *tpool_t;

tpool_t tpool_create(unsigned int num_threads);
void tpool_schedule_task(tpool_t pool, void (*fun)(tpool_t, void *), void *arg);
void tpool_join(tpool_t pool);

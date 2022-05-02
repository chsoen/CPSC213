#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "uthread.h"
#include "uthread_sem.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

#define MAX_OCCUPANCY  3
#define NUM_ITERATIONS 100
#define NUM_CARS       20

// These times determine the number of times yield is called when in
// the street, or when waiting before crossing again.
#define CROSSING_TIME             NUM_CARS
#define WAIT_TIME_BETWEEN_CROSSES NUM_CARS

/**
 * You might find these declarations useful.
 */
enum Direction {EAST = 0, WEST = 1};
const static enum Direction oppositeEnd [] = {WEST, EAST};

struct Street {
  uthread_sem_t mutex;
  uthread_sem_t mutex2;
  uthread_sem_t west;
  uthread_sem_t east;
  int direction;
  int crossing;
  int carCrossed;
  int carsOnLeft;
  int carsOnRight;
} Street;

void initializeStreet(void) {
  Street.mutex = uthread_sem_create(1);
  Street.west = uthread_sem_create(0);
  Street.east = uthread_sem_create(0);
  Street.direction = -1;
  Street.crossing = 0;
  Street.carCrossed = 0;
  Street.carsOnLeft = 0;
  Street.carsOnRight = 0;
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_sem_t   waitingHistogramLock;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];

void enterStreet (enum Direction g) {
  while (1) {
    if(Street.crossing == 0 || (Street.direction == g && !Street.carCrossed && Street.crossing < MAX_OCCUPANCY)) {
      Street.direction = g;
      Street.crossing++;
      assert(Street.direction == g);
      assert(Street.crossing <= MAX_OCCUPANCY);
      return;
    }
    if(g == WEST) {
      Street.carsOnRight++;
      uthread_sem_signal(Street.mutex);
      uthread_sem_wait(Street.west);
      uthread_sem_wait(Street.mutex);
      Street.carsOnRight--;
    } else {
      Street.carsOnLeft++;
      uthread_sem_signal(Street.mutex);
      uthread_sem_wait(Street.east);
      uthread_sem_wait(Street.mutex);
      Street.carsOnLeft--;
    }
  }
}

void leaveStreet(void) {
  if(!Street.carCrossed) {
    Street.carCrossed = 1;
    occupancyHistogram[Street.direction][Street.crossing]++;
  }
  Street.crossing--;
  if(Street.crossing <= 0) {
    Street.carCrossed = 0;
    if(Street.carsOnRight + Street.carsOnLeft == 0) {
      return;
    }
    if(Street.carsOnRight <= 0 || (Street.direction == WEST && Street.carsOnLeft >0)) {
      Street.direction = EAST;
      for(int i=0; i < MAX_OCCUPANCY; i++) {
        uthread_sem_signal(Street.east);
      }
    }
    if(Street.carsOnLeft <= 0 || (Street.direction == EAST && Street.carsOnRight >0)) {
      Street.direction = WEST;
      for(int i=0; i < MAX_OCCUPANCY; i++) {
        uthread_sem_signal(Street.west);
      }
    }
  }
}

void recordWaitingTime (int waitingTime) {
  uthread_sem_wait (waitingHistogramLock);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_sem_signal (waitingHistogramLock);
}

void *car(void * direction) {
  for(int i=0; i < NUM_ITERATIONS; i++) {
    uthread_sem_wait(Street.mutex);
    int initial = ++entryTicker;
    enterStreet((enum Direction) direction);
    int final = entryTicker;
    recordWaitingTime(final - initial);
    uthread_sem_signal(Street.mutex);

    for(int j=0; j < NUM_CARS; j++) {
      uthread_yield();
    }

    uthread_sem_wait(Street.mutex);
    leaveStreet();
    uthread_sem_signal(Street.mutex);
    for(int j=0; j < NUM_CARS; j++) {
      uthread_yield();
    }
  }
}

int main (int argc, char** argv) {
  
  uthread_init(8);

  waitingHistogramLock = uthread_sem_create(1);

  initializeStreet();
  uthread_t pt [NUM_CARS];

  for(int i=0; i < NUM_CARS; i++) {
    pt[i] = uthread_create(car, (void *) (random() % 2));
  }
  for(int i=0; i < NUM_CARS; i++) {
    uthread_join(pt[i], NULL);
  }

  int sum = 0;
  for(int i=0; i <  WAITING_HISTOGRAM_SIZE; i++) {
    sum += waitingHistogram[i];
  }
  assert(NUM_ITERATIONS*NUM_CARS == sum);

  assert(NUM_ITERATIONS*NUM_CARS == occupancyHistogram[EAST][1] +
  occupancyHistogram[EAST][2]*2 +
  occupancyHistogram[EAST][3]*3 +
  occupancyHistogram[WEST][1] +
  occupancyHistogram[WEST][2]*2 +
  occupancyHistogram[WEST][3]*3);
  
  printf ("Times with 1 car  going east: %d\n", occupancyHistogram [EAST] [1]);
  printf ("Times with 2 cars going east: %d\n", occupancyHistogram [EAST] [2]);
  printf ("Times with 3 cars going east: %d\n", occupancyHistogram [EAST] [3]);
  printf ("Times with 1 car  going west: %d\n", occupancyHistogram [WEST] [1]);
  printf ("Times with 2 cars going west: %d\n", occupancyHistogram [WEST] [2]);
  printf ("Times with 3 cars going west: %d\n", occupancyHistogram [WEST] [3]);
  
  printf ("Waiting Histogram\n");
  for (int i=0; i < WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Cars waited for           %4d car%s to enter: %4d time(s)\n",
	      i, i==1 ? " " : "s", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Cars waited for more than %4d cars to enter: %4d time(s)\n",
	    WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}

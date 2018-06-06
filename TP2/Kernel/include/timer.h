#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <scheduler.h>

#define MAX_SLEEPING_PROCESSES 64
#define NOT_USED -1
#define SLEEPING_PROCESS_LIMIT_REACHED -1
#define SLEEPING_PROCESS_NOT_FOUND -1

typedef struct {
  int pid;
  int thread;
  uint64_t tickQuantity;
  int next;
} sleepingThread_t;

void initializeTimer();

void sleep(uint64_t milliseconds, int pid, int thread);

void decrementTicks();

int removeSleepingProcess(int pid, int thread);

int addSleepingProcess(int pid, int thread, uint64_t tickQuantity);

int getNextAvailableSpace();

#endif

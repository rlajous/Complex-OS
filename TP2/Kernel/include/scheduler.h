#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <process.h>
#include <buddyMemoryAllocator.h>

#define MAX_PROCESSES 100
#define PROCESS_LIMIT_REACHED -1
#define FIRST_PROCESS -2
#define QUANTUM 2

typedef struct {
  int next;
  process_t * process;
} roundRobinNode_t;

void * schedule(void * rsp);

int getPid();

void addProcess(process_t* process);

void removeProcess(process_t* process);

int findFirstAvailableSpace();

#endif
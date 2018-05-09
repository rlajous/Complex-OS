#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <process.h>
#include <buddyMemoryAllocator.h>

#define MAX_PROCESSES 100
#define PROCESS_LIMIT_REACHED -1
#define PID_NOT_FOUND -1
#define QUANTUM 2

typedef struct {
  int next;
  process_t * process;
} roundRobinNode_t;

void initializeScheduler();

void * nextProcess();

void * schedule();

int getpid();

void * getCurrentStack();

void addProcess(process_t* process);

void removeProcess(process_t* process);

int findFirstAvailableSpace();

void * switchToKernelStack(void * rsp);

void * getEntryPoint();

void killCurrent();

void killProcess(int pid);

int getProcessIndex(int pid);

void blockProcess(int pid);

void unblockProcess(int pid);

void getProcesses(char * buffer, int size);

#endif

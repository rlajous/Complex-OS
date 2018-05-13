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

int addProcess(process_t* process);

void removeProcess(process_t* process);

int findFirstAvailableSpace();

void * switchToKernelStack(void * rsp);

void * getEntryPoint();

void killForeground();

void killCurrent();

void killProcess(int pid);

int getProcessIndex(int pid);

void changeProcessState(int pid, processState state);

void getProcesses(char * buffer, int size);

void setForeground(int pid);

int getForeground();

int isForeground(int pid);

void lock();

void unlock();

void blockRead();

void unblockRead();

#endif

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <process.h>
#include <buddyMemoryAllocator.h>
#include <threads.h>

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

int getCurrentThread();

void * getCurrentStack();

int addProcess(process_t* process);

void removeProcess(process_t* process);

int findFirstAvailableSpace();

void * switchToKernelStack(void * rsp);

void * getEntryPoint();

void killForeground();

void killCurrent();

void killThread(int pid, int thread);

void killProcess(int pid);

void changeChildrenToOrphan(int pid);

int getProcessIndex(int pid);

process_t * getProcess(int pid);

void getProcesses(char * buffer, int size);

void setForeground(int pid);

int getForeground();

int isForeground(int pid);

void lock();

void unlock();

void blockRead();

void unblockRead();

#endif

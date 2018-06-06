#ifndef THREADS_H
#define THREADS_H

#include <types.h>
#include <process.h>

#define THREAD_INITIAL_PAGES 2
#define NOT_WAITING -1

thread_t * createThread(int argc, char * argv[], void * entryPoint);

void terminateThread(process_t * process, int thread);

void deleteThread(thread_t * thread);

void killThreads(process_t * process);

int nextThread(process_t * process);

void unlockWaitingThreads(process_t * process, int thread);

void joinThread(process_t * process, int thread, int toJoin);

void changeThreadState(int pid, int thread, processState state);

#endif

#ifndef MUTEX_H
#define MUTEX_H

#include <scheduler.h>

#define MAX_BLOCKED 30
#define MAX_MUTEXES 128
#define MAX_NAME 50

#define NOT_USED -1
#define MUTEX_CREATION_ERROR -2
#define FULL_LIST -3
#define LOCKED 0
#define UNLOCKED 1

typedef struct {
    threadId_t creator;
    threadId_t lockProcess;
    char name[MAX_NAME];
    threadId_t blocked[MAX_BLOCKED];
    int blockedQuantity;
    int value;
} mutex_t;

void setupMutexSystem();

int getMutex(char * name, int pid, int thread);

int createMutex(int pid, char * name, int thread);

int releaseMutex(int pid, int mutex, int thread);

int getNextMutexAvailable();

void clearMutex(int mutex);

int nextListIndexAvailable(threadId_t * list, int length);

int addToBlocked(int mutex, int pid, int thread);

int isValidMutex(int mutex);

void mutexDown(int mutex, int pid, int thread);

void mutexUp(int mutex, int pid, int thread);

int unlockProcess(int mutex);

void removePidFromMutexes(int pid, int thread);

void removeThreadFromMutexes(int pid, int thread);

#endif

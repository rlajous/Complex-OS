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
#define BLOCKED 0

typedef struct {
    int pid;
    int lockPid;
    char name[MAX_NAME];
    int blocked[MAX_BLOCKED];
    int blockedQuantity;
    int value;
} mutex_t;

void setupMutexSystem();

int getMutex(char * name, int pid);

int createMutex(int pid, char * name);

int releaseMutex(int pid, int mutex);

int getNextMutexAvailable();

void clearMutex(int mutex);

int nextListIndexAvailable(int * list, int length);

int addToBlocked(int mutex, int pid);

int isValidMutex(int mutex);

void mutexDown(int mutex, int pid);

void mutexUp(int mutex, int pid);

int unlockProcess(int mutex);

#endif

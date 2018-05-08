#ifndef MUTEX_H
#define MUTEX_H

#include <scheduler.h>

#define MAX_BLOCKED 50
#define MAX_MUTEXES 50
#define MAX_NAME 50

#define NOT_USED -1
#define MUTEX_CREATION_ERROR -2
#define FULL_LIST -3
#define BLOCKED 1

typedef struct {
    int pid;
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

int isValid(int mutex);

void mutexDown(int mutex, int pid);

void mutexUp(int mutex);

int unlockProcess(int mutex);

#endif

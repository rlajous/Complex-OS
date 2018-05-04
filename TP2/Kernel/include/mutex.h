#ifndef MUTEX_H
#define MUTEX_H

#include <scheduler.h>

#define MAX_BLOCKED 10
#define MAX_PROCESSES 10
#define MAX_MUTEXES 50
#define MAX_NAME 50


#define MUTEX_NOT_USED -1
#define MUTEX_CREATION_ERROR -1
#define FULL_LIST -2
#define PID_NOT_USED -1

typedef struct {
    int creatorPid;
    char name[MAX_NAME];
    int blocked[MAX_BLOCKED];
    int processes[MAX_PROCESSES];
    int value;
} mutex_t;

void setupMutexSystem();

int getMutex(char * name);

int createMutex(int pid, char * name);

int getNextMutexAvailable();

void clearMutex(int mutex);

#endif MUTEX_H

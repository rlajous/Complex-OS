#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex.h>

#define MAX_SEMAPHORES 64

#define SEM_CREATION_ERROR -2

typedef struct {
    int pid;
    char name[MAX_NAME];
    int blocked[MAX_BLOCKED];
    int blockedQuantity;
    int value;
    int mutex;
} semaphore_t;

void setupSemaphoreSystem();

int getSemaphore(char * name, int pid);

int createSemaphore(int pid, char * name);

int releaseSemaphore(int pid, int semaphore);

int getNextSemaphoreAvailable();

void clearSemaphore(int semaphore);

int addToSemaphoreBlocked(int semaphore, int pid);

int isValidSemaphore(int semaphore);

void wait(int semaphore, int pid);

void signal(int semaphore);

int unlockSemaphoreProcess(int semaphore);

#endif

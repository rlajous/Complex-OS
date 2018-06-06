#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <mutex.h>

#define MAX_SEMAPHORES 64

#define SEM_CREATION_ERROR -2

typedef struct {
    threadId_t creator;
    char name[MAX_NAME];
    threadId_t blocked[MAX_BLOCKED];
    int blockedQuantity;
    int value;
    int mutex;
} semaphore_t;

void setupSemaphoreSystem();

int getSemaphore(char * name, int pid, int thread);

int createSemaphore(int pid, char * name, int thread);

int releaseSemaphore(int pid, int semaphore, int thread);

int getNextSemaphoreAvailable();

void clearSemaphore(int semaphore);

int addToSemaphoreBlocked(int semaphore, int pid, int thread);

int isValidSemaphore(int semaphore);

void wait(int semaphore, int pid, int thread);

void signal(int semaphore);

void setValue(int semaphore, int value);

int unlockSemaphoreProcess(int semaphore);

void removePidFromSemaphores(int pid, int thread);

void removeThreadFromSemaphores(int pid, int thread);

#endif

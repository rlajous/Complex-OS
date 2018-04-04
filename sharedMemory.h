#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHMSIZE 100000

int * generateSharedMemory(int key, int* memoryId);
int generateSemaphore(int key);
void initializeSemaphore(int semaphoreId);
void changeSemaphore(int semaphoreId, int change);
int verifySharedMemoryIndexBounds(int index);
void detachMemory(int * sharedMemory);
void destroyMemory(int memoryId, int * sharedMemory);

//Semun definition from http://man7.org/linux/man-pages/man2/semctl.2.html

union semun {
  int val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
 (Linux-specific) */
};

#endif

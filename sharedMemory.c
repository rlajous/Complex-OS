#include "sharedMemory.h"

int * generateSharedMemory(int key, int* memoryId) {

	int * memory;

	//Read & Write Memory
	*memoryId = shmget(key, sizeof(int) * SHMSIZE, 0606 | IPC_CREAT);

	if (*memoryId == -1) {
		perror("Shared Memory creation error");
		exit(1);
	}

	memory = (int *) shmat(*memoryId, NULL, 0);

	if (memory == NULL) {
		perror("Shared Memory creation error");
		exit(1);
	}
	return memory;
}

int generateSemaphore(int key) {

	int semaphoreId;

	semaphoreId = semget (key, 1, 0606 | IPC_CREAT);

	if (semaphoreId == -1) {
		perror("Semaphore creation error");
		exit(1);
	}
	return semaphoreId;
}

void initializeSemaphore(int semaphoreId) {
	union semun sem;

	sem.val = 1;
	semctl(semaphoreId, 0, SETVAL, sem);
}

void changeSemaphore(int semaphoreId, int change) {

	struct sembuf modification;
	modification.sem_num = 0;
	modification.sem_op = change;
	modification.sem_flg = 0;
	semop(semaphoreId, &modification, 1);
}

int verifySharedMemoryIndexBounds(int index) {
	return index < SHMSIZE;
}

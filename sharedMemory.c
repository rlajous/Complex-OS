#include "sharedMemory.h"

char * generateSharedMemory(int key, int* memoryId) {

	char * memory;

	//Read & Write Memory
	*memoryId = shmget(key, sizeof(char) * SHMSIZE, 0606 | IPC_CREAT);

	if (*memoryId == -1) {
		perror("Shared Memory creation error");
		exit(1);
	}

	memory = (char *) shmat(*memoryId, NULL, 0);

	if (memory == NULL) {
		perror("Shared Memory creation error");
		exit(1);
	}
	return memory;
}

int generateSemaphore(int key) {

	int semaphoreId;

	semaphoreId = semget (key, 2, 0606 | IPC_CREAT);

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
	semctl(semaphoreId, 1, SETVAL, sem);
}

void changeSemaphore(int semaphoreNum, int semaphoreId, int change) {

	struct sembuf modification;
	modification.sem_num = semaphoreNum;
	modification.sem_op = change;
	modification.sem_flg = 0;
	semop(semaphoreId, &modification, 1);
}

int getSemaphoreValue(int semaphoreNum, int semaphoreId) {
	return semctl(semaphoreId, semaphoreNum, GETVAL, NULL);
}

int verifySharedMemoryIndexBounds(int index) {
	return index < SHMSIZE;
}

void detachMemory(char * sharedMemory) {
	shmdt(sharedMemory);
}

void destroyMemory(int memoryId, char * sharedMemory) {
	detachMemory(sharedMemory);
	shmctl(memoryId, IPC_RMID, NULL);
}

void destroySemaphore(int semaphoreId) {
	semctl(semaphoreId, 0, IPC_RMID, NULL);
	semctl(semaphoreId, 1, IPC_RMID, NULL);
}

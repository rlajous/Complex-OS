#include "application.h"

int main(int argc, char ** argv) {
	slave * slaves;
	char ** filenames;
	int numOfFiles = 0;
	int distributedFiles = 0;
	int finishedFiles = 0;
	fd_set readFds;
	int maxFd;

	FILE * outputFile;

	char * sharedMemory;
	int sharedMemoryIndex = 1;
	int semaphoreId;
	int sharedMemoryId;
	key_t key = getpid();
	FD_ZERO(&readFds);

	if (argc < 2){
		printf("No files were specified to be hashed.\n");
		exit(1);
	}

	outputFile = fopen("md5Hash.txt", "w");
	if (outputFile == NULL) {
		perror("File creation error.");
		exit(1);
	}

	sharedMemory = generateSharedMemory(key, &sharedMemoryId);

	sharedMemory[0] = 0;

	semaphoreId = generateSemaphore(key);

	initializeSemaphore(semaphoreId);
	changeSemaphore(1, semaphoreId, -1);

	printf("This process pid is: %d\n",key);
	printf("To continue type 'y'\n");
	while(getchar() != 'y');

	slaves = createSlaves(NUM_OF_SLAVES, &readFds, &maxFd);

	filenames = parseFileList(argc, argv, &numOfFiles);

	while(finishedFiles != numOfFiles) {
		if(distributedFiles != numOfFiles)
			distributeFiles(slaves, filenames, &distributedFiles, numOfFiles);

		slaveListener(slaves, filenames, &finishedFiles, outputFile, sharedMemory, &sharedMemoryIndex, semaphoreId, &readFds, maxFd);
	}

	changeSemaphore(0, semaphoreId, -1);
	sharedMemory[0] = -1;
	changeSemaphore(0, semaphoreId, 1);

	stopSlaves(slaves);

	destroyMemory(sharedMemoryId, sharedMemory);
	destroySemaphore(semaphoreId);
	fclose(outputFile);
	free(slaves);
	free(filenames);
	return 0;
}
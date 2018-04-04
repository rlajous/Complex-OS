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
	int sharedMemoryIndex = 2;
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
	sharedMemory[1] = 0;

	semaphoreId = generateSemaphore(key);

	initializeSemaphore(semaphoreId);

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

	changeSemaphore(semaphoreId, -1);
	sharedMemory[1] = -1;
	changeSemaphore(semaphoreId, 1);

	stopSlaves(slaves);

	destroyMemory(sharedMemoryId, sharedMemory);
	fclose(outputFile);
	free(slaves);
	free(filenames);
	return 0;
}

void slaveListener(slave* slaves, char** filenames, int * finishedFiles, FILE * outputFile, char * sharedMemory, int * sharedMemoryIndex, int semaphoreId, fd_set* readFds, int maxFd) {
	int i, j;
	char message[MAX_FILENAME + MD5_LENGTH + 3] = {0};

	select(maxFd+1, readFds, NULL, NULL, NULL);
	for (i = 0; i < NUM_OF_SLAVES; i++) {
		if(FD_ISSET(slaves[i].readFd, readFds)) {
			if(readLine(&slaves[i], message)) {
				reformatMd5Output(message);
				fprintf(outputFile,"%s\n", message);

				changeSemaphore(semaphoreId, -1);
				j = 0;
				while(message[j] != '\0' && verifySharedMemoryIndexBounds(*sharedMemoryIndex)) {
					sharedMemory[(*sharedMemoryIndex)++] = message[j];
					j++;
				}
				if(verifySharedMemoryIndexBounds(*sharedMemoryIndex)) {
					sharedMemory[(*sharedMemoryIndex)++] = '\0';
					sharedMemory[0] = *finishedFiles + 1;
				}
				changeSemaphore(semaphoreId, 1);
				slaves[i].remainingFiles--;
				(*finishedFiles)++;
			}
		}
		FD_SET(slaves[i].readFd, readFds);
	}
}

slave* createSlaves(int slaveQuantity, fd_set * readFds, int * maxFd) {
	slave* slaves;
	int auxRead[2];
	int auxWrite[2];

	int i;
	pid_t auxPid;

	*maxFd = 0;

	slaves = (slave *) calloc(slaveQuantity, sizeof(slave));
	if (slaves == NULL) {
		perror("Couldn't allocate memory for slaves :( (Line 40)");
			exit(1);
	}

	for (i = 0; i < slaveQuantity; i++) {
		pipe(auxRead);
		pipe(auxWrite);
		FD_SET(auxRead[0], readFds);
		if(auxRead[0] > *maxFd)
			*maxFd = auxRead[0];
		slaves[i].readFd = auxRead[0];
		slaves[i].writeFd = auxWrite[1];
		auxPid = fork();
		switch (auxPid) {
			case -1:
				perror("Slave fork() failed.");
				abortProgram(slaves);
			case 0:
				dup2(auxWrite[0], STDIN_FILENO);
				dup2(auxRead[1], STDOUT_FILENO);
				close(auxWrite[0]);
				close(auxWrite[1]);
				close(auxRead[0]);
				close(auxRead[1]);
				execlp("./slave.o", "./slave.o" ,NULL);
				perror("Slave process exec() failed.");
				_exit(1);
			default:
				slaves[i].pid = auxPid;
				close(auxWrite[0]);
				close(auxRead[1]);
				break;
		}
	}

	return slaves;
}

void distributeFiles(slave* slaves, char** files, int* distributed, int fileQuantity) {
	int i, j;
	char distributingBatch = 0;

	for(i = 0; i < NUM_OF_SLAVES; i++) {
		if(fileQuantity - *distributed > 0) {
			for(j = 0; j < BATCH_SIZE && (fileQuantity - *distributed > 0); j++) {
				if(slaves[i].remainingFiles == 0 || distributingBatch) {
					write(slaves[i].writeFd, files[*distributed], strlen(files[*distributed]) + 1);
					(*distributed)++;
					slaves[i].remainingFiles++;
					distributingBatch = 1;
				}
			}
			distributingBatch = 0;
		}
		else
			stopSlave(&slaves[i]);
	}
}

void abortProgram(slave* slaves) {
	int i;
	for (i = 0; i < NUM_OF_SLAVES; i++) {
		if (slaves[i].pid != 0)
			kill(slaves[i].pid, SIGINT);
	}
	exit(1);
}

void stopSlave(slave* slave) {
	char code = STOP_CODE;
	write(slave->writeFd, &code, 1);
	write(slave->writeFd, "\0", 1);
	slave->pid = 0;
}

void stopSlaves(slave* slaves) {
	int i;
	for (i = 0; i < NUM_OF_SLAVES; i++) {
		if (slaves[i].pid != 0)
			stopSlave(&slaves[i]);
	}
}

int readLine(slave* slave, char * buffer) {
	int messageLength = 0;

	while(read(slave->readFd, buffer, 1) > 0 && *buffer != '\n') {
		buffer++;
		messageLength++;
	}

	if(*buffer == '\n')
		*buffer = '\0';

	return messageLength > 0;
}

void reformatMd5Output(char * output) {
	char hash[MD5_LENGTH + 3] = ": ";
	char filename[MAX_FILENAME + MD5_LENGTH + 2];
	strncpy(hash + 2, output, MD5_LENGTH);
	hash[MD5_LENGTH + 2] = '\0';
	strncpy(filename, (output + MD5_LENGTH + 2), MAX_FILENAME);
	strcat(filename, hash);
	strcpy(output, filename);
}

#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "fileListParser.h"
#include "sharedMemory.h"

#define NUM_OF_SLAVES 4
#define STOP_CODE 5
#define MAX_FILENAME 256
#define MD5_LENGTH 32
#define BATCH_SIZE 2

typedef struct {
	pid_t pid;
	int readFd;
	int writeFd;
	int remainingFiles;
} slave;

slave* createSlaves(int slaveQuantity, fd_set * readFds, int * maxfd);
void distributeFiles(slave * slaves, char ** files, int * distributed, int fileQuantity);
void slaveListener(slave * slaves, char ** filenames, int * finishedFiles, FILE * outputFile, char * sharedMemory, int * sharedMemoryIndex, int semaphoreId, fd_set * readFds, int maxFd);
void abortProgram(slave * slaves);
void stopSlave(slave * slave);
void stopSlaves(slave * slaves);
int readLine(slave * slave, char * buffer);
void reformatMd5Output(char * output);

#endif

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

#define NUM_OF_SLAVES 4
#define STOP_CODE 5
#define MAX_FILENAME 256
#define MD5_LENGTH 32
#define BATCH_SIZE 2
#define STATE_IDLE 0
#define STATE_BUSY 1

typedef struct {
	pid_t pid;
	int readFd;
	int writeFd;
	int remainingFiles;
} slave;

slave* createSlaves(int slaveQuantity);
void distributeFiles(slave* slaves, char** files, int slaveQuantity, int* distributed, int fileQuantity);
void slaveListener(slave* slaves, char** filenames, int slaveQuantity, int * finishedFiles);
void abortProgram(slave* slaves, int slaveQuantity);
void stopSlave(slave* slave);
void stopSlaves(slave* slaves, int slaveQuantity);
int readLine(slave* slave, char * buffer);
void reformatMd5Output(char * output);

#endif

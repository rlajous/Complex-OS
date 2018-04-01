#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#define NUM_OF_SLAVES 4
#define STOP_CODE 5
#define MAX_FILENAME 256
#define MD5_LENGTH 32

typedef struct {
	pid_t pid;
	int readFd;
	int writeFd;
	int remainingFiles;
} slave;

slave* createSlaves(int numberOfSlaves);
void startApplicationListener(slave * slaves);
void abortProgram(slave** slaves);
void stopSlave(slave* slave);
int readLine(slave* slave, char * buffer);
void reformatMd5Output(char * output);

#endif

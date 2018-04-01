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

#endif

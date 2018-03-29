#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define NUM_OF_SLAVES 4

typedef struct {
	pid_t pid;
	int readFd;
	int writeFd;
} slave;

slave* createSlaves(int numberOfSlaves);
void abortProgram(slave** slaves);
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

#define NUM_OF_SLAVES 4
#define STOP_CODE 5
#define MAX_FILENAME 256
#define MD5_LENGTH 32
#define MEMORY_BLOCK 10

typedef struct {
	pid_t pid;
	int readFd;
	int writeFd;
	int remainingFiles;
} slave;

slave* createSlaves(int numberOfSlaves);
void startApplicationListener(slave* slaves, char** fileNames, int* numOfSlaves);
void abortProgram(slave** slaves);
void stopSlave(slave* slave);
int readLine(slave* slave, char * buffer);
void reformatMd5Output(char * output);
void addFileToList(char* fileName, char** list, int* numOfFiles);
void addDirectoryFilesToList(char* dirPath, char** fileNames, int* numOfFiles);
int strEndsWithAsterix(char* str);
void removeAsterixFromStr(char* str);

#endif

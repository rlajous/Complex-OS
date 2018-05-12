#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>

void kill(int pid);

int addBackgroundProcess(char * filename, int argc, char * argv[]);

int getpid();

int runProcess(void * entryPoint, int argc, char * argv[]);

#endif

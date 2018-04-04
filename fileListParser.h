#ifndef FILELISTPARSER_H
#define FILELISTPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MEMORY_BLOCK 10

char ** parseFileList(int argc, char ** argv, int* numOfFiles);
void addFileToList(char* fileName, char*** list, int* numOfFiles);

#endif

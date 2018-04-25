#ifndef SLAVE_H
#define SLAVE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_SLAVES 4
#define STOP_CODE 5
#define MAX_FILENAME 256

void startSlaveListener(char * message);

#endif

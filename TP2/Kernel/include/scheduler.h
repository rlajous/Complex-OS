#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_PROCESES = 10;

typedef struct {
    uint64_t rsp;
    //processState
    int pid;
    //programCounter
    //registers
    //memoryLimits
} process_t;

uint64_t scheduler(int ticks, uint64_t rsp);
int getPid();
int findFirstAvailableSpace();

#endif
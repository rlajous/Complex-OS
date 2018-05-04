#ifndef MMU_H
#define MMU_H

#include <stdint.h>
#include <moduleLoader.h>
#include <lib.h>
#include <terminal.h>
#include <interruptions.h>
#include <types.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif

static const uint64_t PageSize = 0x200000;
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]);
void copyAndExecuteDefaultModule();
void * initializeKernelBinary();
//void * malloc(uint64_t size);
char** backupArguments(int argc, char * argv[]);
void clearBSS(void * bssAddress, uint64_t bssSize);
void * getStackBase();

#endif

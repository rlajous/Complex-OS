#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <buddyMemoryAllocator.h>
#include <types.h>

#define PROCESS_INITIAL_PAGES 2
#define MAX_NAME 50

typedef enum {RUNNING, READY, BLOCKED, SLEEPING} processState;

typedef struct {
    void * stack;
    void * entryPoint;
    void * memoryBase;
    processState state;
    int pid;
    char name[MAX_NAME];
} process_t;

//Source RowDaBoat Wyrm
typedef struct {
    //Registers restore context
    uint64_t gs;
    uint64_t fs;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    //iretq hook
    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t base;
} stackFrame_t;

process_t * createProcess(void * entryPoint, int argc, char * argv[], char * name);

void deleteProcess(process_t * process);

void callProcess(int argc, char * argv[], void * entryPoint);

void * fillStackFrame(void * entryPoint, void * stack, int argc, char * argv[]);

#endif

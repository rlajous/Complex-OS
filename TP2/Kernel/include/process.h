#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <buddyMemoryAllocator.h>
#include <types.h>

#define MAX_NAME 50
#define MAX_THREADS 32
#define THREAD_LIMIT_REACHED -1

typedef struct {
    //processState state;
    int pid;
    int ppid;
    char name[MAX_NAME];
    threadNode_t threads[MAX_THREADS];
    int currentThread;
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

void initializeThreads(process_t * process);

void deleteProcess(process_t * process);

void callProcess(int argc, char * argv[], void * entryPoint);

void * fillStackFrame(void * entryPoint, void * stack, int argc, char * argv[]);

int addThread(process_t * process, thread_t * thread);

int firstAvailableThreadSpace(process_t * process);

#endif

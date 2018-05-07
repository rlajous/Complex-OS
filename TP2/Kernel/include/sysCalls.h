#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <terminal.h>
#include <rtc.h>
#include <lib.h>
#include <MMU.h>
#include <process.h>
#include <mutex.h>
#include <buddyMemoryAllocator.h>

#define SYSCALLS 17

typedef int (*sys)(uint64_t rsi, uint64_t rdx, uint64_t rcx);

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size);

int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size);

int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx);

int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds);

int sysGetDate(uint64_t day, uint64_t month, uint64_t year);

int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx);

int sysExec(uint64_t filename, uint64_t argc, uint64_t argv);

int sysKill(uint64_t pid, uint64_t rdx, uint64_t rcx);

int sysPs(uint64_t rsi, uint64_t rdx, uint64_t rcx);

int sysGetPid(uint64_t rsi, uint64_t rdx, uint64_t rcx);

int sysAllocateMemory(uint64_t address, uint64_t size, uint64_t rcx);

int sysFreeMemory(uint64_t address, uint64_t rdx, uint64_t rcx);

int sysCreateMutex(uint64_t name, uint64_t rdx, uint64_t rcx);

int sysReleaseMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx);

int sysUpMutex(uint64_t rsi, uint64_t rdx, uint64_t rcx);

int sysDownMutex(uint64_t rsi, uint64_t rdx, uint64_t rcx);

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

void sysCallsSetup();

#endif

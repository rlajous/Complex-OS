#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size);

int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size);

int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx);

int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds);

int sysGetDate(uint64_t day, uint64_t month, uint64_t year);

int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx);

int sysExec(uint64_t filename, uint64_t argc, uint64_t argv);

//int sysMalloc(uint64_t address, uint64_t size, uint64_t rcx);

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

void sysCallsSetup();

#endif

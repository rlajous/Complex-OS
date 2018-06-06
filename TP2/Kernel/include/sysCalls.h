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
#include <channel.h>
#include <timer.h>

#define SYSCALLS 34

typedef int (*sys)(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size, uint64_t r8);

int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size, uint64_t r8);

int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds, uint64_t r8);

int sysGetDate(uint64_t day, uint64_t month, uint64_t year, uint64_t r8);

int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysRunModule(uint64_t filename, uint64_t argc, uint64_t argv, uint64_t background);

int sysKill(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysPs(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysGetPid(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysAllocateMemory(uint64_t address, uint64_t size, uint64_t rcx, uint64_t r8);

int sysFreeMemory(uint64_t address, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysCreateMutex(uint64_t name, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysReleaseMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysUpMutex(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysDownMutex(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysCreateChannel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysSend(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysReceive(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysDeleteChannel(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysWriteCharAtScreenPos(uint64_t ch, uint64_t x, uint64_t y, uint64_t r8);

int sysRunProcess(uint64_t entryPoint, uint64_t argc, uint64_t argv, uint64_t background);

int sysCreateSemaphore(uint64_t name, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysReleaseSemaphre(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysSignal(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysWait(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysOpenPipe(uint64_t name, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysClosePipe(uint64_t pipe, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysAddThread(uint64_t entryPoint, uint64_t argc, uint64_t argv, uint64_t r8);

int sysKillThread(uint64_t thread, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysJoinThread(uint64_t thread, uint64_t rdx, uint64_t rcx, uint64_t r8);

int sysPipeStd(uint64_t pid, uint64_t isWriter, uint64_t rcx, uint64_t r8);

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

void sysCallsSetup();

#endif

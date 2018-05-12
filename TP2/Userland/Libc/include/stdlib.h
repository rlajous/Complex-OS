#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

int addBackgroundProcess(char * filename, int argc, char * argv[]);

int getpid();

void deleteChannel(int recipientPid);

int createChannel(int recipientPid);

int receive(int senderPid, char * buffer, int length);

int createMutex(char * name);

int createSemaphore(char * name);

void mutexUp(int mutex);

void mutexDown(int mutex);

void releaseMutex(int mutex);

void signal(int semaphore);

void wait(int semaphore);

void releaseSemaphore(int semaphore);

int runProcess(void * entryPoint, int argc, char * argv[]);

void kill(int pid);

int itoa(int value, char * buffer, int base);

int ftoa(float value, char * buffer, int precision); 

int isAlpha(char c);

int isDigit(char c);

#endif

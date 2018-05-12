#ifndef IPC_H
#define IPC_H

#include <stdlib.h>

int createChannel(int recipientPid);

int send(int recipientPid, char * message, int length);

int receive(int senderPid, char * buffer, int length);

void deleteChannel(int recipientPid);

int createMutex(char * name);

void releaseMutex(int mutex);

void mutexUp(int mutex);

void mutexDown(int mutex);

int createSemaphore(char * name);

void releaseSemaphore(int semaphore);

void signal(int semaphore);

void wait(int semaphore);

#endif

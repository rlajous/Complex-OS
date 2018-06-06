#include <ipc.h>

int createChannel(int recipientPid) {
  return int80(18, (uint64_t) recipientPid, 0, 0, 0);
}

int send(int recipientPid, char * message, int length) {
  return int80(19, (uint64_t) recipientPid, (uint64_t) message, (uint64_t) length, 0);
}

int receive(int senderPid, char * buffer, int length) {
  return int80(20, (uint64_t) senderPid, (uint64_t) buffer, (uint64_t) length, 0);
}

void deleteChannel(int recipientPid) {
  int80(21, (uint64_t) recipientPid, 0, 0, 0);
}

int createMutex(char * name) {
  return int80(14, (uint64_t) name, 0, 0, 0);
}

void releaseMutex(int mutex) {
  int80(15, (uint64_t) mutex, 0, 0, 0);
}

void mutexUp(int mutex) {
  int80(16, (uint64_t) mutex, 0, 0, 0);
}

void mutexDown(int mutex) {
  int80(17, (uint64_t) mutex, 0, 0, 0);
}

int createSemaphore(char * name) {
  return int80(23, (uint64_t) name, 0, 0, 0);
}

void releaseSemaphore(int semaphore) {
  int80(24, (uint64_t) semaphore, 0, 0, 0);
}

void signal(int semaphore) {
  int80(25, (uint64_t) semaphore, 0, 0, 0);
}

void wait(int semaphore) {
  int80(26, (uint64_t) semaphore, 0, 0, 0);
}

int openPipe(char * name) {
  return int80(28, (uint64_t) name, 0, 0, 0);
}

int closePipe(int pipe) {
  return int80(29, (uint64_t) pipe, 0, 0, 0);
}

int shellPipe(char * filename, int argc, char * argv[], int isWriter) {
  return int80(33, (uint64_t) filename, argc, (uint64_t) argv, (uint64_t) isWriter);
}
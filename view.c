#include "sharedMemory.h"

int main(int argc, char** argv) {

  char * sharedMemory;
	int semaphoreId;
	int sharedMemoryId;
	key_t key = atoi(argv[1]);

  int index = 1;
  int running = 1;
  int c;

  sharedMemory = generateSharedMemory(key, &sharedMemoryId);

	semaphoreId = generateSemaphore(key);

  while(running) {
    changeSemaphore(1, semaphoreId, -1);
    changeSemaphore(0, semaphoreId, -1);
    if(sharedMemory[0] != -1) {
      while((c = sharedMemory[index++]) != '\0')
        putchar(c);
      putchar('\n');
    }
    else
      running = 0;
    changeSemaphore(0, semaphoreId, 1);
  }

  detachMemory(sharedMemory);
  destroySemaphore(semaphoreId);
  return 0;
}

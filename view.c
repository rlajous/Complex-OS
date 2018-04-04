#include "sharedMemory.h"

int main(int argc, char** argv) {

  char * sharedMemory;
	int semaphoreId;
	int sharedMemoryId;
	key_t key = atoi(argv[1]);

  int index = 2;
  int files = 0;
  int running = 1;
  int c;

  sharedMemory = generateSharedMemory(key, &sharedMemoryId);

	semaphoreId = generateSemaphore(key);

  while(running) {
    changeSemaphore(semaphoreId, -1);
    if(files < sharedMemory[0] || sharedMemory[1] != -1) {
      if(files < sharedMemory[0]) {
        while((c = sharedMemory[index++]) != '\0')
          putchar(c);
        files++;
        putchar('\n');
      }
    }
    else
      running = 0;
    changeSemaphore(semaphoreId, 1);
  }

  detachMemory(sharedMemory);
  return 0;
}

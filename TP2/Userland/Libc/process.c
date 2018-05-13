#include <process.h>

void kill(int pid) {
  int80(8, (uint64_t) pid, 0, 0, 0);
}

int runModule(char * filename, int argc, char * argv[], int background) {
  return int80(7, (uint64_t) filename, argc, (uint64_t) argv, (uint64_t) background);
}

int getpid() {
  return int80(10, 0, 0, 0, 0);
}

int runProcess(void * entryPoint, int argc, char * argv[], int background) {
  return int80(11, (uint64_t) entryPoint, argc, (uint64_t) argv,(uint64_t) background);
}
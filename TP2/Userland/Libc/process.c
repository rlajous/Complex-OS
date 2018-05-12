#include <process.h>

void kill(int pid) {
  int80(8,(uint64_t) pid,0,0);
}

int addBackgroundProcess(char * filename, int argc, char * argv[]) {
  return int80(11, (uint64_t) filename, argc, (uint64_t) argv);
}

int getpid() {
  return int80(10, 0,0,0);
}

int runProcess(void * entryPoint, int argc, char * argv[]) {
  return int80(23, (uint64_t) entryPoint, argc, (uint64_t) argv);
}
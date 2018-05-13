#include <process.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int pid;
  if(argc > 1) {
  	parseInt(argv[1], &pid);
  	kill(pid);
  }

  return 0;
}

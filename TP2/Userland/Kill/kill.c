#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int pid;
  if(argc > 0) {
  	parseInt(argv[0], &pid);
  	kill(pid);
  }

  return 0;
}

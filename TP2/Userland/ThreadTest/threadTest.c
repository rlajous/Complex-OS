#include <stdlib.h>
#include <stdio.h>
#include <process.h>

int threadFun(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	int i = 0;
  char * args[1];
  char num[3];
  int threads[3];

	for(i = 0; i < 3; i++) {
		itoa(i + 1, num, 10);
		args[0] = num;
		threads[i] = addThread(threadFun, 1, args);
	}

  joinThread(threads[0]);
  return 0;
}

int threadFun(int argc, char *argv[]) {
  int num;
  parseInt(argv[0], &num);
  printf("Thread %d\n", num);
	while(1);
	return 0;
}
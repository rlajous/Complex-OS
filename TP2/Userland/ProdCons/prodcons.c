#include <stdlib.h>
#include <stdio.h>
#include "prodcons.h"

#define MAX_PRODUCERS 5
#define MAX_CONSUMERS 5

int producerPid[MAX_PRODUCERS];
int consumerPid[MAX_CONSUMERS];

int consumerIndex = 0;
int producerIndex = 0;


int main(int argc, char *argv[]) {
	char c, aux;
	int control = 1;

	printf("Type:\n\tq to quit\n\tp to add producer\n\tk to kill producer\n\tc to add consumer\n\tx to kill consumer\n\ts for status\n\th for help\n");

  while(control != -1) {
    c = getchar();
    while(aux != '\n')
    	aux = getchar();
    aux = 0;
    control = processControl(c);
  }

  return 0;
}

int processControl(char option) {

	char buffer[11];
	char num[11];
	char * arguments[] = {buffer, num};
	int ret = 1;

	switch(option) {
		case 'q':
			killAllProcesses();
			ret = -1;
			break;

		case 'p':
			producerIndex++;
			itoa(getpid(), buffer, 10);
			printf("Added Producer\n");
			addBackgroundProcess("producer", 1, arguments);
			break;

		case 'k':
			producerIndex--;
			deleteChannel(producerPid[producerIndex]);
			printf("Killed Producer\n");
			kill(producerPid[producerIndex]);
			break;

		case 'c':
			consumerIndex++;
			itoa(getpid(), buffer, 10);
			itoa(consumerIndex, num, 10);
			printf("Added Consumer\n");
			addBackgroundProcess("consumer", 2, arguments);
			break;

		case 'x':
			consumerIndex--;
			deleteChannel(consumerPid[consumerIndex]);
			printf("Killed Consumer %d\n", consumerPid[consumerIndex]);
			kill(consumerPid[consumerIndex]);
			break;

		case 's':
			printf("%d Producers, %d Consumers\n", producerIndex, consumerIndex);
			break;

		case 'h':
			printf("Type:\n\tq to quit\n\tp to add producer\n\tk to kill producer\n\tc to add consumer\n\tx to kill consumer\n\ts for status\n\th for help\n");
			break;

		default:
			printf("Unknown command\n");
			break;
	}

	return ret;
}

void killAllProcesses() {
	int i = consumerIndex - 1;

	while(i-- >= 0) {
		deleteChannel(consumerPid[i]);
		//kill(consumerPid[i]);
	}

	i = producerIndex - 1;

	while(i-- >= 0) {
		deleteChannel(producerPid[i]);
		kill(producerPid[i]);
	}
}

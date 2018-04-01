#include "slave.h"

int main(int argc, char ** argv) {
	char message[MAX_FILENAME];
	char * current;
	char running = 1;
	current = message;

	while(running) {
		while((*current = getchar()) !='\0') {
			current++;
		}

		if(message[0] != STOP_CODE) {
			startSlaveListener(message);
			current = message;
		}
		else {
			running = 0;
		}
	}
	return 0;
}

void startSlaveListener(char * message) {
	char command[7 + MAX_FILENAME] = "md5sum ";
	strcat(command,message);
	system(command);
}

#include "tp.h"

void md5(char * fileName); 

int main(int argc, char ** argv)
{
	slave * slaves;

	slaves = createSlaves(NUM_OF_SLAVES);
	//startApplicationListener();

	// if (argc < 1){
	// 	printf("Please specify a file or set of files to hash.\n");
	// 	exit(EXIT_FAILURE);
	// }
	int hashedFiles = 0;
	int currentArg = 0;
}

// void startApplicationListener(){
// 	// int msgLength;
// 	// char message[30];

// 	// while (1){
// 	// 	for (int i = 0; i < NUM_OF_SLAVES; i++){
// 	// 		if ((msgLength = read(slaves[i].readFd, message, 30)) != 0)
// 	// 			printf("El hijo dijo %s\n", message);
// 	// 	}
		
// 	// }
// }

slave* createSlaves(int numberOfSlaves){
	slave* slaves;
	int auxRead[2];
	int auxWrite[2];

	int i;
	pid_t auxPid;

	slaves = (slave *) calloc(numberOfSlaves, sizeof(slave));
	if (slaves == NULL) {
		perror("Couldn't allocate memory for slaves :( (Line 40)");
			exit(1);
	}

	for (i = 0; i < NUM_OF_SLAVES; i++) {
		pipe(auxRead);
		pipe(auxWrite);
		slaves[i].readFd = auxRead[0];
		slaves[i].writeFd = auxWrite[1];
		fcntl(slaves[i].readFd, F_SETFL, O_NONBLOCK);
		auxPid = fork();
		//TODO: agustin rompe las pelotas por un switch
		if (auxPid == -1) {
			perror("Slave fork() failed.");
			abortProgram(&slaves);
		}
		else if (auxPid == 0) {
			dup2(auxWrite[0], STDIN_FILENO);
			dup2(auxRead[1], STDOUT_FILENO);
			close(auxWrite[0]);
			close(auxWrite[1]);
			close(auxRead[0]);
			close(auxRead[1]);
			execlp("./slave.o", "./slave.o" ,NULL);
			perror("Slave process exec() failed.");
			//(write)Avisarle a papa que me voy a pegar un corchazo
			_exit(1);
		}
		else {
			slaves[i].pid = auxPid;
			close(auxWrite[0]);
			close(auxRead[1]);
		}

	}

	return slaves;
}

void abortProgram(slave** slaves) {
	int i;
	for (i = 0; i < NUM_OF_SLAVES; i++) {
		if (slaves[i]->pid != 0)
			kill(slaves[i]->pid, SIGINT);
	}
	exit(1);
}

void stopSlave(slave* slave) {
	//write(slave->writeFd, STOP_CODE, STOP_CODE_LEN);
}

// void startChildListener(){
// 	int messageLength;
// 	char* message = malloc(30);

// 	receivedLen = read(fd[0], message, 20);
// 	while (strcmp(parentMessage, "enough") != 0){
// 			int i = 0;
// 			char c;
// 			while ((messageLength = read(fd[0], parentMessage, 20)) == -1);
// 			printf("%s\n", parentMessage);
// 		}
// }

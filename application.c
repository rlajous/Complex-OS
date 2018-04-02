#include "application.h"

int main(int argc, char ** argv) {
	slave * slaves;
	char ** filenames;
	int slaveQuantity = NUM_OF_SLAVES;
	int numOfFiles = 2;
	int distributedFiles = 0;
	int finishedFiles = 0;

	if (argc < 2){
		printf("No files were specified to be hashed.\n");
		exit(1);
	}

	slaves = createSlaves(slaveQuantity);

	filenames = parseFileList(argc, argv, &numOfFiles);

	while(distributedFiles != numOfFiles) {
		distributeFiles(slaves, filenames, slaveQuantity, &distributedFiles, numOfFiles);

		slaveListener(slaves, filenames, slaveQuantity, &finishedFiles);
	}

	while(finishedFiles != numOfFiles) {
		slaveListener(slaves, filenames, slaveQuantity, &finishedFiles);
	}

	stopSlaves(slaves, slaveQuantity);

	free(slaves);
	free(filenames);
	return 0;
}

void slaveListener(slave* slaves, char** filenames, int slaveQuantity, int * finishedFiles) {
	int i;
	char message[MAX_FILENAME + MD5_LENGTH + 3] = {0};

	for (i = 0; i < slaveQuantity; i++) {
		if(readLine(&slaves[i], message)) {
			reformatMd5Output(message);
			slaves[i].remainingFiles--;
			(*finishedFiles)++;
			printf("Hijo %d: %s\n", i,message);
		}
	}
}

slave* createSlaves(int slaveQuantity) {
	slave* slaves;
	int auxRead[2];
	int auxWrite[2];

	int i;
	pid_t auxPid;

	slaves = (slave *) calloc(slaveQuantity, sizeof(slave));
	if (slaves == NULL) {
		perror("Couldn't allocate memory for slaves :( (Line 40)");
			exit(1);
	}

	for (i = 0; i < slaveQuantity; i++) {
		pipe(auxRead);
		pipe(auxWrite);
		slaves[i].readFd = auxRead[0];
		slaves[i].writeFd = auxWrite[1];
		fcntl(slaves[i].readFd, F_SETFL, O_NONBLOCK);
		auxPid = fork();
		switch (auxPid) {
			case -1:
				perror("Slave fork() failed.");
				abortProgram(slaves, slaveQuantity);
			case 0:
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
			default:
				slaves[i].pid = auxPid;
				close(auxWrite[0]);
				close(auxRead[1]);
				break;
		}
	}

	return slaves;
}

void distributeFiles(slave* slaves, char** files, int slaveQuantity, int* distributed, int fileQuantity) {
	int i, j;
	char distributingBatch = 0;

	for(i = 0; i < slaveQuantity; i++) {
		if(fileQuantity - *distributed > 0) {
			for(j = 0; j < BATCH_SIZE && (fileQuantity - *distributed > 0); j++) {
				if(slaves[i].remainingFiles == 0 || distributingBatch) {
					//printf("File %s to slave %d\n", files[*distributed], i);
					write(slaves[i].writeFd, files[*distributed], strlen(files[*distributed]) + 1);
					(*distributed)++;
					slaves[i].remainingFiles++;
					distributingBatch = 1;
				}
			}
			distributingBatch = 0;
		}
		else
			stopSlave(&slaves[i]);
	}
}

void abortProgram(slave* slaves, int slaveQuantity) {
	int i;
	for (i = 0; i < slaveQuantity; i++) {
		if (slaves[i].pid != 0)
			kill(slaves[i].pid, SIGINT);
	}
	exit(1);
}

void stopSlave(slave* slave) {
	char code = STOP_CODE;
	write(slave->writeFd, &code, 1);
	write(slave->writeFd, "\0", 1);
	slave->pid = 0;
}

void stopSlaves(slave* slaves, int slaveQuantity) {
	int i;
	for (i = 0; i < slaveQuantity; i++) {
		if (slaves[i].pid != 0)
			stopSlave(&slaves[i]);
	}
}

int readLine(slave* slave, char * buffer) {
	int messageLength = 0;

	while(read(slave->readFd, buffer, 1) > 0 && *buffer != '\n') {
		buffer++;
		messageLength++;
	}

	if(*buffer == '\n')
		*buffer = '\0';

	return messageLength > 0;
}

void reformatMd5Output(char * output) {
	char hash[MD5_LENGTH + 3] = ": ";
	char filename[MAX_FILENAME + MD5_LENGTH + 2];
	strncpy(hash + 2, output, MD5_LENGTH);
	hash[MD5_LENGTH + 2] = '\0';
	strncpy(filename, (output + MD5_LENGTH + 2), MAX_FILENAME);
	strcat(filename, hash);
	strcpy(output, filename);
}

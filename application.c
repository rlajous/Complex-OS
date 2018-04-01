#include "application.h"

int main(int argc, char ** argv) {
	slave * slaves;

	if (argc < 2){
		printf("No files were specified to be hashed.\n");
		exit(1);
	}

	char ** fileNames;
	int numOfFiles;
	struct stat statBuffer;
	fileNames = (char **) malloc(sizeof(char*) * MEMORY_BLOCK);
	numOfFiles = 0;
	int i;
	for (i = 1; i < argc; i++){
		if (stat(argv[i], &statBuffer) == 0 && !S_ISDIR(statBuffer.st_mode)){
			if (strEndsWithAsterix(argv[i])){
				removeAsterixFromStr(argv[i]);
				addDirectoryFilesToList(argv[i], fileNames, &numOfFiles);
			}
			else{
				addFileToList(argv[i], fileNames, &numOfFiles);
			}
		}
	}
	printf("Files to be hashed: \n");
	for (int j = 0; j < numOfFiles; j++){
		printf("%s\n", fileNames[j]);
	}

	slaves = createSlaves(NUM_OF_SLAVES);

	startApplicationListener(slaves, fileNames, numOfFiles);
	// if (argc < 1){
	// 	printf("Please specify a file or set of files to hash.\n");
	// 	exit(EXIT_FAILURE);
	// }
	return 0;
}

void startApplicationListener(slave* slaves, char** fileNames, int numOfFiles) {
	int hashedFiles;
	hashedFiles = 0;
	int slaveStates[NUM_OF_SLAVES];
	for (int i = 0; i < NUM_OF_SLAVES; i++){
		slaveStates[i] = STATE_IDLE;
	}
	char message[MAX_FILENAME + MD5_LENGTH + 3];
	int i, j;

 	while(1) {
		for (i = 0; i < NUM_OF_SLAVES; i++) {
			if (slaveStates[i] == STATE_IDLE && hashedFiles < numOfFiles){
				if (numOfFiles - hashedFiles >= BATCH_SIZE){
					for (j = 0; j < BATCH_SIZE; j++)
						write(slaves[i].writeFd, fileNames[hashedFiles + j], strlen(fileNames[hashedFiles + j]) + 1);
					hashedFiles += 2;
				} else {
					//printf("%s\n", fileNames[hashedFiles]);
					write(slaves[i].writeFd, fileNames[hashedFiles], strlen(fileNames[hashedFiles]) + 1);
					hashedFiles++;
				}
			}
			if(readLine(&slaves[i], message)) {
				reformatMd5Output(message);
				printf("Hijo %d: %s\n", i,message);
			}
		}
	}
}

slave* createSlaves(int numberOfSlaves) {
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
		switch (auxPid) {
			case -1:
				perror("Slave fork() failed.");
				abortProgram(&slaves);
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

void abortProgram(slave** slaves) {
	int i;
	for (i = 0; i < NUM_OF_SLAVES; i++) {
		if (slaves[i]->pid != 0)
			kill(slaves[i]->pid, SIGINT);
	}
	exit(1);
}

void stopSlave(slave* slave) {
	char code = STOP_CODE;
	write(slave->writeFd, &code, 1);
	write(slave->writeFd, "\0", 1);
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
	hash[MD5_LENGTH + 3] = '\0';
	strncpy(filename, (output + MD5_LENGTH + 2), MAX_FILENAME);
	strcat(filename, hash);
	strcpy(output, filename);
}

void addFileToList(char* fileName, char** list, int* numOfFiles){
	if (*numOfFiles % MEMORY_BLOCK == 0){
		list = realloc(list, sizeof(char*) * (*numOfFiles + MEMORY_BLOCK));
		if (list == NULL){
			perror("Error allocating memory.");
			exit(1);
		}
	}
	list[(*numOfFiles)++] = fileName;
}

void addDirectoryFilesToList(char* dirPath, char** fileNames, int* numOfFiles){
	DIR * dirp = opendir(dirPath);
	if (dirp != NULL){
		struct dirent *direntry;
		while ((direntry = readdir(dirp)) != NULL){
			if (direntry->d_type == DT_REG){
				addFileToList(strcat(dirPath, strcat("/", direntry->d_name)), fileNames, numOfFiles);
			}
			else if (direntry->d_type == DT_DIR){
				printf("Files from the %s directory have NOT been included.\n", direntry->d_name);
			}
		}
	}
}
int strEndsWithAsterix(char* str){
	int i = 0;
	while (str[i++] != '\0');
	return str[--i] == '*';
}
void removeAsterixFromStr(char* str){
	int i = 0;
	while (str[i++] != '\0');
	str[--i] = '\0';
}
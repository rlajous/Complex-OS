#include "test.h"

slave * slaves;
char ** filenames = NULL;
int numOfFiles = 0;
int distributedFiles = 0;
fd_set readFds;
int maxFd;
int numOfSlaves;

char message[MAX_FILENAME+ MD5_LENGTH +3] = {'\0'};

int amountOfFiles;
char * inputfilenames[2];

int main() {

	printf("Test Slaves:\n");

	testSlaves();

	printf("Test File Parser: \n");

	testFileParser();
}

void testFileParser() {

	givenArgvFiles();

	whenFilesParsedToString();

	thenCheckIsNotNull();

	thenCheckAreTheSameGiven();

	thenFilesSuccess();

}

void testSlaves() {	

	givenFiles();

	givenAnAmountOfSlaves();

	whenSlaveISCreated();

	thenSlavesAreSuccsesfullyCreated();

	thenSlaveComunicationIsChecked();
	
	thenSlaveSuccess();
	
}


void givenAnAmountOfSlaves() {

	numOfSlaves=NUM_OF_SLAVES;

}

void whenSlaveISCreated() {

	FD_ZERO(&readFds);

	slaves=createSlaves(numOfSlaves, &readFds, &maxFd);

}

void thenSlaveSuccess() {

	stopSlaves(slaves);

	free(slaves);

}

void thenSlaveComunicationIsChecked() {
	
	int i, count = 0;

	distributeFiles(slaves,inputfilenames,&distributedFiles,amountOfFiles);
	sleep(1);
	select(maxFd+1, &readFds, NULL, NULL, NULL);
	for (i = 0; i < NUM_OF_SLAVES; i++)  {
		if(FD_ISSET(slaves[i].readFd, &readFds))  {
			if(readLine(&slaves[i], message))  {
				assert(*message != '\0');
				count++;
			}
		}
	}
	assert(count == 1);
	printf("Slave comunication is ok! :)\n");

}
void givenArgvFiles() {
	amountOfFiles=2;
	inputfilenames[1]="test.c";
}

void givenFiles() {
	amountOfFiles=1;
	inputfilenames[0]="test.c";
}

void whenFilesParsedToString() {
	filenames = parseFileList(amountOfFiles, inputfilenames, &numOfFiles);
}
void thenCheckAreTheSameGiven() {
	assert(strcmp(filenames[0],inputfilenames[1]) == 0);
	printf("Chek if both are equal is ok! :)\n");
}
void thenSlavesAreSuccsesfullyCreated() {
	int i;
	assert(slaves != NULL);
	for ( i = 0; i < NUM_OF_SLAVES; i++)
	 {
		assert(slaves[i].pid != 0);
	}
	printf("Slave creation is ok! :)\n");

}
void thenCheckIsNotNull() {
	assert(filenames != NULL);
	printf("Check if not NULL is ok! :)\n");
}

void thenFilesSuccess() {
	free(filenames);
}
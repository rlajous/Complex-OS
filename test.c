#include "testlib.h"

slave * slaves;
char ** filenames;
int numOfFiles = 0;
int distributedFiles = 0;
int finishedFiles = 0;
fd_set readFds;
int maxFd;
int numOfSlaves;

FILE * outputFile;

char * sharedMemory;
int sharedMemoryIndex = 2;
int semaphoreId;
int sharedMemoryId;
key_t key;
FD_ZERO(&readFds);

int amountOfFiles;
char ** inputfilenames;

int main()
{
	printf("Test sheared memory: ");
	testShearedMemory();

	printf("Test slaves: ");
	testSlave();

	printf("Test File Parser: ");
	testFileParser();
}


void testShearedMemory()
{

	givenAKey();

	whenMemoryISCreated();
	whenSemaphoreIsCreated();
	whenOtherProcesWritesMemory();

	thenSomeMemoryIsWritten();
	thenSemaphoreBlocksRW();
	thenWriteInMemory();

	thenSuccess();


}


void testSlaves()
{

	givenAnAmountOfSlaves();

	whenSlaveISCreated();

	thenSlaveComunicationIsChecked();
	/*
	whenMemoryISCreated();
	whenSemaphoreIsCreated();
	whenOtherProcesWritesMemory();

	thenSomeMemoryIsWritten();
	thenSemaphoreBlocksRW();
	thenWriteInMemory();
*/
	thenSlaveSuccess();
	
}


void testFileParser()
{

	givenFiles();

	whenFilesParsedToString();

	thenCheckAreTheSameGIven();
	/*
	whenMemoryISCreated();
	whenSemaphoreIsCreated();
	whenOtherProcesWritesMemory();

	thenSomeMemoryIsWritten();
	thenSemaphoreBlocksRW();
	thenWriteInMemory();
	*/
	thenSuccess();

}

void givenAKey()
{
	key=1996;

}

void whenMemoryISCreated()
{
	sharedMemory=generateSharedMemory(key, &sharedMemoryId);
}

void givenAnAmountOfSlaves()
{
	numOfSlaves=4;
}

void whenSlaveISCreated()
{
	slaves=createSlaves(numOfSlaves, &readFds, &maxFd);
}

void thenSlaveSuccess()
{
	stopSlaves(slaves);
}

void thenSlaveComunicationIsChecked()
{

}
void givenFiles(){
	
}
void whenFilesParsedToString()
{
	filenames = parseFileList(numOfFiles, inputfilenames, &numOfFiles);
}
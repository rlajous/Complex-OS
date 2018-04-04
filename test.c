#include "testlib.h"


char * sharedMemory;
int semaphoreId;
int sharedMemoryId;
key_t key


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
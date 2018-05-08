#include "assert.h"
#include <schedulerTest.h>

static roundRobinNode_t processes[MAX_PROCESSES];
static void * schedulerAuxiliaryStack;
static process_t * process;

int main() {
	testSchedulerInitialization();
	
	testAddProcess();
	
	testRemoveProcess();

	freeResources();
}

void testSchedulerInitialization() {
	printf("Test Scheduler Initialization...........");

	whenSchedulerIsInitialized();

	thenCheckSchedulerNotNull();

	thenSuccess();
}

void testAddProcess() {
	printf("Test Add Process...........");

	whenProcessIsAdded();

	thenCheckProcessCanBeFoundWithId();

	thenSuccess();
}


void testRemoveProcess() {
	printf("Test Remove Process...........");

	whenProcessIsRemoved();

	thenCheckProcessCannotBeFoundWithId();

	thenSuccess();
	
}

void whenSchedulerIsInitialized() {
	int i;
	schedulerAuxiliaryStack = allocatePages(1);
  	for(i = 0; i < MAX_PROCESSES; i++) {
    	processes[i].process = NULL;
  	}
}

void thenCheckSchedulerNotNull() {
	int i;
	assert(schedulerAuxiliaryStack != NULL);
	for(i = 0; i < MAX_PROCESSES; i++) {
    	assert(processes[i].process = NULL);
  	}
  	printf("Scheduler Initialization is OK\n");
}

void thenSuccess() {
	printf("OK\n");
}










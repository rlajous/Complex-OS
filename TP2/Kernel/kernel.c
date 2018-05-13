#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <terminal.h>
#include <MMU.h>
#include <buddyMemoryAllocator.h>
#include <scheduler.h>
#include <mutex.h>
#include <semaphore.h>
#include <timer.h>

int main(void) {
	clearScreen();
	initializeHeap();
	initializeScheduler();
	initializeTimer();
	setupMutexSystem();
	setupSemaphoreSystem();
	setupIDT();

	startSystem();
	return 0;
}

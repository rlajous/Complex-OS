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
#include <channel.h>

int main(void) {
	clearScreen();
	setupMutexSystem();
	initializeHeap();
	initializeScheduler();
	initializeTimer();
	setupSemaphoreSystem();
	initializeChannels();

	setupIDT();

	startSystem();
	return 0;
}

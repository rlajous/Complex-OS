#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <terminal.h>
#include <MMU.h>
#include <buddyMemoryAllocator.h>

int main(void) {
	clearScreen();
	setupIDT();
	initializeHeap();

	copyAndExecuteDefaultModule();

	return 0;
}

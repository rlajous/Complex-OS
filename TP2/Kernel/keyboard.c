#include <keyboard.h>
#include <terminal.h>
#include <interruptions.h>
#include <lib.h>
#include <scanCodes.h>
#include <MMU.h>

static uint8_t shiftMayus = 0;
static uint8_t leftShift = FALSE;
static uint8_t rightShift = FALSE;
static uint8_t specialKey = FALSE;
static uint8_t ctrl = FALSE;

void keyboardHandler() {
	uint8_t input = readPort(0x60);
	parseScanCode(input);
}

void parseScanCode(uint8_t scanCode) {
	if(specialKey)
		parseSpecialKey(scanCode);
	else {
		switch(scanCode) {
			case LEFT_SHIFT_PRESS:
			case LEFT_SHIFT_RELEASE:
			case RIGHT_SHIFT_PRESS:
			case RIGHT_SHIFT_RELEASE:
			case CAPSLOCK_PRESS:
			case CAPSLOCK_RELEASE:
			case CTRL_PRESS:
			case CTRL_RELEASE:
				parseSpecialKey(scanCode);
				break;

			case SPECIAL_KEY:
				specialKey = TRUE;
				break;

			default:
				if(scanCode < 0x80) {
					char ch = scanCodes[shiftMayus][scanCode];
					if(ch != 0) {
						if(ctrl && ch == 'c') {
							/*sendEOI(1);
							copyAndExecuteDefaultModule();*/
						}
						else
							writeBuffer(ch);
					}
				}
				break;
		}
	}
}

void parseSpecialKey(uint8_t scanCode) {
	specialKey = FALSE;

	switch(scanCode) {
		case LEFT_SHIFT_PRESS:
			leftShift = TRUE;
			shiftMayus = shiftMayus | SHIFT;
			break;
		case RIGHT_SHIFT_PRESS:
			rightShift = TRUE;
			shiftMayus = shiftMayus | SHIFT;
			break;

		case LEFT_SHIFT_RELEASE:
			leftShift = FALSE;
			if(!(leftShift || rightShift))
				shiftMayus =  shiftMayus & NOT_SHIFT;
			break;

		case RIGHT_SHIFT_RELEASE:
			rightShift = FALSE;
			if(!(leftShift || rightShift))
				shiftMayus =  shiftMayus & NOT_SHIFT;
			break;

		case CAPSLOCK_PRESS:
			shiftMayus = shiftMayus ^ MAYUS;
			break;

		case CURSOR_LEFT:
			keyboardLeft();
			break;

		case CURSOR_RIGHT:
			keyboardRight();
			break;

		case CTRL_PRESS:
			ctrl = TRUE;
			break;

		case CTRL_RELEASE:
			ctrl = FALSE;
			break;

		default:
			break;
	}
}

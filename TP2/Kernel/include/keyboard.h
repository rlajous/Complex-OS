#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef  FALSE
#define FALSE 0
#endif

#define SHIFT 		0x1
#define NOT_SHIFT 	0x2
#define MAYUS 		0x2
#define NOT_MAYUS 	0x1

#define LEFT_SHIFT_PRESS 	0x2A
#define RIGHT_SHIFT_PRESS 	0x36
#define LEFT_SHIFT_RELEASE 	0xAA
#define RIGHT_SHIFT_RELEASE 0xB6
#define CAPSLOCK_PRESS 		0x3A
#define CAPSLOCK_RELEASE	0xBA
#define BACKSPACE_PRESS		0x0E
#define ENTER_PRESS			0x1C
#define SPECIAL_KEY			0xE0
#define CURSOR_UP			0x48
#define CURSOR_DOWN			0x50
#define CURSOR_LEFT			0x4B
#define CURSOR_RIGHT		0x4D
#define CTRL_RELEASE		0x9D
#define CTRL_PRESS			0x1D

void keyboardHandler();
void parseScanCode(uint8_t scanCode);
void parseSpecialKey(uint8_t scanCode);

#endif

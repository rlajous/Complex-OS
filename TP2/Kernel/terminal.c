#include <terminal.h>
#include <lib.h>
#include <video.h>
#include <scheduler.h>


static video_row* video = (video_row*) 0xB8000;
static char kbBuffer[BUFFER_SIZE];

static uint16_t startIndex = 0;
static uint16_t endIndex = 0;
static uint16_t writeIndex = 0;
static uint16_t readIndex = 0;
static uint16_t size = 0;

static uint8_t echo = TRUE;
static uint8_t cursorVisible = FALSE;

static uint8_t cursorX = 0;
static uint8_t cursorY = 0;
static char defaultStyle = 0x07;

static char buffer[64];

//==============================================================================
//  VIDEO
//==============================================================================

void toggleCursors();

void printChar(char ch, char style) {
	int x = cursorX;
	int y = cursorY;
	int prevX = -1;
	int prevY;
	while(video[y][x].ch != 0) {
		x++;
		if(x == WIDTH) {
			x = 0;
			if(y == HEIGHT-1) {
				if(cursorY == 0)
					return;
				shiftScreen();
				cursorY--;
			}
			else
				y++;
		}
	}
	while(x != cursorX || y != cursorY) {
		prevX = x-1;
		prevY = y;
		if(prevX < 0) {
			prevX = WIDTH-1;
			prevY--;
		}
		video[y][x] = video[prevY][prevX];
		x = prevX;
		y = prevY;
	}
	video[cursorY][cursorX].style = style;
	video[cursorY][cursorX].ch = ch;
	if(prevX == -1)
		writeAtCursor(ch, style);
	incrementCursor();
}

void printCharAtPos(char ch, int posX, int posY) {
	int x = posX;
	int y = posY;	
	if (posX < 0 || posX > WIDTH || posY < 0 || posY > HEIGHT)
		return;
	video[y][x].style = defaultStyle;
	video[y][x].ch = ch;
}

void printc(char ch) {
	printcWithStyle(ch, defaultStyle);
}

void printcWithStyle(char ch, char style) {
	switch(ch) {
		case 8:
			backspace();
			break;
		case 10:
			newLine();
			break;
		default:
			toggleCursors();
			printChar(ch, style);
			toggleCursors();
			break;
	}
}

void print(char *s) {
	toggleCursors();
	int i = 0;
	while(s[i] != 0) {
		printChar(s[i], defaultStyle);
		i++;
	}
	toggleCursors();
}

void println(char *s) {
	print(s);
	newLine();
}

void setStyle(char style) {
	defaultStyle = style;
}

void incrementCursor() {
	if(cursorX == WIDTH-1) {
		cursorX = 0;

		if(cursorY == HEIGHT-1)
			shiftScreen();
		else
			cursorY++;
	}
	else
		cursorX++;

	updateCursor(cursorX, cursorY);
}

void newLine() {
	toggleCursors();
	while (video[cursorY][cursorX].ch != 0) {
		if(cursorX == WIDTH-1) {
			cursorX = 0;
			if(cursorY == HEIGHT-1)
				shiftScreen();
			else
				cursorY++;
		}
		else
			cursorX++;

	}

	cursorX = 0;

	if(cursorY == HEIGHT-1)
		shiftScreen();
	else
		cursorY++;

	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void backspace() {
	if(cursorY > 0 || cursorX > 0) {
		toggleCursors();
		int x = cursorX;
		int y = cursorY;
		int prevX;
		int prevY;
		if(x == 0) {
			prevX = WIDTH-1;
			prevY = y-1;
		}
		else {
			prevX = x-1;
			prevY = y;
		}
		int offset = 0;
		cursorX = prevX;
		cursorY = prevY;
		while(prevY >= 0 && video[prevY][prevX].ch == 0) {
			offset++;
			cursorX = prevX;
			cursorY = prevY;
			prevX--;
			if(prevX < 0) {
				prevX = WIDTH-1;
				prevY--;
			}
		}
		if(offset == 0)
			offset = 1;

		do {
			prevX = x - offset;
			prevY = y;
			while (prevX < 0) {
				prevX += WIDTH;
				prevY--;
			}

			video[prevY][prevX] = video[y][x];
			video[y][x].ch = 0;

			if(x == WIDTH-1) {
				x = 0;
				y++;
			}
			else
				x++;

		} while(y < HEIGHT && video[prevY][prevX].ch != 0);

		updateCursor(cursorX, cursorY);
		toggleCursors();
	}
}

void cursorUp() {
	toggleCursors();
	if(cursorY > 0)
		cursorY--;
	while (cursorX > 0 && video[cursorY][cursorX].ch == 0)
		cursorX--;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorDown() {
	toggleCursors();
	if(cursorY < HEIGHT-1)
		cursorY++;
	while (cursorX > 0 && video[cursorY][cursorX].ch == 0)
		cursorX--;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorLeft() {
	toggleCursors();
	if(cursorX > 0)
		cursorX--;
	else if(cursorY > 0) {
		cursorY--;
		while(cursorX < WIDTH-1 && video[cursorY][cursorX].ch != 0)
			cursorX++;
	}
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void cursorRight() {
	toggleCursors();
	if(cursorX == WIDTH-1 || video[cursorY][cursorX].ch == 0) {
		if(cursorY < HEIGHT-1) {
			cursorY++;
			cursorX = 0;
		}
	}
	else
		cursorX++;
	updateCursor(cursorX, cursorY);
	toggleCursors();
}

void shiftScreen() {
	memcpy((uint8_t*) video[0], (uint8_t*) video[1], CELLSIZE*WIDTH*(HEIGHT-1));
	for(uint8_t x = 0; x < WIDTH; x++) {
		video[HEIGHT-1][x].ch = 0;
		video[HEIGHT-1][x].style = defaultStyle;
	}
}

void clearScreen() {
	toggleCursors();
	for(uint8_t y = 0; y < HEIGHT; y++) {
		for(uint8_t x = 0; x < WIDTH; x++) {
			video[y][x].ch = 0;
			video[y][x].style = defaultStyle;
		}
	}
	cursorX = 0;
	cursorY = 0;
	updateCursor(0, 0);
	toggleCursors();
}

uint8_t getCharAt(uint8_t x, uint8_t y) {
    return video[y][x].ch;
}

void printBase(uint64_t value, uint32_t base) {
	uintToBase(value, buffer, base);
	print(buffer);
}

void printDec(uint64_t value) {
	printBase(value, 10);
}

void printHex(uint64_t value) {
	printBase(value, 16);
}

void printBin(uint64_t value) {
	printBase(value, 2);
}

void toggleCursors() {
	if(cursorVisible) {
		video[cursorY][cursorX].style = 0x77 ^ video[cursorY][cursorX].style;
	}
}

void blinkCursor() {
	video[cursorY][cursorX].style = 0x77 ^ video[cursorY][cursorX].style;
	if(cursorVisible)
		cursorVisible = FALSE;
	else
		cursorVisible = TRUE;
}

//==============================================================================
//  KEYBOARD
//==============================================================================

void shiftLeft() {
	if(writeIndex != startIndex) {
		uint16_t from = writeIndex-1;
		if(from < 0)
			from = BUFFER_SIZE;
		uint16_t to;
		while(from != endIndex) {
			to = from;
			from++;
			if(from == BUFFER_SIZE)
				from = 0;
			kbBuffer[to] = kbBuffer[from];
		}
	}
}

void shiftRight() {
	if(writeIndex != endIndex) {
		uint16_t from = endIndex;
		uint16_t to;
		do {
			to = from;
			from--;
			if(from < 0)
				from = BUFFER_SIZE;
			kbBuffer[to] = kbBuffer[from];
		}	while(from != writeIndex);
	}
}

void writeBuffer(char ch) {
	switch (ch) {
		case '\b':
			if(writeIndex != startIndex) {
				shiftLeft();
				writeIndex--;
				if(writeIndex < 0)
					writeIndex = BUFFER_SIZE;
				endIndex--;
				if(endIndex < 0)
					endIndex = BUFFER_SIZE;
				size--;
				if(echo)
					printc(ch);
			}
			break;
		case '\n':
			kbBuffer[endIndex] = ch;
			endIndex++;
			size++;
			if(endIndex == BUFFER_SIZE)
				endIndex = 0;
			writeIndex = startIndex = endIndex;
			if(echo)
				printc(ch);
			unblockRead();
			break;
		default:
			if(size < BUFFER_SIZE-1) {					//Dejar un espacio para \n
				shiftRight();
				kbBuffer[writeIndex] = ch;
				writeIndex++;
				if(writeIndex == BUFFER_SIZE)
					writeIndex = 0;
				endIndex++;
				if(endIndex == BUFFER_SIZE)
					endIndex = 0;
				size++;
			}
			if(echo)
				printc(ch);
			break;
	}
}

char readBuffer() {
	char ch = 0;
	if(readIndex >= startIndex)
	  blockRead();
	if(readIndex < startIndex) {
		ch = kbBuffer[readIndex];
		readIndex++;
		if(readIndex == BUFFER_SIZE)
			readIndex = 0;
		size--;
	}
	return ch;
}

void keyboardLeft() {
	if(writeIndex != startIndex) {
		writeIndex--;
		if(writeIndex < 0)
			writeIndex = BUFFER_SIZE;
		if(echo)
			cursorLeft();
	}
}

void keyboardRight() {
	if(writeIndex != endIndex) {
		writeIndex++;
		if(writeIndex == BUFFER_SIZE)
			writeIndex = 0;
		if(echo)
			cursorRight();
	}
}

void setEcho(uint8_t boolean) {
	echo = boolean;
}

void resetBuffer() {
	startIndex = 0;
	endIndex = 0;
	writeIndex = 0;
	readIndex = 0;
}

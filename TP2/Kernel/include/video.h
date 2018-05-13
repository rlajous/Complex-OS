#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>
#include <types.h>

#define CELLSIZE 2

#ifndef TRUE
#define TRUE 1
#endif

#ifndef  FALSE
#define FALSE 0
#endif


typedef cell_t video_row[80];

void writeAtCursor(char ch, char style);
void writeScreen(uint8_t x, uint8_t y, char ch, char style);
void updateCursor(uint8_t x, uint8_t y);

#endif

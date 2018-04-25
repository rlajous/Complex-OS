#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

void * malloc(unsigned int size);

int itoa(int value, char * buffer, int base);

int ftoa(float value, char * buffer, int precision); 

int isAlpha(char c);

int isDigit(char c);

#endif

#ifndef STDIO_H
#define STDIO_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <strings.h>

#define BUFFER_SIZE 25*80+1000

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

void write(int fd, char* buffer, int size);

void read(int fd, char* buffer, int size);

void putchar(char c);

char getchar();

int printf(const char* format, ...);

int scanf(const char* format, ...);

int sscanf(const char* format, const char * str, ...);

int parseInt(const char* string, int * value);

int parseDouble(const char* string, double * value);

int readLine(char * buffer, int maxSize);

#endif

#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stddef.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
int strcmp(const char *str1, const char *str2);
size_t strlen(const char *str);

char *cpuVendor(char *result);
void writePort(uint8_t port, uint8_t value);
int readPort(uint8_t port);

#endif
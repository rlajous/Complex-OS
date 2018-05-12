#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

size_t strlen(const char *str);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *strncpy(char *dest, const char *src, size_t n);

void toUpperCase(char *str);

#endif
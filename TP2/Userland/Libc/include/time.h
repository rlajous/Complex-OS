#ifndef TIME_H
#define TIME_H

#include <stdint.h>

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx); 

void updateTime();

void updateDate();

int getHour();

int getMinutes();

int getSeconds();

int getDay();

int getMonth();

int getYear();

void setTimeZone(int tz);

#endif

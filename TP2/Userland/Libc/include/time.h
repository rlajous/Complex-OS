#ifndef TIME_H
#define TIME_H

#include <stdint.h>

typedef struct {
    int hours;
    int minutes;
    int seconds;
} time;

typedef struct {
    int day;
    int month;
    int year;
} date;

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8);

void updateTime(time * currentTime);

void updateDate(date * currentDate);

int getHour();

int getMinutes();

int getSeconds();

int getDay();

int getMonth();

int getYear();

void setTimeZone(int tz);

#endif

#ifndef _RTC_H_
#define _RTC_H_

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define WEEKDAY 0x06
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09

#include <stdint.h>

uint8_t getTime(uint8_t value);

void recalculate(int * hours, int * day, int * month, int * year);

uint8_t days(int month);

void setTimeZone(int tz);

void setupRTC();

#endif

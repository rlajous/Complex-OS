#include <rtc.h>
#include <lib.h>

static int timeZone = -3;

static uint8_t daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //No considerardo bisiesto

uint8_t getTime(uint8_t value) {
  int seconds, minutes, hours, day, month, year;

  writePort(0x70, SECONDS);
  seconds = readPort(0x71);
  writePort(0x70, MINUTES);
  minutes = readPort(0x71);
  writePort(0x70, HOURS);
  hours = readPort(0x71);
  writePort(0x70, DAY);
  day = readPort(0x71);
  writePort(0x70, MONTH);
  month = readPort(0x71);
  writePort(0x70, YEAR);
  year = readPort(0x71);

  recalculate(&hours, &day, &month, &year);

  switch(value) {
    case SECONDS: return seconds;
    case MINUTES: return minutes;
    case HOURS: return hours;
    case DAY: return day;
    case MONTH: return month;
    case YEAR: return year;
  }
  return hours;
}

void recalculate(int * hours, int * day, int * month, int * year) {
  *hours += timeZone;

  if(*hours < 0) {
    *hours += 24;
    (*day)--;
    if(*day < 0) {
      (*month)--;
      *day = days(*month);
    }
    if(*month < 0) {
      *month = 12;
      (*year)--;
    }
  }
  else if(*hours > 24){
    *hours = *hours % 24;
    (*day)++;
    if(*day > days(*month)){
      (*month)++;
      *day = 1;
    }
    if(*month > 12) {
      *month = 1;
      (*year)++;
    }
  }
}

uint8_t days(int month) {
  return daysPerMonth[month - 1];
}

void setTimeZone(int tz) {
  if(tz >= -12 && tz <=14)
    timeZone = tz;
}

void setupRTC() {
  uint8_t status;
  writePort(0x70, 0x0B);
  status = readPort(0x71);
  status |= 6;
  writePort(0x71, status);
}

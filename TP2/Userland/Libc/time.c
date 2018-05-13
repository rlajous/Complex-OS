#include <time.h>

void updateTime(time* currentTime) {
  int80(4, (uint64_t)&currentTime->hours, (uint64_t)&currentTime->minutes, (uint64_t)&currentTime->seconds, 0);
}

void updateDate(date* currentDate) {
  int80(5, (uint64_t)&currentDate->day, (uint64_t)&currentDate->month, (uint64_t)&currentDate->year, 0);
}

int getHour() {
  time currentTime;
  updateTime(&currentTime);
  return currentTime.hours;
}

int getMinutes() {
  time currentTime;
  updateTime(&currentTime);
  return currentTime.minutes;
}

int getSeconds() {
  time currentTime;
  updateTime(&currentTime);
  return currentTime.seconds;
}

int getDay() {
  date currentDate;
  updateDate(&currentDate);
  return currentDate.day;
}

int getMonth() {
  date currentDate;
  updateDate(&currentDate);
  return currentDate.month;
}

int getYear() {
  date currentDate;
  updateDate(&currentDate);
  return currentDate.year;
}

void setTimeZone(int tz) {
  int80(3, tz, 0, 0, 0);
}

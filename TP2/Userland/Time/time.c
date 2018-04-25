#include <stdint.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
  printf("%.2d:%.2d:%.2d\n", getHour(), getMinutes(), getSeconds());
  return 0;
}

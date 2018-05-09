#include "stdio.h"
#include <stdint.h>

int main(int argc, char *argv[]) {
  char buffer[500];
  int80(9, (uint64_t)buffer, 500, 0);
  printf("%s\n", buffer);
  return 0;
}

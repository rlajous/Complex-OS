#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
  char buffer[1024];
  int80(9, (uint64_t)buffer, 1024, 0, 0);
  printf("%s", buffer);
  return 0;
}

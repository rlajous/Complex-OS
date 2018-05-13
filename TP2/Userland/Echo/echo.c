#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
  for(int i = 1; i < argc; i++)
    printf("%s ", argv[i]);
  putchar('\n');
  return 0;
}

#include "stdio.h"
#include <stdint.h>

int main(int argc, char *argv[]) {
  printf("Ave ");
  for(int i=0; i < argc; i++)
    printf("%s ", argv[i]);
  putchar('\n');
  return 0;
}

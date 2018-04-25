#include <stdint.h>

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

int main(int argc, char *argv[]) {
  int80(2,0,0,0);
  return 0;
}

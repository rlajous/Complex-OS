#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

int execv(char *filename, int argc, char *argv[]);
int parseArguments(char * string);

char * arguments[100];

int main(int argc, char *argv[]) {
  char buffer[128];
  while(1) {
    printf("$> ");
    scanf("%s", buffer);
    if(execv(buffer,parseArguments(buffer),arguments) == -1)
      printf("%s: Invalid Module\n", buffer);
  }

  return 0;
}

int execv(char *filename, int argc, char *argv[]) {
	return int80(7, (uint64_t)filename, argc, (uint64_t)argv);
}

int parseArguments(char * string) {
  int argc = 0;
  char * previous;
  char c;
  while((c=*string) != ' ' && c != '\0' )
    string++;
  *string = '\0';
  previous = ++string;

  if(c != '\0') {
    while((c=*string) != '\0') {
      if(c == ' ') {
        arguments[argc++] = previous;
        previous = string + 1;
        *string = '\0';
      }
      string++;
    }
    arguments[argc++] = previous;
  }
  return argc;
}

#include <stdint.h>
#include <stdio.h>
#include <process.h>
#include <ipc.h>

int executeCommand(char * name, int argc, char *argv[]);
int parseArguments(char * string);

char * arguments[100];

int main(int argc, char *argv[]) {
  char buffer[128];
  while(1) {
    printf("$> ");
    scanf("%s", buffer);
    if(executeCommand(buffer,parseArguments(buffer),arguments) == -1)
      printf("%s: Invalid Module\n", buffer);
  }

  return 0;
}

int executeCommand(char * name, int argc, char *argv[]) {
  char* string = name;
  while(*string != '\0') {
    if(*string == '|') {
      *string = '\0';
      shellPipe(name, argc, argv, 1);

      return shellPipe(string + 1, parseArguments(string),arguments, 0);
    }
    string++;
  }
  if(*(string - 1) == '&') {
    *(string - 1) = '\0';
    return runModule(name, argc, argv, 1);
  }
	return runModule(name, argc, argv, 0);
}

int parseArguments(char * string) {
  int argc = 0;
  char * previous = string;
  char c;
  while((c=*string) != ' ' && c != '\0')
    string++;
  *string = '\0';
  arguments[argc++] = previous;
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

#include <stdint.h>
#include <stdio.h>

static const char * man =
"\t time: Prints out the current time\n\
\t date: Prints out the current date\n\
\t clear: Wipes out the screen\n\
\t ps: Prints processes\n\
\t prodcons: Shows producer-consumer problem\n\
\t pipeProdcons: Shows producer-consumer problem solved with pipes\n\
\t messageTest: Tests interprocess communication\n\
\t threadTest&: Tests thread addition (Afterwards run ps)\n\
\t kill: Kills pid sent as argument\n\
\t snek: Snake game\n\
\t time|shellPipe: Tests shell pipe, second process will print first output\n\
\t help: Prints all commands and uses\n\
\t cmd&: Executes command in background\n\
\t echo: Prints what you send as arguments\n\
\t sampleCodeModule: A sample module";

int main(int argc, char *argv[]) {
  printf("%s\n", man);
  return 0;
}

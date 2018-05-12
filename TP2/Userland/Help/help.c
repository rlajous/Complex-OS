#include <stdint.h>
#include <stdio.h>

static const char * man =
"time: Prints out the current time\n\
date: Prints out the current date\n\
clear: Wipes out the screen\n\
ps: Prints processes\n\
prodcons: Shows producer-consumer problem\n\
messageTest: Tests interprocess communication\n\
kill: kills pid sent as argument\n\
cmd&: Executes command in background\n\
echo: Prints what you send as arguments\n\
sampleCodeModule: A sample module";

int main(int argc, char *argv[]) {
  printf("%s\n", man);
  return 0;
}

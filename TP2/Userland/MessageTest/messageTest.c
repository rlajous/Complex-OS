#include <stdio.h>
#include <ipc.h>
#include <process.h>

int otherProcess(int argc, char * argv[]);

int main(int argc, char *argv[]) {
  char name[20];
  char num[11];
  char * arguments[] = {name, num};
  int recipientPid;

  strncpy(name, "messageTestOther", 17);
  itoa(getpid(), num, 10);
  recipientPid = runProcess(&otherProcess, 2, arguments);

  createChannel(recipientPid);

  send(recipientPid, "KILL ME", 7);

  return 0;
}

int otherProcess(int argc, char *argv[]) {
  int senderPid;
  char buffer[20];
  buffer[0] = 'a';
  buffer[1] = '\0';

  if(argc > 0) {
    parseInt(argv[1],&senderPid);
  }
  else return -1;

  createChannel(senderPid);

  receive(senderPid, buffer, 8);

  printf("Process pid: %d received: %s from process %d\n", getpid(), buffer, senderPid);

  return 0;
}

#include <stdio.h>
#include <ipc.h>
#include <process.h>

int otherProcess(int argc, char * argv[]);

int main(int argc, char *argv[]) {
  char buffer[50];
  int length;
  char name[20];
  char sender[11];
  char len[2];
  char * arguments[] = {name, sender, len};
  int recipientPid;

  strncpy(name, "messageTestOther", 17);
  itoa(getpid(), sender, 10);

  printf("Type message: ");

  scanf("%s", buffer);

  length = strlen(buffer);

  itoa(length, len, 10);

  recipientPid = runProcess(&otherProcess, 3, arguments);

  createChannel(recipientPid);

  send(recipientPid, buffer, length);

  return 0;
}

int otherProcess(int argc, char *argv[]) {
  int senderPid;
  int length;
  char buffer[50];

  if(argc > 0) {
    parseInt(argv[1],&senderPid);
    parseInt(argv[2], &length);
  }
  else return -1;

  createChannel(senderPid);

  receive(senderPid, buffer, length);

  printf("Process pid: %d\nReceived: %s\nFrom process pid %d\n", getpid(), buffer, senderPid);

  return 0;
}

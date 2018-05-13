#include <timer.h>

static sleepingProcess_t sleepingProcesses[MAX_SLEEPING_PROCESSES];
static int firstAvailableSpace = 0;
static int sleeping = 0;
static int first = NOT_USED;

void initializeTimer() {
  int i;
  for(i = 0; i < MAX_SLEEPING_PROCESSES; i++) {
    sleepingProcesses[i].pid = NOT_USED;
  }
}

void sleep(uint64_t milliseconds, int pid) {
  uint64_t ticks = milliseconds/55; //ticks every 55ms
  if(ticks > 0) {
    addSleepingProcess(pid, ticks);
    changeProcessState(pid, SLEEPING);
    yield();
  }
}

void decrementTicks() {
  int i;
  int current = first;

  for(i = 0; i < sleeping; i++) {
    sleepingProcesses[current].tickQuantity--;
    if(sleepingProcesses[current].tickQuantity == 0) {
      removeSleepingProcess(sleepingProcesses[current].pid);
    }
    current = sleepingProcesses[current].next;
  }
}

int removeSleepingProcess(int pid) {
  int current = first;
  int previous = current;
  int i;

  for(i = 0; i < sleeping; i++) {
    if(sleepingProcesses[current].pid == pid) {
      changeProcessState(pid, READY);
      sleepingProcesses[current].pid = NOT_USED;
      sleepingProcesses[previous].next = sleepingProcesses[current].next;
      sleeping--;
      firstAvailableSpace = firstAvailableSpace > current ? current : firstAvailableSpace;
      return 1;
    } else {
      previous = current;
      current = sleepingProcesses[current].next;
    }
  }
  return SLEEPING_PROCESS_NOT_FOUND;
}

int addSleepingProcess(int pid, uint64_t tickQuantity) {
  if(sleeping < MAX_SLEEPING_PROCESSES) {
    sleepingProcesses[firstAvailableSpace].pid = pid;
    sleepingProcesses[firstAvailableSpace].tickQuantity = tickQuantity;
    sleepingProcesses[firstAvailableSpace].next = first;
    first = firstAvailableSpace;
    firstAvailableSpace = getNextAvailableSpace();
    sleeping++;
    return 1;
  }
  return SLEEPING_PROCESS_LIMIT_REACHED;
}

int getNextAvailableSpace() {
  int i;
  for(i = 0; i < MAX_SLEEPING_PROCESSES; i++) {
    if(sleepingProcesses[i].pid == NOT_USED)
      return i;
  }
  return SLEEPING_PROCESS_LIMIT_REACHED;
}

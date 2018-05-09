#include <scheduler.h>
#include <process.h>

static roundRobinNode_t processes[MAX_PROCESSES];
static int firstAvailableSpace = 0;
static int current = 0;
static int processQuantity = 0;
static int quantum = QUANTUM;
static void * schedulerAuxiliaryStack;


void initializeScheduler() {
  int i;
  schedulerAuxiliaryStack = allocatePages(1);
  for(i = 0; i < MAX_PROCESSES; i++) {
    processes[i].process = NULL;
  }
}

void * nextProcess() {
  int next = current;
  int first = next;

  if(processes[current].process != NULL)
    processes[current].process->state = READY;

  do {
    next = processes[current].next;
  } while(processes[next].process->state != READY && first != next);

  current = next;
  copyModule(processes[current].process->entryPoint);
  quantum = QUANTUM;
  processes[current].process->state = RUNNING;
  return getCurrentStack();
}

void * schedule() {
	if(quantum-- < 0){
    return nextProcess();
	}
	return getCurrentStack();
}

int getpid() {
  return processQuantity == 0? -1 : processes[current].process->pid;
}

void * getCurrentStack() {
  return processes[current].process->stack;
}

void addProcess(process_t* process) {
  int next;

  if(processQuantity != MAX_PROCESSES) {
    if (processQuantity == 0) {
      current = 0;
      processes[current].next = current;
      processes[current].process = process;
    } else {
      next = processes[current].next;
      processes[current].next = firstAvailableSpace;
      processes[firstAvailableSpace].process = process;
      processes[firstAvailableSpace].next = next;
    }
    firstAvailableSpace = findFirstAvailableSpace();
    processQuantity++;
  }
}

void removeProcess(process_t *process) {
  int previous = current;
  int toRemove = processes[current].next;
  int first = toRemove;

  if(processQuantity == 0 || process == NULL)
    return;

  if(previous == toRemove && processes[current].process->pid == process->pid) {
    deleteProcess(processes[current].process);
    processes[current].process = NULL;
    current = 0;
    processQuantity--;
    return;
  }

  while(processes[toRemove].process->pid != process->pid) {
    previous = toRemove;
    toRemove = processes[toRemove].next;
    if(toRemove == first)
      return;
  }

  processes[previous].next = processes[toRemove].next;
  deleteProcess(processes[toRemove].process);
  processes[toRemove].process = NULL;
  processQuantity--;
  firstAvailableSpace = firstAvailableSpace > toRemove ? toRemove : firstAvailableSpace;

  if(current == toRemove) {
    yield();
  }
}

int findFirstAvailableSpace(){
	int i;
	for(i = 0; i < MAX_PROCESSES; i++) {
	  if(processes[i].process == NULL)
	    return i;
	}
	return PROCESS_LIMIT_REACHED;
}

void * switchToKernelStack(void * rsp) {
  processes[current].process->stack = rsp;
  return (schedulerAuxiliaryStack + PAGE_SIZE - 1);
}

void * getEntryPoint() {
  return processes[current].process->entryPoint;
}

void killCurrent() {
  killProcess(processes[current].process->pid);
}

void killProcess(int pid) {
  int index = getProcessIndex(pid);

  if(index != PID_NOT_FOUND) {
    removeProcess(processes[index].process);
    yield();
  }
}

int getProcessIndex(int pid) {
  int i = 0;
  int next = current;

  while(i < processQuantity) {
    if(processes[next].process->pid == pid)
      return next;

    next = processes[next].next;
    i++;
  }
  return PID_NOT_FOUND;
}

void blockProcess(int pid) {
  int index = getProcessIndex(pid);

  if(index != PID_NOT_FOUND) {
    processes[index].process->state = BLOCKED;
    yield();
  }
}

void unblockProcess(int pid) {
  int index = getProcessIndex(pid);

  if(index != PID_NOT_FOUND)
    processes[index].process->state = READY;
}

void getProcesses(char * buffer, int size) {
  int i,j = 0;
  int length;
  int k = current;

  for(i = 0; i < processQuantity; i++) {
      if(size > 0) {
        memcpy(buffer + j, "pid: ", 5);
        j += 5;
        size -= 5;
      }

      if(size > 0) {
        length = uintToBase(processes[k].process->pid, buffer + j, 10);
        j += length;
        size -= length;
      }

      if(size > 0) {
        memcpy(buffer + j, ", Name: ", 8);
        j += 8;
        size -= 8;
      }

      if(size > 0) {
        length = strlen(processes[k].process->name);
        memcpy(buffer + j, processes[k].process->name, length);
        j += length;
        size -= length;
      }

      if(size > 0) {
        memcpy(buffer + j, ", Status: ", 10);
        j += 10;
        size -= 10;
      }

      if(size > 0) {
        char * state;
        int length;

        if(processes[k].process->state == RUNNING)
          state = "Running\n";
        else if(processes[k].process->state == READY)
          state = "Ready\n";
        else
          state = "Blocked\n";

        length = strlen(state);
        memcpy(buffer + j, state, length);
        j += length;
        size -= length;
      }
      k = processes[k].next;
  }
  if(j < size)
    buffer[j] = '\0';
  else
    buffer[size - 1] = '\0';
}

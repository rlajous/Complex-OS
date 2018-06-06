#include <scheduler.h>
#include <process.h>
#include <terminal.h>
#include <semaphore.h>
#include <types.h>

static roundRobinNode_t processes[MAX_PROCESSES];
static int firstAvailableSpace = 0;
static process_t * foreground = NULL;
static int current = 0;
static int processQuantity = 0;
static int quantum = QUANTUM;
static void * schedulerAuxiliaryStack;
static int changeLock = 0;
static int lockQuantumExceded = 0;


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
  int currentThread;

  if(changeLock == 1) {
    lockQuantumExceded = 1;
    return getCurrentStack();
  }

  if(processes[current].process != NULL) {
    currentThread = processes[current].process->currentThread;
    thread_t * thread = processes[current].process->threads[currentThread].thread;
    if(thread->state == RUNNING) {
      thread->state = READY;
    }
  }

  do {
    next = processes[next].next;
  } while(nextThread(processes[next].process) == -1 && first != next);

  current = next;
  quantum = QUANTUM;
  currentThread = processes[current].process->currentThread;
  processes[current].process->threads[currentThread].thread->state = RUNNING;
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

int getCurrentThread() {
  return processQuantity == 0? -1 : processes[current].process->currentThread;
}

void * getCurrentStack() {
  int currentThread = processes[current].process->currentThread;
  return processes[current].process->threads[currentThread].thread->stack;
}

int addProcess(process_t* process) {
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
    return process->pid;
  }
  return -1;
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
  int currentThread = processes[current].process->currentThread;
  processes[current].process->threads[currentThread].thread->stack = rsp;
  return (schedulerAuxiliaryStack + PAGE_SIZE - 1);
}

void * getEntryPoint() {
  int currentThread = processes[current].process->currentThread;
  return processes[current].process->threads[currentThread].thread->entryPoint;
}

void killForeground() {
  if(foreground != NULL)
    killProcess(foreground->pid);
  yield();
}

void killCurrent() {
  killProcess(processes[current].process->pid);
  yield();
}

void killThread(int pid, int thread) {
  process_t * process = getProcess(pid);

  if(process != NULL) {
    removeThreadFromMutexes(pid, thread);
    removeThreadFromSemaphores(pid,thread);
    terminateThread(process, thread);
  }
}

void killProcess(int pid) {
  int index = getProcessIndex(pid);

  if(index != PID_NOT_FOUND) {
    changeChildrenToOrphan(pid);
    if(foreground->pid == pid) {
      resetBuffer();
      setForeground(foreground->ppid);
      changeThreadState(foreground->pid, foreground->currentThread, READY);
    }
    removePidFromMutexes(pid, processes[index].process->currentThread);
    removePidFromSemaphores(pid,processes[index].process->currentThread);
    removeProcess(processes[index].process);
  }
}

void changeChildrenToOrphan(int pid) {
  int i;
  int index = getProcessIndex(pid);
  int next = current;

  for (i= 0; i < processQuantity; i++) {
    if(processes[next].process->ppid == pid)
      processes[next].process->ppid = processes[index].process->ppid;
    next = processes[next].next;
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

process_t * getProcess(int pid) {
  int index = getProcessIndex(pid);
  process_t * process = NULL;

  if(index != PID_NOT_FOUND)
    process = processes[index].process;

  return process;
}

void getProcesses(char * buffer, int size) {
  int i,j, thread = 0;
  int length;
  int k = current;

  for(i = 0; i < processQuantity; i++) {
    for(thread = 0; thread < MAX_THREADS; thread++) {
      if (processes[k].process->threads[thread].thread != NULL) {
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

        if (size > 0) {
          char *state;
          int length;

          switch (processes[k].process->threads[thread].thread->state) {
            case RUNNING:
              state = "Running";
              break;

            case READY:
              state = "Ready";
              break;

            case BLOCKED:
              state = "Blocked";
              break;

            case SLEEPING:
              state = "Sleeping";
              break;
          }

          length = strlen(state);
          memcpy(buffer + j, state, length);
          j += length;
          size -= length;
        }

        if (size > 0) {
          memcpy(buffer + j, ", Stack: 0x", 11);
          j += 11;
          size -= 11;
        }

        if (size > 0) {
          length = uintToBase((uint64_t) processes[k].process->threads[thread].thread->memoryBase, buffer + j, 16);
          j += length;
          size -= length;
        }

        if (size > 0) {
          memcpy(buffer + j, ", ThreadId: ", 12);
          j += 12;
          size -= 12;
        }

        if (size > 0) {
          length = uintToBase((uint64_t) thread, buffer + j, 10);
          j += length;
          size -= length;
        }

        if (size > 0) {
          char *status;

          if (processes[k].process == foreground)
            status = ", Fg\n";
          else
            status = ", Bg\n";

          length = strlen(status);
          memcpy(buffer + j, status, length);
          j += length;
          size -= length;
        }
      }
    }
    k = processes[k].next;
  }

  if(j < size)
    buffer[j] = '\0';
  else
    buffer[size - 1] = '\0';
}

void setForeground(int pid) {
  foreground = processes[getProcessIndex(pid)].process;
}

int getForeground() {
  return foreground->pid;
}

int isForeground(int pid) {
  return foreground->pid == pid;
}

void lock() {
  changeLock = 1;
}

void unlock() {
  changeLock = 0;
  if(testAndSetLock(&lockQuantumExceded)) {
    yield();
  }
}

void blockRead() {
  int currentThread = foreground->currentThread;
  if(foreground->threads[currentThread].thread->state != BLOCKED) {
    foreground->threads[currentThread].thread->state = BLOCKED;
    yield();
  }
}

void unblockRead() {
  int currentThread = foreground->currentThread;
  if(foreground->threads[currentThread].thread->state == BLOCKED) {
    foreground->threads[currentThread].thread->state = READY;
  }
}

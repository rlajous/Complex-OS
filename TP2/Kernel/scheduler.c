#include <scheduler.h>
#include <process.h>

static roundRobinNode_t processes[MAX_PROCESSES];
static int firstAvailableSpace = FIRST_PROCESS;
static int current = FIRST_PROCESS;
static int quantum = QUANTUM;


void initializeScheduler() {
  int i;
  for(i = 0; i < MAX_PROCESSES; i++) {
    processes[i].process = NULL;
  }
}

void * schedule(void * rsp) {
  void * nextRsp = rsp;
  quantum--;
	if(quantum < 0){
	  /*processes[current].process->stack = rsp;
    current = processes[current].next;
		nextRsp = processes[current].process->stack;*/
	  quantum = QUANTUM;
	}
	return nextRsp;
}

int getPid() {
  return current == FIRST_PROCESS? -1 : processes[current].process->pid;
}

void addProcess(process_t* process) {
  int next;

  if(firstAvailableSpace != PROCESS_LIMIT_REACHED) {
    if (current == FIRST_PROCESS) {
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
  }
}

void removeProcess(process_t *process) {
  int previous = current;
  int toRemove = processes[current].next;
  int first = toRemove;

  if(current == FIRST_PROCESS || process == NULL)
    return;

  if(previous == toRemove && processes[current].process->pid == process->pid) {
    deleteProcess(processes[current].process);
    processes[current].process = NULL;
    current = FIRST_PROCESS;
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
  firstAvailableSpace = firstAvailableSpace > toRemove ? toRemove : firstAvailableSpace;
}

int findFirstAvailableSpace(){
	int i;
	for(i = 0; i < MAX_PROCESSES; i++) {
	  if(processes[i].process == NULL)
	    return i;
	}
	return PROCESS_LIMIT_REACHED;
}
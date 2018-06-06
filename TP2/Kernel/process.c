#include <process.h>
#include <scheduler.h>
#include <threads.h>

static int nextPid = 1;

process_t * createProcess(void * entryPoint, int argc, char * argv[], char * name) {
  size_t length = strlen(name);
  process_t * process = allocateMemory(sizeof(process_t));

  process->pid = nextPid++;
  process->ppid = getpid();
  process->standardPipes[0] = -1;
  process->standardPipes[1] = -1;

  initializeThreads(process);
  process->currentThread = 0;

  process->threads[0].thread = createThread(argc, argv, entryPoint);
  process->threads[0].next = 0;

  if(length < MAX_NAME) {
    memcpy(process->name, name, length);
    process->name[length] = '\0';
  } else
    process->name[0] = 0;

  return process;
}

void initializeThreads(process_t * process) {
  int i;

  for(i = 0 ; i < MAX_THREADS ; i++){
    process->threads[i].thread = NULL;
  }
}

void deleteProcess(process_t * process) {
  killThreads(process);
  freeMemory(process);
}

void callProcess(int argc, char * argv[], void * entryPoint) {
  ((EntryPoint)(entryPoint))(argc, argv);
  if(argv != NULL) {
    while(argc--) {
      freeMemory(argv[argc]);
    }
    freeMemory(argv);
  }
  process_t * process = getProcess(getpid());
  terminateThread(process, process->currentThread);
  yield();
}

//Source RowDaBoat Wyrm
void * fillStackFrame(void * entryPoint, void * stack, int argc, char * argv[]) {
  stackFrame_t * frame = (stackFrame_t *)stack - 1;
  frame->gs =		0x001;
  frame->fs =		0x002;
  frame->r15 =	0x003;
  frame->r14 =	0x004;
  frame->r13 =	0x005;
  frame->r12 =	0x006;
  frame->r11 =	0x007;
  frame->r10 =	0x008;
  frame->r9 =		0x009;
  frame->r8 =		0x00A;
  frame->rsi =	(uint64_t)argv;
  frame->rdi =	(uint64_t)argc;
  frame->rbp =	0x00D;
  frame->rdx = (uint64_t)entryPoint;
  frame->rcx =	0x00F;
  frame->rbx =	0x010;
  frame->rax =	0x011;
  frame->rip = (uint64_t)&callProcess;
  frame->cs =		0x008;
  frame->eflags = 0x202;
  frame->rsp =	(uint64_t)&(frame->base);
  frame->ss = 	0x000;
  frame->base =	0x000;

  return frame;
}

int addThread(process_t * process, thread_t * thread) {
  int next, current;
  int index = firstAvailableThreadSpace(process);

  if(index != THREAD_LIMIT_REACHED) {
    current = process->currentThread;
    process->threads[index].thread = thread;
    next = process->threads[current].next;
    process->threads[current].next = index;
    process->threads[index].next = next;

    return index;
  }
  return -1;
}

int firstAvailableThreadSpace(process_t * process) {
  int i;
  for(i = 0; i < MAX_THREADS; i++) {
    if(process->threads[i].thread == NULL)
      return i;
  }
  return THREAD_LIMIT_REACHED;
}

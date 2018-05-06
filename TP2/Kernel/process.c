#include <process.h>
#include <scheduler.h>

static int nextPid = 1;

process_t * createProcess(void * entryPoint, int argc, char * argv[], char * name) {
  int length = strlen(name);
  void * aux = allocatePages(PROCESS_INITIAL_PAGES);
  process_t * process = (process_t *)(aux + PROCESS_INITIAL_PAGES * PAGE_SIZE - 1 - sizeof(process));

  process->entryPoint = entryPoint;
  process->pid = nextPid++;
  process->state = READY;
  process->stack = (void *) (process);
  process->stack = fillStackFrame(entryPoint, process->stack, argc, argv);
  process->memoryBase = aux;
  if(length < MAX_NAME)
    memcpy(process->name, name, length);
  else
    process->name[0] = 0;

  return process;
}

void deleteProcess(process_t * process) {
  freeMemory(process->memoryBase);
}

void callProcess(int argc, char * argv[], void * entryPoint) {
  ((EntryPoint)(entryPoint))(argc, argv);
  if(argv != NULL)
    freeMemory(argv);
  killCurrent();
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

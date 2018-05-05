#include <process.h>

static int nextPid = 1;

process_t * createProcess(void * entryPoint) {
  void * aux = allocatePages(PROCESS_INITIAL_PAGES);
  process_t * process = (process_t *)(aux + PROCESS_INITIAL_PAGES * PAGE_SIZE - 1);

  process->entryPoint = entryPoint;
  process->pid = nextPid++;
  process->state = READY;
  process->stack = (void *) (process - sizeof(process_t));
  process->stack = fillStackFrame(entryPoint, process->stack);
  process->memoryBase = aux;

  return process;
}

void deleteProcess(process_t * process) {
  freeMemory(process->memoryBase);
}

//Source RowDaBoat Wyrm
void * fillStackFrame(void * entryPoint, void * stack) {
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
  frame->rsi =	0x00B;
  frame->rdi =	0x00C;
  frame->rbp =	0x00D;
  frame->rdx =	0x00E;
  frame->rcx =	0x00F;
  frame->rbx =	0x010;
  frame->rax =	0x011;
  frame->rip =	(uint64_t)entryPoint;
  frame->cs =		0x008;
  frame->eflags = 0x202;
  frame->rsp =	(uint64_t)&(frame->base);
  frame->ss = 	0x000;
  frame->base =	0x000;

  return frame;
}

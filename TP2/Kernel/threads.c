#include <threads.h>
#include <process.h>
#include <scheduler.h>
#include <types.h>

thread_t * createThread(int argc, char * argv[], void * entryPoint) {
  void * aux = allocatePages(THREAD_INITIAL_PAGES);
  thread_t * thread = (thread_t *)(aux + THREAD_INITIAL_PAGES * PAGE_SIZE - 1 - sizeof(thread_t));

  thread->entryPoint = entryPoint;
  thread->state = READY;
  thread->stack = (void *) (thread);
  thread->stack = fillStackFrame(entryPoint, thread->stack, argc, argv);
  thread->memoryBase = aux;
  thread->waitingThread = NOT_WAITING;

  return thread;
}

void terminateThread(process_t * process, int thread) {

  int previous = process->currentThread;
  int toRemove = process->threads[previous].next;
  int first = toRemove;

  if(thread == 0) {
    killThreads(process);
    killCurrent();
    return;
  }

  while(toRemove != thread) {
    previous = toRemove;
    toRemove = process->threads[toRemove].next;
    if(toRemove == first)
      return;
  }
  process->threads[previous].next = process->threads[thread].next;
  deleteThread(process->threads[thread].thread);
  process->threads[thread].thread = NULL;
  unlockWaitingThreads(process, thread);
}

void deleteThread(thread_t * thread) {
  freeMemory(thread->memoryBase);
}

void killThreads(process_t * process) {
  int i;

  for (i = 0; i < MAX_THREADS; i++) {
    if(process->threads[i].thread != NULL)
      deleteThread(process->threads[i].thread);
  }
}

int nextThread(process_t * process) {
  int next = process->currentThread;
  int first = next;

  do {
    next = process->threads[next].next;
  } while(process->threads[next].thread->state != READY && first != next);

  if(first == next && process->threads[next].thread->state != READY)
    return -1;

  process->currentThread = next;
  return next;
}

void unlockWaitingThreads(process_t * process, int thread) {
  int i;
  for (i = 0; i < MAX_THREADS; i++) {
    if(process->threads[i].thread != NULL && process->threads[i].thread->state == SLEEPING)
      if(process->threads[i].thread->waitingThread == thread) {
        process->threads[i].thread->waitingThread = NOT_WAITING;
        changeThreadState(process->pid, i, READY);
      }
  }
}

void joinThread(process_t * process, int thread, int toJoin) {
  if(process != NULL && process->threads[toJoin].thread != NULL) {
    process->threads[thread].thread->waitingThread = toJoin;
    process->threads[thread].thread->state = SLEEPING;
    yield();
  }
}

void changeThreadState(int pid, int thread, processState state) {
  process_t * process = getProcess(pid);

  if(process != NULL && thread >= 0 && thread < MAX_THREADS) {
    process->threads[thread].thread->state = state;
  }
}

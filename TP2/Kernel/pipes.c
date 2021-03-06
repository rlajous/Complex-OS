#include <pipes.h>
#include <types.h>

static pipe_t pipes[MAX_PIPES];

int createPipe(char * pipeName) {
  int pid = getpid();
  int thread = getCurrentThread();
  int  i;
  char semWriteName[15] = "PIPE_WRITE_";
  char semReadName[15] = "PIPE_READ_";
  char mutexName[15] = "PIPE_MUTEX_";

  if(pipeName == NULL) {
    return -1;
  }

  for(i = 0; i < MAX_PIPES; i++) {
      if(pipes[i].used == FALSE) {
        pipes[i].creator.pid = pid;
        pipes[i].creator.thread = thread;
        size_t nameLength = strlen(pipeName);
        memcpy(pipes[i].name, pipeName, nameLength);
        pipes[i].readIndex = 0;
        pipes[i].writeIndex = 0;
        pipes[i].used = TRUE;

        uintToBase(i, semWriteName + strlen(semWriteName), 10);
        pipes[i].semWrite = createSemaphore(pid, semWriteName, thread);
        setValue(pipes[i].semWrite, PIPE_BUFFER_LENGTH);

        uintToBase(i, semReadName + strlen(semReadName), 10);
        pipes[i].semRead = createSemaphore(pid, semReadName, thread);

        pipes[i].buffer = allocatePages(PIPE_PAGES);

        uintToBase(i, mutexName + strlen(mutexName), 10);
        pipes[i].mutex = getMutex(mutexName, pid, thread);

        return i;
      }
  }

  return -1;
}

void initializePipes() {
  int i;

  for(i = 0; i < MAX_PIPES; i++) {
    pipe_t * p = &pipes[i];

    p->readIndex = 0;
    p->writeIndex = 0;
    p->creator.pid = 0;
    p->creator.thread = 0;
    p->used = FALSE;
  }
}

int getPipe(char * pipeName) {
  int i;

  for(i = 0; i < MAX_PIPES; i++) {
    if(strcmp(pipes[i].name, pipeName) == 0 && pipes[i].used == TRUE){
      return i + 3;
    }
  }

  return createPipe(pipeName) + 3;
}

int writePipe(int pipe, char * message, int bytes, int pid, int thread) {
  pipe = pipe - 3;
  pipe_t * p;
  int i = 0;

  if(!isValidPipe(pipe)) {
    return -1;
  }
  p = &pipes[pipe];

  while(bytes != 0) {
    wait(p->semWrite, pid, thread);
    mutexDown(p->mutex, pid, thread);
    p->buffer[p->writeIndex] = message[i++];
    bytes--;
    if(p->writeIndex == PIPE_BUFFER_LENGTH) {
      p->writeIndex = 0;
    } else {
      p->writeIndex++;
    }

    mutexUp(p->mutex, pid, thread);
    signal(p->semRead);
  }
  return 0;
}

int readPipe(int pipe, char * buffer, int bytes, int pid, int thread) {
  pipe = pipe - 3;

  pipe_t * p;
  int i = 0;

  if(!isValidPipe(pipe)) {
      return -1;
  }
  p = &pipes[pipe];

  while(bytes != 0) {
    wait(p->semRead, pid, thread);
    mutexDown(p->mutex, pid, thread);

    buffer[i++] = p->buffer[p->readIndex];

    bytes--;
    if(p->readIndex == PAGE_SIZE * PIPE_PAGES) {
        p->readIndex = 0;
    }
    else {
        p->readIndex++;
    }

    mutexUp(p->mutex, pid, thread);
    signal(p->semWrite);
  }
  return 0;
}

int deletePipe(int pipe, int pid, int thread) {

  pipe_t * p;

  if(!isValidPipe(pipe)) {
    return -1;
  }

  p = &pipes[pipe];

  if(p->creator.pid == pid && p->creator.thread == thread){
    freeMemory(p->buffer);
    releaseMutex(p->creator.pid, p->mutex, p->creator.thread);
    releaseSemaphore(p->creator.pid, p->semWrite, p->creator.thread);
    releaseSemaphore(p->creator.pid, p->semRead, p->creator.thread);
    p->used = FALSE;

    return 0;
  }

  return -1;
}

int isValidPipe(int pipe) {
  if(pipe < 0 || pipe >= MAX_PIPES)
    return 0;

  return pipes[pipe].used != FALSE;
}


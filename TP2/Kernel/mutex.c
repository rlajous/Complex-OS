#include <mutex.h>
#include <lib.h>

static mutex_t mutexes[MAX_MUTEXES];
static int firstAvailableMutex = 0;
static int usedMutexes = 0;

void setupMutexSystem() {
  int i;

  for(i = 0 ; i < MAX_MUTEXES ; i++){
    clearMutex(i);
  }
}

int getMutex(char * name) {
  int i;
  int processIndex;
  int pid = getpid();
  int mutex = MUTEX_NOT_USED;

  for(i = 0; i < MAX_MUTEXES; i++) {
    if(!strcmp(mutexes[i].name, name) && mutexes[i].creatorPid != MUTEX_NOT_USED) {
      mutex = i;
      processIndex = nextListIndexAvailable(mutex, mutexes[i].processes, MAX_PROCESSES);
      mutexes[mutex].processes[processIndex] = pid;
      return mutex;
    }
  }

  if(mutex == MUTEX_NOT_USED)
    mutex = createMutex(pid, name);

  return mutex;
}

int createMutex(int pid, char * name) {
  int mutex;
  int length = strlen(name);

  if(usedMutexes < MAX_MUTEXES && length < MAX_NAME) {
    mutex = firstAvailableMutex;
    memcpy(mutexes[mutex].name, name, length);
    mutexes[mutex].creatorPid = pid;
    usedMutexes++;
  } else
    mutex = MUTEX_CREATION_ERROR;

  return mutex;
}

int getNextMutexAvailable() {
  int i;

  for(i = 0; i < MAX_MUTEXES; i++) {
    if(mutexes[i].creatorPid == MUTEX_NOT_USED){
      return i;
    }
  }

  return FULL_LIST;
}

void clearMutex(int mutex) {
  if(mutex < MAX_MUTEXES) {
    mutexes[mutex].creatorPid = MUTEX_NOT_USED;
    clearMutexArrays(mutex);
    mutexes[mutex].name[0] = '\0';
    mutexes[mutex].value = 0;

    if(mutex < firstAvailableMutex)
      firstAvailableMutex = mutex;
    else
      firstAvailableMutex = getNextMutexAvailable();
  }
}

void clearMutexArrays(int mutex) {
  int i;

  for(i = 0; i < MAX_BLOCKED; i++) {
    mutexes[mutex].blocked[i] = PID_NOT_USED;
  }

  for(i = 0; i < MAX_PROCESSES; i++) {
    mutexes[mutex].processes[i] = PID_NOT_USED;
  }
}

int nextListIndexAvailable(int mutex, int * list, int length) {
  int i;

  for(i = 0; i < length; i++){
    if(list[i] == PID_NOT_USED){
      return i;
    }
  }

  return FULL_LIST;
}
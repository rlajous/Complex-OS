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

int getMutex(char * name, int pid) {
  int i;
  int mutex;

  for(i = 0; i < MAX_MUTEXES; i++) {
    if(!strcmp(mutexes[i].name, name) && mutexes[i].pid != NOT_USED) {
      return i;
    }
  }

  mutex = createMutex(pid, name);

  return mutex;
}

int createMutex(int pid, char * name) {
  int mutex;
  size_t length = strlen(name);

  if(usedMutexes < MAX_MUTEXES && length < MAX_NAME) {
    mutex = firstAvailableMutex;
    memcpy(mutexes[mutex].name, name, length);
    mutexes[mutex].pid = pid;
    usedMutexes++;
    firstAvailableMutex = getNextMutexAvailable();
  } else
    mutex = MUTEX_CREATION_ERROR;
  return mutex;
}


int releaseMutex(int pid, int mutex) {
  int released = 0;

  if(isValidMutex(mutex)) {
    if (pid == mutexes[mutex].pid) {
      mutexes[mutex].pid = NOT_USED;
      mutexes[mutex].lockPid = NOT_USED;
      while(mutexes[mutex].blockedQuantity > 0)
        unlockProcess(mutex);
      released = 1;
    }
  }
  return released;
}

int getNextMutexAvailable() {
  int i;

  for(i = 0; i < MAX_MUTEXES; i++) {
    if(mutexes[i].pid == NOT_USED){
      return i;
    }
  }

  return FULL_LIST;
}

void clearMutex(int mutex) {
  int i;

  if(mutex < MAX_MUTEXES) {
    mutexes[mutex].pid = NOT_USED;
    mutexes[mutex].lockPid = NOT_USED;
    mutexes[mutex].name[0] = '\0';
    mutexes[mutex].value = UNLOCKED;
    mutexes[mutex].blockedQuantity = 0;

    for(i = 0; i < MAX_BLOCKED; i++) {
      mutexes[mutex].blocked[i] = NOT_USED;
    }

    if(mutex < firstAvailableMutex)
      firstAvailableMutex = mutex;
    else
      firstAvailableMutex = getNextMutexAvailable();
  }
}

int nextListIndexAvailable(int * list, int length) {
  int i;

  for(i = 0; i < length; i++){
    if(list[i] == NOT_USED){
      return i;
    }
  }

  return FULL_LIST;
}

int addToBlocked(int mutex, int pid) {
  int index;

  index = nextListIndexAvailable(mutexes[mutex].blocked, MAX_BLOCKED);
  if(index != FULL_LIST) {
    mutexes[mutex].blocked[index] = pid;
    mutexes[mutex].blockedQuantity++;
  }

  return index;
}

int isValidMutex(int mutex) {
  if(mutex < 0 || mutex >= MAX_MUTEXES)
    return 0;

  return mutexes[mutex].pid != NOT_USED;
}

void mutexDown(int mutex, int pid) {
  int locked;
  int added;

  lock();

  if(isValidMutex(mutex)) {
    locked = testAndSetLock(&mutexes[mutex].value);

    if (locked == LOCKED) {
      added = addToBlocked(mutex, pid);
      if (added != FULL_LIST) {
        changeProcessState(pid, BLOCKED);
        unlock();
        yield();
      } else {
        killProcess(pid);
        unlock();
        yield();
      }
    } else {
      mutexes[mutex].lockPid = pid;
    }
  }

  unlock();
}

void mutexUp(int mutex, int pid) {

  int unlocked = 0;

  lock();

  if(isValidMutex(mutex) && mutexes[mutex].lockPid == pid) {
    unlocked = unlockProcess(mutex);
    if (unlocked == 0)
      mutexes[mutex].value = UNLOCKED;
  }
  unlock();
}

int unlockProcess(int mutex) {
  int i = 0;
  int unlocked = 0;

  if(mutexes[mutex].blockedQuantity == 0) {
    return 0;
  }

  while(i < MAX_BLOCKED && !unlocked) {
    if(mutexes[mutex].blocked[i] != NOT_USED) {
      mutexes[mutex].lockPid = mutexes[mutex].blocked[i];
      changeProcessState(mutexes[mutex].blocked[i], READY);
      mutexes[mutex].blocked[i] = NOT_USED;
      mutexes[mutex].blockedQuantity--;
      unlocked = 1;
    }
    i++;
  }

  return unlocked;
}

void removePidFromMutexes(int pid) {
  int i, j;

  for(i = 0 ; i < MAX_MUTEXES ; i++){
    if(mutexes[i].pid == pid) {
      releaseMutex(pid, i);
    }

    if(mutexes[i].lockPid == pid) {
      mutexUp(i, pid);
    }

    for(j = 0; j < MAX_BLOCKED; j++) {
      if (mutexes[i].blocked[j] == pid) {
        mutexes[i].blocked[j] = NOT_USED;
        mutexes[i].blockedQuantity--;
        changeProcessState(pid, READY);
      }
    }
  }
}
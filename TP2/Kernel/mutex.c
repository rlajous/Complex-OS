#include <mutex.h>
#include <lib.h>
#include <types.h>
#include <process.h>

static mutex_t mutexes[MAX_MUTEXES];
static int firstAvailableMutex = 1;
static int usedMutexes = 1;
static int adminMutex = 0;

void setupMutexSystem() {
  int i;

  for(i = 0 ; i < MAX_MUTEXES ; i++){
    clearMutex(i);
  }
  mutexes[adminMutex].creator.pid = 0;
}

int getMutex(char * name, int pid, int thread) {
  int i;
  int mutex;

  mutexDown(adminMutex,pid, thread);
  for(i = 0; i < MAX_MUTEXES; i++) {
    if(!strcmp(mutexes[i].name, name) && mutexes[i].creator.pid != NOT_USED) {
      return i;
    }
  }

  mutex = createMutex(pid, name, thread);

  mutexUp(adminMutex, pid, thread);
  return mutex;
}

int createMutex(int pid, char * name, int thread) {
  int mutex;
  size_t length = strlen(name);

  if(usedMutexes < MAX_MUTEXES && length < MAX_NAME) {
    mutex = firstAvailableMutex;
    memcpy(mutexes[mutex].name, name, length);
    mutexes[mutex].creator.pid = pid;
    mutexes[mutex].creator.thread = thread;
    usedMutexes++;
    firstAvailableMutex = getNextMutexAvailable();
  } else
    mutex = MUTEX_CREATION_ERROR;
  return mutex;
}


int releaseMutex(int pid, int mutex, int thread) {
  int released = 0;

  mutexDown(adminMutex, pid, thread);
  if(isValidMutex(mutex)) {
    if (pid == mutexes[mutex].creator.pid && thread == mutexes[mutex].creator.thread) {
      mutexes[mutex].creator.pid = NOT_USED;
      mutexes[mutex].lockProcess.pid = NOT_USED;
      while(mutexes[mutex].blockedQuantity > 0)
        unlockProcess(mutex);
      released = 1;
    }
  }
  mutexUp(adminMutex, pid, thread);
  return released;
}

int getNextMutexAvailable() {
  int i;

  for(i = 0; i < MAX_MUTEXES; i++) {
    if(mutexes[i].creator.pid == NOT_USED){
      return i;
    }
  }

  return FULL_LIST;
}

void clearMutex(int mutex) {
  int i;

  if(mutex < MAX_MUTEXES) {
    mutexes[mutex].creator.pid = NOT_USED;
    mutexes[mutex].lockProcess.pid = NOT_USED;
    mutexes[mutex].name[0] = '\0';
    mutexes[mutex].value = UNLOCKED;
    mutexes[mutex].blockedQuantity = 0;

    for(i = 0; i < MAX_BLOCKED; i++) {
      mutexes[mutex].blocked[i].pid = NOT_USED;
    }

    if(mutex < firstAvailableMutex)
      firstAvailableMutex = mutex;
    else
      firstAvailableMutex = getNextMutexAvailable();
  }
}

int nextListIndexAvailable(threadId_t * list, int length) {
  int i;

  for(i = 0; i < length; i++){
    if(list[i].pid == NOT_USED){
      return i;
    }
  }

  return FULL_LIST;
}

int addToBlocked(int mutex, int pid, int thread) {
  int index;

  index = nextListIndexAvailable(mutexes[mutex].blocked, MAX_BLOCKED);
  if(index != FULL_LIST) {
    mutexes[mutex].blocked[index].pid = pid;
    mutexes[mutex].blocked[index].thread = thread;
    mutexes[mutex].blockedQuantity++;
  }

  return index;
}

int isValidMutex(int mutex) {
  if(mutex < 0 || mutex >= MAX_MUTEXES)
    return 0;

  return mutexes[mutex].creator.pid != NOT_USED;
}

void mutexDown(int mutex, int pid, int thread) {
  int locked;
  int added;

  lock();

  if(isValidMutex(mutex)) {
    locked = testAndSetLock(&mutexes[mutex].value);

    if (locked == LOCKED) {
      added = addToBlocked(mutex, pid, thread);
      if (added != FULL_LIST) {

        changeThreadState(pid, thread, BLOCKED);
        unlock();
        yield();
      } else {
        killThread(pid, thread);
        unlock();
        yield();
      }
    } else {
      mutexes[mutex].lockProcess.pid = pid;
      mutexes[mutex].lockProcess.thread = thread;
    }
  }

  unlock();
}

void mutexUp(int mutex, int pid, int thread) {

  int unlocked = 0;

  lock();

  if(isValidMutex(mutex) && mutexes[mutex].lockProcess.pid == pid && mutexes[mutex].lockProcess.thread == thread) {
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
    if(mutexes[mutex].blocked[i].pid != NOT_USED) {
      mutexes[mutex].lockProcess.pid = mutexes[mutex].blocked[i].pid;
      mutexes[mutex].lockProcess.thread = mutexes[mutex].blocked[i].thread;
      changeThreadState(mutexes[mutex].blocked[i].pid, mutexes[mutex].blocked[i].thread, READY);
      mutexes[mutex].blocked[i].pid = NOT_USED;
      mutexes[mutex].blocked[i].thread = NOT_USED;
      mutexes[mutex].blockedQuantity--;
      unlocked = 1;
    }
    i++;
  }

  return unlocked;
}

void removePidFromMutexes(int pid, int thread) {
  int i, j;
  int blockedThread;

  mutexDown(adminMutex, pid, thread);

  for(i = 0 ; i < MAX_MUTEXES ; i++){
    if(mutexes[i].creator.pid == pid) {
      releaseMutex(pid, i, thread);
    }

    if(mutexes[i].lockProcess.pid == pid) {
      mutexUp(i, pid, mutexes[i].lockProcess.pid);
    }

    for(j = 0; j < MAX_BLOCKED; j++) {
      if (mutexes[i].blocked[j].pid == pid) {
        mutexes[i].blocked[j].pid = NOT_USED;
        blockedThread = mutexes[i].blocked[j].thread;
        mutexes[i].blocked[j].thread = NOT_USED;
        mutexes[i].blockedQuantity--;
        changeThreadState(pid, blockedThread, READY);
      }
    }
  }

  mutexUp(adminMutex, pid, thread);
}

void removeThreadFromMutexes(int pid, int thread) {
  int i, j;
  int blockedThread;

  mutexDown(adminMutex, pid, thread);

  for(i = 0 ; i < MAX_MUTEXES ; i++){
    if(mutexes[i].creator.pid == pid && mutexes[i].creator.thread == thread) {
      releaseMutex(pid, i, thread);
    }

    if(mutexes[i].lockProcess.pid == pid && mutexes[i].lockProcess.thread == thread) {
      mutexUp(i, pid, mutexes[i].lockProcess.pid);
    }

    for(j = 0; j < MAX_BLOCKED; j++) {
      if (mutexes[i].blocked[j].pid == pid && mutexes[i].blocked[j].thread == thread) {
        mutexes[i].blocked[j].pid = NOT_USED;
        blockedThread = mutexes[i].blocked[j].thread;
        mutexes[i].blocked[j].thread = NOT_USED;
        mutexes[i].blockedQuantity--;
        changeThreadState(pid, blockedThread, READY);
      }
    }
  }

  mutexUp(adminMutex, pid, thread);
}
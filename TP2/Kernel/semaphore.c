#include <semaphore.h>
#include <types.h>
#include <process.h>

static semaphore_t semaphores[MAX_SEMAPHORES];
static int firstAvailableSemaphore = 0;
static int usedSemaphores = 0;
static int semaphoreMutex;

void setupSemaphoreSystem() {
  int i;

  for(i = 0 ; i < MAX_SEMAPHORES ; i++) {
    clearSemaphore(i);
  }
  semaphoreMutex = createMutex(0, "_SEM_MUTEX_", 0);
}

int getSemaphore(char * name, int pid, int thread) {
  int i;
  int semaphore;

  mutexDown(semaphoreMutex, pid, thread);
  for(i = 0; i < MAX_SEMAPHORES; i++) {
    if(!strcmp(semaphores[i].name, name) && semaphores[i].creator.pid != NOT_USED) {
      return i;
    }
  }

  semaphore = createSemaphore(pid, name, thread);

  mutexUp(semaphoreMutex, pid, thread);
  return semaphore;
}

int createSemaphore(int pid, char * name, int thread) {
  int semaphore;
  int mutex;
  size_t length = strlen(name);

  if(usedSemaphores < MAX_SEMAPHORES && length < MAX_NAME) {
    semaphore = firstAvailableSemaphore;
    memcpy(semaphores[semaphore].name, name, length);
    semaphores[semaphore].creator.pid = pid;
    semaphores[semaphore].creator.thread = thread;
    mutex = getMutex(name, pid, thread);
    if(mutex == MUTEX_CREATION_ERROR)
      semaphore = SEM_CREATION_ERROR;
    else {
      semaphores[semaphore].mutex = mutex;
      usedSemaphores++;
      firstAvailableSemaphore = getNextSemaphoreAvailable();
    }
  } else
    semaphore = SEM_CREATION_ERROR;
  return semaphore;
}


int releaseSemaphore(int pid, int semaphore, int thread) {
  int released = 0;

  mutexDown(semaphoreMutex, pid, thread);

  if(isValidSemaphore(semaphore)) {
    if (pid == semaphores[semaphore].creator.pid && thread == semaphores[semaphore].creator.thread) {
      releaseMutex(pid, semaphores[semaphore].mutex, thread);
      semaphores[semaphore].creator.pid = NOT_USED;
      while(semaphores[semaphore].blockedQuantity > 0)
        unlockProcess(semaphore);
      released = 1;
    }
  }

  mutexUp(semaphoreMutex, pid, thread);
  return released;
}

int getNextSemaphoreAvailable() {
  int i;

  for(i = 0; i < MAX_SEMAPHORES; i++) {
    if(semaphores[i].creator.pid == NOT_USED) {
      return i;
    }
  }

  return FULL_LIST;
}

void clearSemaphore(int semaphore) {
  int i;

  if(semaphore < MAX_SEMAPHORES) {
    semaphores[semaphore].creator.pid = NOT_USED;
    semaphores[semaphore].name[0] = '\0';
    semaphores[semaphore].value = 0;
    semaphores[semaphore].blockedQuantity = 0;

    for(i = 0; i < MAX_BLOCKED; i++) {
      semaphores[semaphore].blocked[i].pid = NOT_USED;
      semaphores[semaphore].blocked[i].thread = NOT_USED;
    }

    if(semaphore < firstAvailableSemaphore)
      firstAvailableSemaphore = semaphore;
    else
      firstAvailableSemaphore = getNextSemaphoreAvailable();
  }
}

int addToSemaphoreBlocked(int semaphore, int pid, int thread) {
  int index;

  index = nextListIndexAvailable(semaphores[semaphore].blocked, MAX_BLOCKED);
  if(index != FULL_LIST) {
    semaphores[semaphore].blocked[index].pid = pid;
    semaphores[semaphore].blocked[index].thread = thread;
    semaphores[semaphore].blockedQuantity++;
  }

  return index;
}

int isValidSemaphore(int semaphore) {
  if(semaphore < 0 || semaphore >= MAX_SEMAPHORES)
    return 0;

  return semaphores[semaphore].creator.pid != NOT_USED;
}

void wait(int semaphore, int pid, int thread) {
  int added;

  if(isValidSemaphore(semaphore)) {

    mutexDown(semaphores[semaphore].mutex, pid, thread);

    semaphores[semaphore].value--;

    if(semaphores[semaphore].value < 0) {
      added = addToSemaphoreBlocked(semaphore, pid, thread);
      if (added != FULL_LIST) {
        changeThreadState(pid, thread, BLOCKED);
        mutexUp(semaphores[semaphore].mutex, pid, thread);
        yield();
      } else {
        killThread(pid, thread);
        mutexUp(semaphores[semaphore].mutex, pid, thread);
        yield();
      }
    } else {
      mutexUp(semaphores[semaphore].mutex, pid, thread);
    }
  }
}

void signal(int semaphore) {

  int pid = getpid();
  int thread = getCurrentThread();

  if(isValidSemaphore(semaphore)) {
    mutexDown(semaphores[semaphore].mutex, pid, thread);
    unlockSemaphoreProcess(semaphore);

    semaphores[semaphore].value++;
    mutexUp(semaphores[semaphore].mutex, pid, thread);
  }
}

void setValue(int semaphore, int value) {
  int pid = getpid();
  int thread = getCurrentThread();

  if(isValidSemaphore(semaphore)) {
    mutexDown(semaphores[semaphore].mutex, pid, thread);
    semaphores[semaphore].value = value;
    mutexUp(semaphores[semaphore].mutex, pid, thread);
  }
}

int unlockSemaphoreProcess(int semaphore) {
  int i = 0;
  int unlocked = 0;

  if(semaphores[semaphore].blockedQuantity == 0) {
    return 0;
  }

  while(i < MAX_BLOCKED && !unlocked) {
    if(semaphores[semaphore].blocked[i].pid != NOT_USED) {
      changeThreadState(semaphores[semaphore].blocked[i].pid, semaphores[semaphore].blocked[i].thread, READY);
      semaphores[semaphore].blocked[i].pid = NOT_USED;
      semaphores[semaphore].blocked[i].thread = NOT_USED;
      semaphores[semaphore].blockedQuantity--;
      unlocked = 1;
    }
    i++;
  }

  return unlocked;
}

void removePidFromSemaphores(int pid, int thread) {
  int i, j;
  int blockedThread;

  mutexDown(semaphoreMutex, pid, thread);

  for(i = 0 ; i < MAX_SEMAPHORES ; i++){
    if(semaphores[i].creator.pid == pid) {
      releaseSemaphore(pid, i, thread);
    }

    for(j = 0; j < MAX_BLOCKED; j++) {
      if (semaphores[i].blocked[j].pid == pid) {
        semaphores[i].blocked[j].pid = NOT_USED;
        blockedThread = semaphores[i].blocked[j].thread;
        semaphores[i].blocked[j].thread = NOT_USED;
        semaphores[i].blockedQuantity--;
        semaphores[i].value++;
        changeThreadState(pid, blockedThread, READY);
      }
    }
  }

  mutexUp(semaphoreMutex, pid, thread);
}

void removeThreadFromSemaphores(int pid, int thread) {
  int i, j;
  int blockedThread;

  mutexDown(semaphoreMutex, pid, thread);

  for(i = 0 ; i < MAX_SEMAPHORES ; i++){
    if(semaphores[i].creator.pid == pid && semaphores[i].creator.thread == thread) {
      releaseSemaphore(pid, i, thread);
    }

    for(j = 0; j < MAX_BLOCKED; j++) {
      if (semaphores[i].blocked[j].pid == pid && semaphores[i].blocked[j].thread == thread) {
        semaphores[i].blocked[j].pid = NOT_USED;
        blockedThread = semaphores[i].blocked[j].thread;
        semaphores[i].blocked[j].thread = NOT_USED;
        semaphores[i].blockedQuantity--;
        semaphores[i].value++;
        changeThreadState(pid, blockedThread, READY);
      }
    }
  }

  mutexUp(semaphoreMutex, pid, thread);
}

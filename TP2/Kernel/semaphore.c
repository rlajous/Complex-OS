#include <semaphore.h>

static semaphore_t semaphores[MAX_SEMAPHORES];
static int firstAvailableSemaphore = 0;
static int usedSemaphores = 0;
static int semaphoreMutex;

void setupSemaphoreSystem() {
  int i;

  for(i = 0 ; i < MAX_SEMAPHORES ; i++) {
    clearSemaphore(i);
  }
  semaphoreMutex = createMutex(0, "_SEM_MUTEX_");
}

int getSemaphore(char * name, int pid) {
  int i;
  int semaphore;

  mutexDown(semaphoreMutex, pid);
  for(i = 0; i < MAX_SEMAPHORES; i++) {
    if(!strcmp(semaphores[i].name, name) && semaphores[i].pid != NOT_USED) {
      return i;
    }
  }

  semaphore = createSemaphore(pid, name);

  mutexUp(semaphoreMutex, pid);
  return semaphore;
}

int createSemaphore(int pid, char * name) {
  int semaphore;
  int mutex;
  size_t length = strlen(name);

  if(usedSemaphores < MAX_SEMAPHORES && length < MAX_NAME) {
    semaphore = firstAvailableSemaphore;
    memcpy(semaphores[semaphore].name, name, length);
    semaphores[semaphore].pid = pid;
    mutex = getMutex(name, pid);
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


int releaseSemaphore(int pid, int semaphore) {
  int released = 0;

  mutexDown(semaphoreMutex, pid);

  if(isValidSemaphore(semaphore)) {
    if (pid == semaphores[semaphore].pid) {
      releaseMutex(pid, semaphores[semaphore].pid);
      semaphores[semaphore].pid = NOT_USED;
      while(semaphores[semaphore].blockedQuantity > 0)
        unlockProcess(semaphore);
      released = 1;
    }
  }

  mutexUp(semaphoreMutex, pid);
  return released;
}

int getNextSemaphoreAvailable() {
  int i;

  for(i = 0; i < MAX_SEMAPHORES; i++) {
    if(semaphores[i].pid == NOT_USED) {
      return i;
    }
  }

  return FULL_LIST;
}

void clearSemaphore(int semaphore) {
  int i;

  if(semaphore < MAX_SEMAPHORES) {
    semaphores[semaphore].pid = NOT_USED;
    semaphores[semaphore].name[0] = '\0';
    semaphores[semaphore].value = 0;
    semaphores[semaphore].blockedQuantity = 0;

    for(i = 0; i < MAX_BLOCKED; i++) {
      semaphores[semaphore].blocked[i] = NOT_USED;
    }

    if(semaphore < firstAvailableSemaphore)
      firstAvailableSemaphore = semaphore;
    else
      firstAvailableSemaphore = getNextSemaphoreAvailable();
  }
}

int addToSemaphoreBlocked(int semaphore, int pid) {
  int index;

  index = nextListIndexAvailable(semaphores[semaphore].blocked, MAX_BLOCKED);
  if(index != FULL_LIST) {
    semaphores[semaphore].blocked[index] = pid;
    semaphores[semaphore].blockedQuantity++;
  }

  return index;
}

int isValidSemaphore(int semaphore) {
  if(semaphore < 0 || semaphore >= MAX_SEMAPHORES)
    return 0;

  return semaphores[semaphore].pid != NOT_USED;
}

void wait(int semaphore, int pid) {
  int added;

  if(isValidSemaphore(semaphore)) {

    mutexDown(semaphores[semaphore].mutex, pid);

    semaphores[semaphore].value--;

    if(semaphores[semaphore].value < 0) {
      added = addToSemaphoreBlocked(semaphore, pid);
      if (added != FULL_LIST) {
        changeProcessState(pid, BLOCKED);
        mutexUp(semaphores[semaphore].mutex, pid);
        yield();
      } else {
        killProcess(pid);
        mutexUp(semaphores[semaphore].mutex, pid);
        yield();
      }
    } else {
      mutexUp(semaphores[semaphore].mutex, pid);
    }
  }
}

void signal(int semaphore) {

  int pid = getpid();

  if(isValidSemaphore(semaphore)) {
    mutexDown(semaphores[semaphore].mutex, pid);
    unlockSemaphoreProcess(semaphore);

    semaphores[semaphore].value++;
    mutexUp(semaphores[semaphore].mutex, pid);
  }
}

int unlockSemaphoreProcess(int semaphore) {
  int i = 0;
  int unlocked = 0;

  if(semaphores[semaphore].blockedQuantity == 0) {
    return 0;
  }

  while(i < MAX_BLOCKED && !unlocked) {
    if(semaphores[semaphore].blocked[i] != NOT_USED) {
      changeProcessState(semaphores[semaphore].blocked[i], READY);
      semaphores[semaphore].blocked[i] = NOT_USED;
      semaphores[semaphore].blockedQuantity--;
      unlocked = 1;
    }
    i++;
  }

  return unlocked;
}

void removePidFromSemaphores(int pid) {
  int i, j;

  mutexDown(semaphoreMutex, pid);

  for(i = 0 ; i < MAX_SEMAPHORES ; i++){
    if(semaphores[i].pid == pid) {
      releaseSemaphore(pid, i);
    }

    for(j = 0; j < MAX_BLOCKED; j++) {
      if (semaphores[i].blocked[j] == pid) {
        semaphores[i].blocked[j] = NOT_USED;
        semaphores[i].blockedQuantity--;
        semaphores[i].value++;
        changeProcessState(pid, READY);
      }
    }
  }

  mutexUp(semaphoreMutex, pid);
}

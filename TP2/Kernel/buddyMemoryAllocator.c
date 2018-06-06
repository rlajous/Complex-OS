#include <buddyMemoryAllocator.h>
#include <mutex.h>

static buddyHeap_t buddyHeap;
static int maxLevels = 0;
static int buddyMutex;

//http://www.memorymanagement.org/mmref/alloc.html
buddyHeap_t initializeHeap() {
  int i;

  buddyHeap.memoryBase[0] = base;
  buddyHeap.blockUsage[0] = UNUSED;

	for(i = 1; i < HEAPSIZE; i++) {
    buddyHeap.memoryBase[i] = NULL;
    buddyHeap.blockUsage[i] = UNUSED;
	}
  maxLevels = log2(PAGE_QUANTITY);

	buddyMutex = createMutex(0, "_BUDDY_MUTEX_", 0);

	return buddyHeap;
}

void * allocatePages(int pages) {
  if(pages == 0 || pages > PAGE_QUANTITY)
    return NULL;

  void * address;
  int pid = getpid();
  int thread = getCurrentThread();

  mutexDown(buddyMutex, pid, thread);

  address = getPages(pages);

  mutexUp(buddyMutex, pid, thread);

  return address;
}

void * allocateMemory(uint64_t bytes) {
  int pages = pagesToAllocate(bytes);

  return allocatePages(pages);
}

void * reallocateMemory(uint64_t bytes, void* address) {
  int pages = pagesToAllocate(bytes);

  return reallocatePages(pages, address);
}

//NOT TESTED
void * reallocatePages(int pages, void* address) {
  void * aux = NULL;
  int previousPages;
  int divisions = log2(getParentIndex(address)) + 1;

  if(pages == 0 || pages > PAGE_QUANTITY)
    return aux;

  if(freeMemory(address) == 1) {
    previousPages = ((PAGE_SIZE * PAGE_QUANTITY) >> divisions);
    aux = allocatePages(pages);
    if(aux != NULL)
      memcpy(address, aux, previousPages * PAGE_SIZE);
  }
  return aux;
}

void * getPages(int quantity) {
  int levelsToDescend = maxLevels - log2(quantity);

  if(quantity == 1)
    levelsToDescend = maxLevels;

  return getPagesRecursive(1, levelsToDescend);
}

void * getPagesRecursive(int index, int levels) {

  void * left;
  void * right;

  if(buddyHeap.blockUsage[index - 1] == FULLY_USED || buddyHeap.blockUsage[index - 1] == ALLOCATED) {
    return NULL;
  }
  if(levels == 0) {
    if(buddyHeap.blockUsage[index - 1] == HALF_USED)
      return NULL;
    buddyHeap.blockUsage[index - 1] = ALLOCATED;
    return buddyHeap.memoryBase[index - 1];
  }
  setMemoryBlockDivisionBases(index);

  left = getPagesRecursive(LEFT(index), levels - 1);

  if(left != NULL) {
    buddyHeap.blockUsage[index - 1] = HALF_USED;
    return left;
  }

  right = getPagesRecursive(RIGHT(index), levels - 1);


  if(right != NULL && (buddyHeap.blockUsage[RIGHT(index) - 1] != HALF_USED)
     && (buddyHeap.blockUsage[LEFT(index) - 1] != HALF_USED)) {
    buddyHeap.blockUsage[index - 1] = FULLY_USED;
  }
  return right;
}

void setMemoryBlockDivisionBases(int parentIndex) {
  int divisions;
  unsigned long memory, rightMemoryDivisionBase;

  if(buddyHeap.memoryBase[LEFT(parentIndex) - 1] == NULL || buddyHeap.memoryBase[RIGHT(parentIndex) - 1] == NULL) {
    divisions = log2(parentIndex) + 1;
    if(parentIndex % 2 != 0)
      divisions -= 1;
    memory = (unsigned long)buddyHeap.memoryBase[parentIndex - 1];
	  buddyHeap.memoryBase[LEFT(parentIndex) - 1] = (void *)memory;

    rightMemoryDivisionBase = ((PAGE_SIZE * PAGE_QUANTITY) >> divisions) + memory;

    buddyHeap.memoryBase[RIGHT(parentIndex) - 1] = (void *)rightMemoryDivisionBase;
   }
}

int freeMemory(void * memoryBase) {
  int pid = getpid();
  int thread = getCurrentThread();

	if(isAllocatedPage(memoryBase)) {
    mutexDown(buddyMutex, pid, thread);
		int index = ((unsigned long)memoryBase - (unsigned long)base)/PAGE_SIZE;
    index = index + (HEAPSIZE/2);
		buddyHeap.blockUsage[index] = UNUSED;
		freeMemoryRecursive(PARENT(index + 1));
    mutexUp(buddyMutex, pid, thread);
		return 1;
	}

	return 0;
}

void freeMemoryRecursive(int index) {
  if(index > 0) {
    if(buddyHeap.blockUsage[LEFT(index) - 1] == UNUSED && buddyHeap.blockUsage[RIGHT(index) - 1] == UNUSED) {
      buddyHeap.blockUsage[index - 1] = UNUSED;
    }

    if(buddyHeap.blockUsage[LEFT(index) - 1] == FULLY_USED || buddyHeap.blockUsage[RIGHT(index) - 1] == FULLY_USED) {
      buddyHeap.blockUsage[index - 1] = HALF_USED;
    }

    if(buddyHeap.blockUsage[LEFT(index) - 1] == HALF_USED || buddyHeap.blockUsage[RIGHT(index) - 1] == HALF_USED) {
      buddyHeap.blockUsage[index - 1] = HALF_USED;
    }

    if(buddyHeap.blockUsage[LEFT(index) - 1] == ALLOCATED || buddyHeap.blockUsage[RIGHT(index) - 1] == ALLOCATED) {
      buddyHeap.blockUsage[index - 1] = HALF_USED;
    }
    freeMemoryRecursive(PARENT(index));
  }
}

int getParentIndex(void * memoryBase) {
  int index = ((unsigned long)memoryBase - (unsigned long)base)/PAGE_SIZE + 1;
  return getParentIndexRecursive(memoryBase, index + (HEAPSIZE/2));
}

int getParentIndexRecursive(void * memoryBase, int index) {

  if(index <= 0)
    return 0;

  if(buddyHeap.blockUsage[index - 1] == ALLOCATED && buddyHeap.memoryBase[index - 1] == memoryBase)
    return PARENT(index);

  if(buddyHeap.blockUsage[index - 1] == UNUSED)
    return getParentIndexRecursive(memoryBase, PARENT(index));

  return -1;
}

int isAllocatedPage(void * memoryBase) {
  int index = ((unsigned long)memoryBase - (unsigned long)base)/PAGE_SIZE + 1;
  return isAllocatedPageRecursive(memoryBase, index + (HEAPSIZE/2));
}

int isAllocatedPageRecursive(void * memoryBase, int index) {

  if(index <= 0)
    return 0;

  if(buddyHeap.blockUsage[index - 1] == ALLOCATED && buddyHeap.memoryBase[index - 1] == memoryBase)
    return 1;

  if(buddyHeap.blockUsage[index - 1] == UNUSED)
    return isAllocatedPageRecursive(memoryBase, PARENT(index));

  return 0;
}

int pagesToAllocate(uint64_t bytes) {
  int pages = bytes/PAGE_SIZE;

  return (bytes % PAGE_SIZE) == 0 ? pages : pages + 1;
}

int log2(uint64_t x) {
  int result = 0;

  if(x % 2)
    result++;

  while(x >>= 1)
    result++;
  return result;
}

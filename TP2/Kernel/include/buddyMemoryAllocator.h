#ifndef BUDDYMEMORYALLOCATOR_H
#define BUDDYMEMORYALLOCATOR_H

#include <stdint.h>
#include <MMU.h>
#include <types.h>

#define LEFT(x) ((x) << 1)
#define RIGHT(x) (((x) << 1) + 1)
#define PARENT(x) ((x) >> 1)
#define FULLY_USED 0
#define HALF_USED 1
#define UNUSED 2
#define ALLOCATED 3

#define PAGE_SIZE 0x1000 //4KB
#define PAGE_QUANTITY 2048 //8MB

#define HEAPSIZE ((2 * PAGE_QUANTITY) - 1)

typedef struct {
  void * memoryBase[HEAPSIZE];
  char blockUsage[HEAPSIZE];
} buddyHeap_t;

static void * const base = (void*)0x600000;

buddyHeap_t initializeHeap();
void * allocatePages(int pages);
void * allocateMemory(uint64_t bytes);
void * getPages(int quantity);
void * getPagesRecursive(int index, int levels);
void setMemoryBlockDivisionBases(int parentIndex);
int freeMemory(void * memoryBase);
void freeMemoryRecursive(int index);
int isAllocatedPage(void * memoryBase);
int isAllocatedPageRecursive(void * memoryBase, int index);
int pagesToAllocate(uint64_t bytes);
int log2(uint64_t x);

//debug
//void printHeap(buddyHeap_t * heap);

#endif

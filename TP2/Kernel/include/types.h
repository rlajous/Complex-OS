#ifndef TYPES_H
#define TYPES_H

typedef enum {RUNNING, READY, BLOCKED, SLEEPING} processState;

typedef struct {
	char * name;
	void * address;
	int size;
} module_t;

typedef struct {
	char ch;
	char style;
} cell_t;

typedef struct {
		int pid;
		int thread;
} threadId_t;

typedef struct {
		void * stack;
		void * entryPoint;
		void * memoryBase;
		processState state;
		int waitingThread;
} thread_t;

typedef struct {
		thread_t * thread;
		int next;
} threadNode_t;

typedef int (*EntryPoint)(int argc, char *argv[]);

#endif

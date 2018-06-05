#ifndef PIPES_H
#define PIPES_H

#define PIPE_PAGES 1
#define PROCESS_NAME_LENGTH 20
#define PIPE_BUFFER_LENGTH = PIPE_PAGES * PAGE_SIZE
#define MAX_PIPES 10

typedef struct {
    int pid;
    char name[PROCESS_NAME_LENGTH];
    int readIndex;
    int writeIndex;
    char * buffer;
    int semWrite;
    int semRead;
    int used;
    int mutex;
} pipe_t;


int createPipe(char * pipeName);
void initializePipes();
int getPipe(char * pipeName);
int writePipe(int pipe, char * message, int bytes);
int readPipe(int pipe, char * buffer, int bytes);

#endif

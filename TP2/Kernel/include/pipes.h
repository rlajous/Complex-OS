#ifndef PIPES_H
#define PIPES_H

#define PIPE_BUFFER_LENGTH 20
#define PROCESS_NAME_LENGTH 20
#define MAX_PIPES 10

typedef struct {
    char name[PROCESS_NAME_LENGTH];
    int readIndex;
    int writeIndex;
    int bytes;
    int buffer[PIPE_BUFFER_LENGTH];
    int pid;
    int semaphore1;
    int semaphore2;
    int used;
} Pipe;

#endif

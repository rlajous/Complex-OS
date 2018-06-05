#include <pipes.h>
#include <scheduler.h>
#include <semaphore.h>
#include <lib.h>

static pipe_t pipes[MAX_PIPES];

int createPipe(char * pipeName){
    int pid = getpid();
    int  i;

    if(pipeName == NULL){
        return -1;
    }

    for(i = 0; i < MAX_PIPES; i++) {
        if(pipe[i].used == FALSE){
            pipes[i].pid = pid;
            size_t nameLength = strlen(pipeName);
            memcpy(pipes[i].name, pipeName);
            pipes[i].readIndex = 0;
            pipes[i].writeIndex = 0;
            char semWriteName[15] ="PIPE_WRITE_";
            uintToBase(i, semWriteName + 11, 10);
            p->semWrite = createSemaphore(pid, semWriteName);
            p->semWrite.value = PIPE_BUFFER_LENGTH - 1;
            char semReadName[15] ="PIPE_READ_";
            uintToBase(i, semReadName + 11, 10);
            p->semRead = createSemaphore(pid, semReadName);
            p->buffer = allocatePages(PIPE_PAGES);
            char mutexName[15] ="PIPE_MUTEX";
            uintToBase(i, mutexName + 10, 10);
            p->mutex = getMutex(mutexName, pid);

            return i;
        }
    }

    return -1;
}

void initializePipes(){
    int i, j;
    int pid = getPid();

    for(i = 0; i < MAX_PIPES; i++){
        pipe_t * p = &pipes[i];

        p->readIndex = 0;
        p->writeIndex = 0;
        for(j = 0; j < PIPE_BUFFER_LENGTH; j++){
            p->buffer[j] = 0;
        }
        p->pid = 0;
        char semWriteName[15] ="PIPE_WRITE_";
    }
}

int getPipe(char * pipeName){
    int i;

    for(i = 0; i < MAX_PIPES; i++){
        if(strcmp(pipes[i].name, pipeName) == 0 && pipes[i].used == TRUE){
            return i + 3;
        }
    }

    return createPipe(pipeName) + 3;
}

int writePipe(int pipe, char * message, int bytes, int pid){
    pipe = pipe - 3;

    pipe_t * p = &pipes[pipe];
    int i = 0;

    if(!isValidPipe(pipe)) {
        return -1;
    }

    while(bytes != 0){
        wait(p->semWrite, pid);
        mutexDown(p->mutex, pid);
        p->buffer[p->writeIndex] = message[i++];
        mutexUp(p->mutex, pid);
        bytes--;

        if(p->writeIndex == PIPE_BUFFER_LENGTH){
            p->writeIndex = 0;
        }
        else{
            p->writeIndex++;
        }
    }

    signal(p->semRead);
    return 0;
}

int readPipe(int pipe, char * buffer, int bytes, int pid){
    pipe = pipe - 3;

    pipe_t * p = &pipes[pipe];
    int i = 0;

    if(!isValidPipe(pipe)) {
        return -1;
    }

    while(bytes != 0){
        wait(p->semRead, pid);
        mutexDown(p->mutex, pid);
        buffer[i++] = p->buffer[p->readIndex];
        mutexUp(p->mutex, pid);
        bytes--;
        if(p->readIndex == PAGE_SIZE * PIPE_PAGES){
            p->readIndex = 0;
        }
        else {
            p->readIndex++;
        }
    }

    signal(p->semWrite);
    return 0;
}

int deletePipe(int pipe, int pid){

    pipe_t * p;

    if(!isValidPipe(pipe)) {
        return -1;
    }

    p = &pipes[pipe];

    if(p->pid == pid){
        freeMemory(p->buffer);
        releaseMutex(p->pid, p->mutex);
        releaseSemaphore(p->pid, p->semWrite);
        releaseSemaphore(p->pid, p->semRead);
        p->used = FALSE;

        return 0;
    }

    return -1;
}

int isValidPipe(int pipe) {
    if(pipe < 0 || pipe >= MAX_PIPES)
        return 0;

    return pipes[pipe].used != FALSE;
}


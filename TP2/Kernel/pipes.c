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
            pipes[i].bytes = 0;
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
        p->bytes = 0;
        for(j = 0; j < PIPE_BUFFER_LENGTH; j++){
            p->buffer[j] = 0;
        }
        p->pid = 0;
        char semWriteName[12] ="SEM_WRITE_";
//      hay que concatenar el nombre con i
        p->semWrite = createSemaphore(pid, semWriteName);
        p->semWrite.value = PIPE_BUFFER_LENGTH;
        char semReadName[11] ="SEM_READ_";
//      hay que concatenar el nombre con i
        p->semRead = createSemaphore(pid, semReadName);

    }
}

int getPipe(char * pipeName){
    int i;

    for(i = 0; i < MAX_PIPES; i++){
        if(strcmp(pipes[i].name, pipeName) == 0 && pipes[i].used == TRUE){
            return i;
        }
    }

    return createPipe(pipeName);
}

int writePipe(int pipe, char * message, int bytes){
    pipe_t * p = &pipes[pipe];
    int i = 0;

    while(bytes != 0){
        if(p->bytes == PIPE_BUFFER_LENGTH){
//            wait() del semWrite no se que pid pedirle
        }
        else{
            p->buffer[p->writeIndex++] = message[i++];
            p->bytes++;
            bytes--;
        }
    }

//    signal() al semRead
    return 0;
}

int readPipe(int pipe, char * buffer, int bytes){
    pipe_t * p = &pipes[pipe];
    int i = 0;

    while(bytes != 0){
        if(p->bytes == 0){
//            wait al semRead
        }
        else{
            buffer[i++] = p->buffer[p->readIndex++];
            p->bytes++;
            bytes--;
        }
    }

//    signal al write

    return 0;
}

#include <pipes.h>
#include <scheduler.h>

static Pipe pipes[MAX_PIPES];

int createPipe(char * pipeName){
    int pid = getpid();
    int  i;

    if(pipeName == NULL){
        return -1;
    }

    for(i = 0; i < MAX_PIPES; i++) {
        if(pipe[i].used == FALSE){
            size_t nameLength = strlen(pipeName);
            memcpy(pipes[i].name, pipeName);
            pipes[i].readIndex = 0;
            pipes[i].writeIndex = 0;
            pipes[i].bytes = 0;
            pipes[i].pid = pid;
            return i;
        }
    }

    return -1;
}




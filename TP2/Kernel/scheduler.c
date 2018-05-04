#include <scheduler.h>

/*static process_t[MAX_PROCESSES] processes;
static int nextPid = 1;
static int firstAvailableSpace = 0;
static int lastExecutedProcess = 0;

uint64_t scheduler(int ticks, uint64_t rsp){
	static int quantum = 0;
	uint64_t nextRsp = rsp;
	//peek, if null exit
	if(ticks == quantum){
		nextRsp = algo;
	}
	return nextRsp;
}*/

int getPid() {
 //TODO
 return 0;
}

/*
int findFirstAvailableSpace(){
	int i = 0;
	while(i < MAX_PROCESES && process_t[i] != null){
		i++;
	}
	return i <= MAX_PROCESES ? i : -1;
}*/

//cuando algun modulo llame a exit llamar a esta mierda
//crear la estructura del pcb
// hacer una lista varialble con el memory alocator para cada vez que se crea un procesos meterlo en la cola por si vmaos a meter mil procesos (lo que podemoshacer es que el malloc cargue de a modulo 5 o alguna gilada asi no tiene que hacer el malloc que cosume su tiempo)
//




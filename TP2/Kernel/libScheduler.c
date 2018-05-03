#define MAX_PROCESES = 10;
static process_t[MAX_PROCESES] proceses;
static int firstAvaiableSpace = 0;
static int lastExecutedProcess = 0;

void * scheduler(int ticks, void * rsp){
	int quantum;
	void * nextRsp = rsp;
	//peek, if null exit
	if(ticks == quantum){
		nextRsp = algo;
	}
	return nextRsp;
}

int findFirstAvailableSpace(){
	int i = 0;
	while(i < MAX_PROCESES && process_t[i] != null){
		i++;
	}
	return i <= MAX_PROCESES ? i : -1;
}

//cuando algun modulo llame a exit llamar a esta mierda
//crear la estructura del pcb
// hacer una lista varialble con el memory alocator para cada vez que se crea un procesos meterlo en la cola por si vmaos a meter mil procesos (lo que podemoshacer es que el malloc cargue de a modulo 5 o alguna gilada asi no tiene que hacer el malloc que cosume su tiempo)
//

typedef struct {
	pointer
	processState
	processNumber
	programCounter
	registers
	memoryLimits
} process_t; 


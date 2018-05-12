#include <MMU.h>
#include <buddyMemoryAllocator.h>
#include <scheduler.h>
#include <types.h>
#include <naiveConsole.h>
#include <process.h>

static void * const executableMemoryAddress = (void*)0x1000000;
static void * const moduleAddressesStart = (void*)0x0E00000;
//static int executable = -1;

module_t modules[] = {{"shell"}, {"sampleDataModule"}, {"sampleCodeModule"}, {"ps"}, {"help"}, {"date"}, {"time"}, {"clear"}, {"echo"}, {"kill"}, {"prodcons"}, {"snek"} ,{"messageTest"} ,{0}};
/*char* moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", "hello", "help", "date", "time", "clear", "echo",0};
void * moduleAddresses[9] = {(void *)0x0E00000, (void *)0x1000000, (void *)0x1200000, (void *)0x1400000,
	(void *)0x1600000, (void *)0x1800000, (void *)0x1A00000, (void *)0x1C00000, (void *)0x1E00000};*/

int init(){
  while(1) {
    halt();
  }
}

void startSystem() {
  process_t* initProcess = createProcess(&init, 0, NULL, "init");
  process_t* shell = createProcess(modules[0].address , 0, NULL, modules[0].name);
  addProcess(initProcess);
  addProcess(shell);
  setForeground(shell->pid);
  startFirstProcess();
}

int getModule(void * moduleAddress) {
  int i = 0;
  while(modules[i].name != 0) {
    if(modules[i].address == moduleAddress) {
      return i;
    }
    i++;
  }
  return -1;
}

/*void copyModule(void *moduleAddress) {
  int index = getModule(moduleAddress);
  if(index != -1 && index != executable) {
    memcpy(executableMemoryAddress, modules[index].address, modules[index].size);
    executable = index;
  }
}*/

int addModuleProcessBackground(int moduleIndex, int argc, char **argv) {
  int ret;
  process_t * process = createProcess(modules[moduleIndex].address, argc, argv, modules[moduleIndex].name);
  ret = addProcess(process);
  yield();
  return ret;
}

int addModuleProcess(int moduleIndex, int argc, char **argv) {
  int ret;
  process_t * process = createProcess(modules[moduleIndex].address, argc, argv, modules[moduleIndex].name);
  ret = addProcess(process);
  setForeground(process->pid);
  blockProcess(2);
  yield();
  return ret;
}

char** backupArguments(int argc, char * argv[]) {
  char ** temp = argv;
  if(argc > 0) {
    temp = allocateMemory(argc*sizeof(char **));
    if(temp == NULL)
      return argv;
    memcpy(temp, argv, argc*sizeof(char **));

    for(int i = 0; i < argc; i++) {
      size_t len = strlen(temp[i]) + 1;
      char * tempArg = allocateMemory(len*sizeof(char));
      if(tempArg == NULL)
        break;
      memcpy(tempArg, temp[i], len*sizeof(char));
    }
  }
  return temp;
}

void clearBSS(void * bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
  return (void*)(
    (uint64_t)&endOfKernel
    + PageSize * 8        //The size of the stack itself, 32KiB
    - sizeof(uint64_t)      //Begin at the top of the stack
  );
}

void * initializeKernelBinary() {
  loadModules(&endOfKernelBinary, modules, moduleAddressesStart);
  clearBSS(&bss, &endOfKernel - &bss);
  return getStackBase();
}

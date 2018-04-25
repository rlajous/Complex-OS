#include <MMU.h>
#include <buddyMemoryAllocator.h>

typedef int (*EntryPoint)(int argc, char *argv[]);

static void * const executableMemoryAddress = (void*)0x400000;
static void * const executableMemoryEndAddress = (void*)0x5FFFFF;
static void * const moduleAddressesStart = (void*)0x0E00000;
//static void * const heapAddress = (void*)0x600000;

module_t modules[] = {{"shell"}, {"sampleDataModule"}, {"sampleCodeModule"}, {"hello"}, {"help"}, {"date"}, {"time"}, {"clear"}, {"echo"}, {0}};
/*char* moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", "hello", "help", "date", "time", "clear", "echo",0};
void * moduleAddresses[9] = {(void *)0x0E00000, (void *)0x1000000, (void *)0x1200000, (void *)0x1400000,
	(void *)0x1600000, (void *)0x1800000, (void *)0x1A00000, (void *)0x1C00000, (void *)0x1E00000};*/


void copyAndExecuteDefaultModule() {
	memcpy(executableMemoryAddress, modules[0].address, modules[0].size);
  sti();
	((EntryPoint)executableMemoryAddress)(0,0);
}

void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]) {
	memcpy(executableMemoryAddress, modules[moduleIndex].address, modules[moduleIndex].size);
  sti();
	((EntryPoint)executableMemoryAddress)(argc, argv);
	freeMemory(argv);

  copyAndExecuteDefaultModule();
}

/*void * malloc(uint64_t request) {
  static uint64_t capacity = 0x200000;
  static uint64_t size = 0;
  uint64_t futureSize = size + request;
  if(futureSize > capacity)
    return 0;

  uint64_t blockAddress = heapAddress + size;
  size = futureSize;
  return blockAddress;
}*/

char** backupArguments(int argc, char * argv[]) {
  if(argc > 0) {
    if((void *)argv >= executableMemoryAddress && (void *)argv < executableMemoryEndAddress) {
      char ** temp = allocateMemory(argc*sizeof(char **));
      if(temp == 0)
        return argv;
      memcpy(temp, argv, argc*sizeof(char **));
      argv = temp;
    }
    for(int i = 0; i < argc; i++) {
      size_t len = strlen(argv[i]) + 1;
      char * temp = allocateMemory(len*sizeof(char));
      if(temp == 0)
        break;
      memcpy(temp, argv[i], len*sizeof(char));
      argv[i] = temp;
    }
  }
  return argv;
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

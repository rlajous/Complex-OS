#include <sysCalls.h>


extern module_t modules[];

static sys sysCalls[SYSCALLS];

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size) {
	int index = 0;
	if(!isForeground(getpid())) {
		blockProcess(getpid());
		yield();
	}
	if(fileDescriptor == 0) {
		while(index++ < size)
			*((char*)buffer++)= readBuffer();
	}
	return 0;
}

int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size) {
	if(fileDescriptor == 1 || fileDescriptor == 2) {
		char next;
		while(size--) {
			next = *(char *)(buffer++);
			if(fileDescriptor == 1)
				printc(next);
			else
				printcWithStyle(next, 0x04);
		}
	}
	return 0;
}

int sysClear(uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	clearScreen();
	return 0;
}

int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx) {
	setTimeZone((int) timeZone);
	return 0;
}

int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds) {
	*(int*)hour = getTime(HOURS);
	*(int*)minute = getTime(MINUTES);
	*(int*)seconds = getTime(SECONDS);
	return 0;
}

int sysGetDate(uint64_t day, uint64_t month, uint64_t year) {
	*(int*)day = getTime(DAY);
	*(int*)month = getTime(MONTH);
	*(int*)year = getTime(YEAR);
	return 0;
}

int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx) {
	setEcho((uint8_t) echoOn);
	return 0;
}

int sysExec(uint64_t filename, uint64_t argc, uint64_t argv) {
	int i = 0;
	while(modules[i].name != 0){
		if(strcmp((const char *)filename, modules[i].name) == 0) {
			argv = (uint64_t)backupArguments(argc, (char **)argv);
			return addModuleProcess(i, argc, (char **) argv);
		}
		i++;
	}
	return -1;
}

int sysExecBackground(uint64_t filename, uint64_t argc, uint64_t argv) {
	int i = 0;
	while(modules[i].name != 0){
		if(strcmp((const char *)filename, modules[i].name) == 0) {
			argv = (uint64_t)backupArguments(argc, (char **)argv);
			return addModuleProcessBackground(i, argc, (char **) argv);
		}
		i++;
	}
	return -1;
}

int sysKill(uint64_t pid, uint64_t rdx, uint64_t rcx) {
	killProcess(pid);
	yield();
	return 0;
}

int sysPs(uint64_t buffer, uint64_t size, uint64_t rcx) {
	getProcesses((char*)buffer, (int) size);
  return 0;
}

int sysGetPid(uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	return getpid();
}

int sysAllocateMemory(uint64_t address, uint64_t size, uint64_t rcx) {
	*((uint64_t *)address) = (uint64_t)allocateMemory(size);
	return 0;
}

int sysFreeMemory(uint64_t address, uint64_t rdx, uint64_t rcx) {
	return freeMemory((void *)address);
}

int sysCreateMutex(uint64_t name, uint64_t rdx, uint64_t rcx) {
	return getMutex((char *) name, getpid());
}

int sysReleaseMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx) {
	return releaseMutex(getpid(), (int)mutex);
}

int sysUpMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx) {
	mutexUp((int) mutex, getpid());
	return 0;
}

int sysDownMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx) {
	mutexDown((int) mutex, getpid());
	return 0;
}

int sysCreateChannel(uint64_t recipientPid, uint64_t rdx, uint64_t rcx) {
	if(createChannel(getpid(), (int)recipientPid) == NULL)
		return -1;
	return 0;
}

int sysSend(uint64_t recipientPid, uint64_t message, uint64_t length) {
	return sendMessage((int)recipientPid, (char *) message, (int)length);
}

int sysReceive(uint64_t senderPid, uint64_t buffer, uint64_t length) {
  char * result = receiveMessage((int) senderPid, (int)length);
  if(result != NULL) {
    memcpy((char *) buffer, result, length);
    freeMemory(result);
    return 0;
  }
	return -1;
}

int sysDeleteChannel(uint64_t recipientPid, uint64_t rdx, uint64_t rcx) {
  channelNode_t * channel;
  channel = getChannelFromList((int)recipientPid, getpid());
  if(channel != NULL)
    deleteChannel(channel);
	return 0;
}

int sysWriteCharAtScreenPos(uint64_t ch, uint64_t x, uint64_t y){
	printCharAtPos((char)ch, (int)x, (int)y);
	return 0;
}

int sysRunProcess(uint64_t entryPoint, uint64_t argc, uint64_t argv) {
  int ret;
  argv = (uint64_t)backupArguments(argc, (char **)argv);
  process_t * process = createProcess((void *) entryPoint, argc, (char **) argv, ((char **) argv)[0]);
  ret = addProcess(process);
  yield();
  return ret;
}

int sysCreateSemaphore(uint64_t name, uint64_t rdx, uint64_t rcx) {
	return getSemaphore((char *) name, getpid());
}

int sysReleaseSemaphre(uint64_t mutex, uint64_t rdx, uint64_t rcx) {
	return releaseSemaphore(getpid(), (int)mutex);
}

int sysSignal(uint64_t semaphore, uint64_t rdx, uint64_t rcx) {
	signal((int) semaphore);
	return 0;
}

int sysWait(uint64_t semaphore, uint64_t rdx, uint64_t rcx) {
	wait((int) semaphore, getpid());
	return 0;
}

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	if(rdi >= SYSCALLS)
		return -1;
	return sysCalls[rdi](rsi, rdx, rcx);
}

void sysCallsSetup(){
	sysCalls[0] = &sysRead;
	sysCalls[1] = &sysWrite;
	sysCalls[2] = &sysClear;
	sysCalls[3] = &sysSetTimeZone;
	sysCalls[4] = &sysGetTime;
	sysCalls[5] = &sysGetDate;
	sysCalls[6] = &sysEcho;

	sysCalls[7] = &sysExec;
	sysCalls[8] = &sysKill;
	sysCalls[9] = &sysPs;
	sysCalls[10] = &sysGetPid;
	sysCalls[11] = &sysExecBackground;

	sysCalls[12] = &sysAllocateMemory;
	sysCalls[13] = &sysFreeMemory;

	sysCalls[14] = &sysCreateMutex;
	sysCalls[15] = &sysReleaseMutex;
	sysCalls[16] = &sysUpMutex;
	sysCalls[17] = &sysDownMutex;

	sysCalls[18] = &sysCreateChannel;
	sysCalls[19] = &sysSend;
	sysCalls[20] = &sysReceive;
	sysCalls[21] = &sysDeleteChannel;

	sysCalls[22] = &sysWriteCharAtScreenPos;
  sysCalls[23] = &sysRunProcess;

	sysCalls[24] = &sysCreateSemaphore;
	sysCalls[25] = &sysReleaseSemaphre;
	sysCalls[26] = &sysSignal;
	sysCalls[27] = &sysWait;
}

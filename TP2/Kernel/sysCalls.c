#include <sysCalls.h>
#include <pipes.h>
#include <process.h>
#include <types.h>


extern module_t modules[];

static sys sysCalls[SYSCALLS];

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size, uint64_t noBlock) {
	int index = 0;
	int pipe = getProcess(getpid())->standardPipes[0];

  if(fileDescriptor > 2 || pipe != -1) {
    readPipe(pipe == -1? (int)fileDescriptor: pipe, (char *)buffer, (int)size, getpid(), getCurrentThread());
    return 0;
  }

	if(!isForeground(getpid()) && fileDescriptor == 0) {
		changeThreadState(getpid(), getCurrentThread(), BLOCKED);
		yield();
	}
	if(fileDescriptor == 0) {
		while(index++ < size) {
		  if(noBlock)
        *((char *) buffer++) = nonBlockRead();
		  else
        *((char *) buffer++) = readBuffer();
    }
	}

	return 0;
}

int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size, uint64_t r8) {
  int pipe = getProcess(getpid())->standardPipes[1];
	if((fileDescriptor == 1 || fileDescriptor == 2) && pipe == -1) {
		char next;
		while(size--) {
			next = *(char *)(buffer++);
			if(fileDescriptor == 1)
				printc(next);
			else
				printcWithStyle(next, 0x04);
		}
	}
	else {
		writePipe(pipe == -1? (int)fileDescriptor: pipe, (char *)buffer, (int)size, getpid(), getCurrentThread());
	}
	return 0;
}

int sysClear(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	clearScreen();
	return 0;
}

int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	setTimeZone((int) timeZone);
	return 0;
}

int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds, uint64_t r8) {
	*(int*)hour = getTime(HOURS);
	*(int*)minute = getTime(MINUTES);
	*(int*)seconds = getTime(SECONDS);
	return 0;
}

int sysGetDate(uint64_t day, uint64_t month, uint64_t year, uint64_t r8) {
	*(int*)day = getTime(DAY);
	*(int*)month = getTime(MONTH);
	*(int*)year = getTime(YEAR);
	return 0;
}

int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	setEcho((uint8_t) echoOn);
	return 0;
}

int sysRunModule(uint64_t filename, uint64_t argc, uint64_t argv, uint64_t background) {
	void * address = getModuleAddress((char*)filename);
	if(address != NULL)
		return sysRunProcess((uint64_t) address, argc, argv, background);
	else
		return -1;
}

int sysKill(uint64_t pid, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	killProcess(pid);
	yield();
	return 0;
}

int sysPs(uint64_t buffer, uint64_t size, uint64_t rcx, uint64_t r8) {
	getProcesses((char*)buffer, (int) size);
  return 0;
}

int sysGetPid(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	return getpid();
}

int sysAllocateMemory(uint64_t address, uint64_t size, uint64_t rcx, uint64_t r8) {
	*((uint64_t *)address) = (uint64_t)allocateMemory(size);
	return 0;
}

int sysFreeMemory(uint64_t address, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	return freeMemory((void *)address);
}

int sysCreateMutex(uint64_t name, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	return getMutex((char *) name, getpid(), getCurrentThread());
}

int sysReleaseMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	return releaseMutex(getpid(), (int)mutex, getCurrentThread());
}

int sysUpMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	mutexUp((int) mutex, getpid(), getCurrentThread());
	return 0;
}

int sysDownMutex(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	mutexDown((int) mutex, getpid(), getCurrentThread());
	return 0;
}

int sysCreateChannel(uint64_t recipientPid, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	if(createChannel(getpid(), (int)recipientPid) == NULL)
		return -1;
	return 0;
}

int sysSend(uint64_t recipientPid, uint64_t message, uint64_t length, uint64_t r8) {
	return sendMessage((int)recipientPid, (char *) message, (int)length);
}

int sysReceive(uint64_t senderPid, uint64_t buffer, uint64_t length, uint64_t r8) {
  char * result = receiveMessage((int) senderPid, (int)length);
  if(result != NULL) {
    memcpy((char *) buffer, result, length);
    freeMemory(result);
    return 0;
  }
	return -1;
}

int sysDeleteChannel(uint64_t recipientPid, uint64_t rdx, uint64_t rcx, uint64_t r8) {
  channelNode_t * channel;
  channel = getChannelFromList((int)recipientPid, getpid());
  if(channel != NULL)
    removeChannelFromList(channel);
	return 0;
}

int sysWriteCharAtScreenPos(uint64_t ch, uint64_t x, uint64_t y, uint64_t r8){
	printCharAtPos((char)ch, (int)x, (int)y);
	return 0;
}

int sysRunProcess(uint64_t entryPoint, uint64_t argc, uint64_t argv, uint64_t background) {
	int ret;
	argv = (uint64_t)backupArguments(argc, (char **)argv);
	process_t * process = createProcess((void *) entryPoint, argc, (char **) argv, ((char **) argv)[0]);
	ret = addProcess(process);
	if(!background) {
		process_t * foreground = getProcess(getForeground());
		changeThreadState(foreground->pid, foreground->currentThread, SLEEPING);
    setForeground(process->pid);
	}
	yield();
	return ret;
}

int sysCreateSemaphore(uint64_t name, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	return getSemaphore((char *) name, getpid(), getCurrentThread());
}

int sysReleaseSemaphre(uint64_t mutex, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	return releaseSemaphore(getpid(), (int)mutex, getCurrentThread());
}

int sysSignal(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	signal((int) semaphore);
	return 0;
}

int sysWait(uint64_t semaphore, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	wait((int) semaphore, getpid(), getCurrentThread());
	return 0;
}

int sysSleep(uint64_t milliseconds, uint64_t rdx, uint64_t rcx, uint64_t r8) {
  sleep(milliseconds, getpid(), getCurrentThread());
  return 0;
}

int sysOpenPipe(uint64_t name, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	return getPipe((char *)name);
}

int sysClosePipe(uint64_t pipe, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	return deletePipe((int)pipe, getpid(), getCurrentThread());
}

int sysAddThread(uint64_t entryPoint, uint64_t argc, uint64_t argv, uint64_t r8) {
	argv = (uint64_t)backupArguments(argc, (char **)argv);
	thread_t * thread = createThread((int)argc, (char **)argv, (void *)entryPoint);
	return addThread(getProcess(getpid()), thread);
}

int sysKillThread(uint64_t thread, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	terminateThread(getProcess(getpid()), (int)thread);
  yield();
	return 0;
}

int sysJoinThread(uint64_t thread, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	joinThread(getProcess(getpid()), getCurrentThread(), (int)thread);
	return 0;
}

int sysPipeStd(uint64_t filename, uint64_t argc, uint64_t argv, uint64_t isWriter) {
	void * address = getModuleAddress((char*)filename);
	int pipe;
	int ret;
	if(address == NULL)
		return -1;

	argv = (uint64_t)backupArguments(argc, (char **)argv);
	process_t * process = createProcess(address, argc, (char **) argv, ((char **) argv)[0]);
	ret = addProcess(process);

	process_t * foreground = getProcess(getForeground());
	changeThreadState(foreground->pid, foreground->currentThread, SLEEPING);
	setForeground(process->pid);

	pipe = getPipe("ShellPipe");
	if(isWriter)
		process->standardPipes[1] = pipe;
	else
		process->standardPipes[0] = pipe;

	yield();
	return ret;


}

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8) {
	if(rdi >= SYSCALLS)
		return -1;
	return sysCalls[rdi](rsi, rdx, rcx, r8);
}

void sysCallsSetup(){
	sysCalls[0] = &sysRead;
	sysCalls[1] = &sysWrite;
	sysCalls[2] = &sysClear;
	sysCalls[3] = &sysSetTimeZone;
	sysCalls[4] = &sysGetTime;
	sysCalls[5] = &sysGetDate;
	sysCalls[6] = &sysEcho;

	sysCalls[7] = &sysRunModule;
	sysCalls[8] = &sysKill;
	sysCalls[9] = &sysPs;
	sysCalls[10] = &sysGetPid;
	sysCalls[11] = &sysRunProcess;

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

	sysCalls[23] = &sysCreateSemaphore;
	sysCalls[24] = &sysReleaseSemaphre;
	sysCalls[25] = &sysSignal;
	sysCalls[26] = &sysWait;

  sysCalls[27] = &sysSleep;

  sysCalls[28] = &sysOpenPipe;
  sysCalls[29] = &sysClosePipe;

	sysCalls[30] = &sysAddThread;
	sysCalls[31] = &sysKillThread;
	sysCalls[32] = &sysJoinThread;

	sysCalls[33] = &sysPipeStd;
}

#include <channel.h>
#include <process.h>

static channelNode_t* firstChannel = NULL;
static int channelMutex;

void initializeChannels() {
	channelMutex = createMutex(0, "_CHANNEL_MUTEX_", 0);
}

channelNode_t* getChannelFromList(int recipientPid, int senderPid) {
	channelNode_t * node = firstChannel;

	while(node != NULL && !channelBelongsToPids(node, senderPid, recipientPid)) {
		node = node->nextChannel;
	}

	return node;
}

int channelBelongsToPids(channelNode_t * channel, int pid1, int pid2) {
	return(channel->senderPid == pid1 && channel->recipientPid == pid2)
	|| (channel->recipientPid == pid1 && channel->senderPid == pid2);
}

void addChannelToList(channelNode_t* newChannel) {
  channelNode_t * next;
  channelNode_t * previous;

  if (firstChannel != NULL) {
    previous = firstChannel;
    next = firstChannel->nextChannel;

    while(next != NULL) {
      previous = next;
      next = next->nextChannel;
    }

    previous->nextChannel = newChannel;
  }
  else
    firstChannel = newChannel;
}

 void removeChannelFromList(channelNode_t* channel) {
	int pid = getpid();
	int thread = getCurrentThread();

	mutexDown(channelMutex, pid, thread);

	channelNode_t * current;
	channelNode_t * previous;

	previous = NULL;
	current = firstChannel;

	while(current != NULL && !channelBelongsToPids(current, channel->senderPid, channel->recipientPid)) {
		previous = current;
		current = current->nextChannel;
	}

	if(current != NULL) {
		if(previous != NULL) {
			previous->nextChannel = current->nextChannel;
		} else
			firstChannel = NULL;

		deleteChannel(current);
	}

	mutexUp(channelMutex, pid, thread);
 }

messageNode_t* addMessageToList(messageNode_t* newMessage, messageNode_t* listHead) {
	if(listHead == NULL) {
		return newMessage;
	}

	listHead->nextMessage = addMessageToList(newMessage, listHead->nextMessage);
	return listHead;
}

messageNode_t* popMessage(messageNode_t** listHead) {
	messageNode_t * aux;
	if (*listHead == NULL)
		return NULL;

	aux = *listHead;
	*listHead = (*listHead)->nextMessage;

	return aux;
}

int addMessageToChannel(messageNode_t* newMessage, channelNode_t* channel, int recipientPid, int senderPid){
	if (channelBelongsToPids(channel, recipientPid, senderPid)) {
		if(channel->recipientPid == recipientPid) {
			channel->senderToRecipient = addMessageToList(newMessage, channel->senderToRecipient);
			return 1;
		} else {
			channel->recipientToSender = addMessageToList(newMessage, channel->recipientToSender);
			return 1;
		}
	}

	return SEND_MESSAGE_ERROR;
}

messageNode_t* getNextMessageForPid(channelNode_t* channel, int pid){
	messageNode_t * aux = NULL;

	if (pid == channel->recipientPid)
		aux = channel->senderToRecipient;
	else
		aux = channel->recipientToSender;

	return popMessage(&aux);
}

channelNode_t * createChannel(int senderPid, int recipientPid){
	//int i;
	int thread = getCurrentThread();

	mutexDown(channelMutex, senderPid, thread);

  channelNode_t* newChannel = getChannelFromList(senderPid, recipientPid);
  if(newChannel != NULL)
    return newChannel;

	newChannel = (channelNode_t*) allocateMemory(sizeof(channelNode_t));
	char senderMutexName[50];
	char recipientMutexName[50];

	if(newChannel != NULL) {
    newChannel->senderPid = senderPid;
    newChannel->recipientPid = recipientPid;
    newChannel->nextChannel = NULL;

    newChannel->senderToRecipient = NULL;
    newChannel->recipientToSender = NULL;

    generateChannelMutexName(senderPid, recipientPid, senderMutexName, 1);
    generateChannelMutexName(recipientPid, senderPid, recipientMutexName, 1);
    newChannel->senderSendSem = createSemaphore(senderPid, senderMutexName, 0);
		newChannel->recipientSendSem = createSemaphore(senderPid, recipientMutexName, 0);

		generateChannelMutexName(senderPid, recipientPid, senderMutexName, 0);
		generateChannelMutexName(recipientPid, senderPid, recipientMutexName, 0);
		newChannel->senderReceiveSem = createSemaphore(senderPid, senderMutexName, 0);
		newChannel->recipientReceiveSem = createSemaphore(senderPid, recipientMutexName, 0);

		/*for(i = 0; i < MAX_MESSAGES; i++) {
			signal(newChannel->senderSendSem);
			signal(newChannel->recipientSendSem);
		}*/
		setValue(newChannel->senderSendSem, MAX_MESSAGES - 1);
		setValue(newChannel->recipientSendSem, MAX_MESSAGES - 1);

    addChannelToList(newChannel);
  }

	mutexUp(channelMutex, senderPid, thread);
	return newChannel;
}

void generateChannelMutexName(int senderPid, int recipientPid, char * mutexName, int send) {
  int senderDigits;
  char * currentSender = mutexName;
  if(send == 1)
		memcpy(currentSender,"chS",3);
  else
		memcpy(currentSender,"chR",3);
  currentSender += 3;
  senderDigits = uintToBase(senderPid,currentSender,10);
  currentSender += senderDigits;
  *currentSender++ = ';';
  uintToBase(recipientPid,currentSender,10);
}

void deleteChannel(channelNode_t * channel) {
  removeChannelFromList(channel);
	deleteMessages(channel->recipientToSender);
	deleteMessages(channel->senderToRecipient);
	releaseSemaphore(channel->senderPid, channel->senderReceiveSem, 0);
	releaseSemaphore(channel->recipientPid, channel->recipientReceiveSem, 0);
	releaseSemaphore(channel->senderPid, channel->senderSendSem , 0);
	releaseSemaphore(channel->recipientPid, channel->recipientReceiveSem, 0);
	freeMemory(channel);
}

void deleteMessages(messageNode_t * start) {
	messageNode_t * current = start;
	while(current != NULL) {
		current = deleteMessage(current);
	}
}

messageNode_t * deleteMessage(messageNode_t * start) {
	messageNode_t * aux;
	freeMemory(start->content);
	aux = start->nextMessage;
	freeMemory(start);
	return aux;
}

int sendMessage(int recipientPid, char* message, int length) {
	int senderPid = getpid();
	int thread = getCurrentThread();
	messageNode_t* newMessage;
	channelNode_t* channel = NULL;
	newMessage = allocateMemory(sizeof(messageNode_t));

	if(newMessage != NULL) {
		newMessage->length = length;
		newMessage->content = allocateMemory(length);
		newMessage->nextMessage = NULL;
		memcpy(newMessage->content, message, length);
		channel = getChannelFromList(recipientPid, senderPid);

		if (channel != NULL) {
			lockSend(senderPid, channel, thread);
			addMessageToChannel(newMessage,channel,recipientPid, senderPid);

			unlockReceive(senderPid, channel);
			return 0;
		} else {
			deleteMessage(newMessage);
		}
	}
	return SEND_MESSAGE_ERROR;
}

char * receiveMessage(int pid, int length) {
	int recipientPid = getpid();
	int thread = getCurrentThread();
	int resultLength;
	messageNode_t *message;
	char * result = NULL;
	channelNode_t* channel = getChannelFromList(recipientPid, pid);
	if(channel != NULL) {
		lockReceive(recipientPid, channel, thread);
		message = getNextMessageForPid(channel, recipientPid);

		resultLength = length > message->length ? message->length: length;
		result = allocateMemory(length);
		memcpy(result, message->content, resultLength);

		deleteMessage(message);
		unlockSend(recipientPid, channel);
	}
	return result;
}

void unlockReceive(int senderPid, channelNode_t * channel) {
	int semaphore;

	if(channel->senderPid == senderPid)
		semaphore = channel->recipientReceiveSem;
	else
		semaphore = channel->senderReceiveSem;

	signal(semaphore);
}

void lockReceive(int recipientPid, channelNode_t * channel, int thread) {
	int semaphore;

	if(channel->recipientPid == recipientPid)
		semaphore = channel->recipientReceiveSem;
	else
		semaphore = channel->senderReceiveSem;

	wait(semaphore, recipientPid, thread);
}

void unlockSend(int recipientPid, channelNode_t * channel) {
	int semaphore;

	if(channel->recipientPid == recipientPid)
		semaphore = channel->senderSendSem;
	else
		semaphore = channel->recipientSendSem;

	signal(semaphore);
}

void lockSend(int senderPid, channelNode_t * channel, int thread) {
	int semaphore;

	if(channel->senderPid == senderPid)
		semaphore = channel->senderSendSem;
	else
		semaphore = channel->recipientSendSem;

	wait(semaphore, senderPid, thread);
}

















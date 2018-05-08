#include <channel.h>

static channelNode_t* firstChannel = NULL;

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

void addMessageToChannel(messageNode_t* newMessage, channelNode_t* channel, int recipientPid, int senderPid){
	if (channelBelongsToPids(channel, recipientPid, senderPid)) {
		if(channel->recipientPid == recipientPid)
			addMessageToList(newMessage, channel->senderToRecipient);
		else
			addMessageToList(newMessage, channel->recipientToSender);
	}
}

messageNode_t* getNextMessageForPid(channelNode_t* channel, int pid){
	messageNode_t * aux = NULL;

	if (pid == channel->recipientPid)	{
		aux = channel->senderToRecipient;
	} else {
		aux = channel->recipientToSender;
	}

	return popMessage(&aux);
}

channelNode_t * createChannel(int senderPid, int recipientPid){
	channelNode_t* newChannel = (channelNode_t*) allocateMemory(sizeof(channelNode_t));
	char senderMutexName[50];
	char recipientMutexName[50];
	if(newChannel != NULL) {
    newChannel->senderPid = senderPid;
    newChannel->recipientPid = recipientPid;
    newChannel->nextChannel = NULL;
    newChannel->senderToRecipient = NULL;
    newChannel->recipientToSender = NULL;
    generateChannelMutexName(senderPid, recipientPid, senderMutexName);
    generateChannelMutexName(recipientPid, senderPid, recipientMutexName);
    newChannel->senderMutex = createMutex(senderPid, senderMutexName);
		newChannel->recipientMutex = createMutex(senderPid, recipientMutexName);
		mutexDown(newChannel->senderMutex, -1);
		mutexDown(newChannel->recipientMutex, -1);
    addChannelToList(newChannel);
  }
	return newChannel;
}

void generateChannelMutexName(int senderPid, int recipientPid, char * mutexName) {
  int senderDigits;
  char * currentSender = mutexName;
  memcpy(currentSender,"ch",2);
  currentSender += 2;
  senderDigits = uintToBase(senderPid,currentSender,10);
  currentSender += senderDigits;
  *currentSender++ = ';';
  uintToBase(recipientPid,currentSender,10);
}

void deleteChannel(channelNode_t * channel) {
	deleteMessages(channel->recipientToSender);
	deleteMessages(channel->senderToRecipient);
	releaseMutex(channel->senderPid, channel->senderMutex);
	releaseMutex(channel->recipientMutex, channel->recipientMutex);
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
	messageNode_t* newMessage;
	channelNode_t* channel;
	newMessage = allocateMemory(sizeof(messageNode_t));
	newMessage->length = length;
	newMessage->content = allocateMemory(length);
	newMessage->nextMessage = NULL;
	memcpy(newMessage->content, message, length);
	channel = getChannelFromList(recipientPid, senderPid);

	if (channel != NULL) {
		addMessageToChannel(newMessage,channel,recipientPid, senderPid);
		unlockRecieve(senderPid, channel);
		return 0;
	}
	return SEND_MESSAGE_ERROR;
}

char * receiveMessage(int pid, int length) {
	int recipientPid = getpid();
	int resultLength;
	messageNode_t *message;
	char * result = NULL;
	channelNode_t* channel = getChannelFromList(recipientPid, pid);
	if(channel != NULL) {
		message = getNextMessageForPid(channel, recipientPid);
		if(message != NULL) {
			resultLength = length > message->length ? message->length: length;
			result = allocateMemory(resultLength);
			memcpy(result, message->content, resultLength);
			if(message->nextMessage == NULL)
				lockRecieve(recipientPid, channel);
			deleteMessage(message);
		}
		else
			lockRecieve(recipientPid, channel);
	}
	return result;
}

void unlockRecieve(int senderPid, channelNode_t * channel) {
	int mutex;
	if(channel->senderPid == senderPid)
		mutex = channel->senderMutex;
	else
		mutex = channel->recipientMutex;

	mutexUp(mutex);
}

void lockRecieve(int recipientPid, channelNode_t * channel) {
	int mutex;
	if(channel->recipientPid == recipientPid)
		mutex = channel->recipientMutex;
	else
		mutex = channel->senderMutex;

	mutexDown(mutex, recipientPid);
}

















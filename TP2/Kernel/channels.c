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

int addMessageToChannel(messageNode_t* newMessage, channelNode_t* channel, int recipientPid, int senderPid){
	if (channelBelongsToPids(channel, recipientPid, senderPid)) {
		if(channel->recipientPid == recipientPid) {
			if(channel->senderToRecipientMessages < MAX_MESSAGES) {
				channel->senderToRecipient = addMessageToList(newMessage, channel->senderToRecipient);
				channel->senderToRecipientMessages++;
				return 1;
			}
		} else {
			if(channel->recipientToSenderMessages < MAX_MESSAGES) {
				channel->recipientToSender = addMessageToList(newMessage, channel->recipientToSender);
				channel->recipientToSenderMessages++;
				return 1;
			}
		}
	}
	return SEND_MESSAGE_ERROR;
}

messageNode_t* getNextMessageForPid(channelNode_t* channel, int pid){
	messageNode_t * aux = NULL;

	if (pid == channel->recipientPid)	{
		aux = channel->senderToRecipient;
		if(aux != NULL) {
			channel->senderToRecipientMessages--;
		}
	} else {
		aux = channel->recipientToSender;
		if(aux != NULL) {
			channel->recipientToSenderMessages--;
		}
	}

	return popMessage(&aux);
}

channelNode_t * createChannel(int senderPid, int recipientPid){
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

		newChannel->senderToRecipientMessages = 0;
		newChannel->recipientToSenderMessages = 0;

    generateChannelMutexName(senderPid, recipientPid, senderMutexName, 1);
    generateChannelMutexName(recipientPid, senderPid, recipientMutexName, 1);
    newChannel->senderSendMutex = createMutex(senderPid, senderMutexName);
		newChannel->recipientSendMutex = createMutex(senderPid, recipientMutexName);

		generateChannelMutexName(senderPid, recipientPid, senderMutexName, 0);
		generateChannelMutexName(recipientPid, senderPid, recipientMutexName, 0);
		newChannel->senderReceiveMutex = createMutex(senderPid, senderMutexName);
		newChannel->recipientReceiveMutex = createMutex(senderPid, recipientMutexName);

    mutexDown(newChannel->recipientReceiveMutex, 0);
    mutexDown(newChannel->senderReceiveMutex, 0);

    addChannelToList(newChannel);
  }
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
	releaseMutex(channel->senderPid, channel->senderReceiveMutex);
	releaseMutex(channel->recipientPid, channel->recipientReceiveMutex);
	releaseMutex(channel->senderPid, channel->senderSendMutex);
	releaseMutex(channel->recipientPid, channel->recipientReceiveMutex);
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
		if(addMessageToChannel(newMessage,channel,recipientPid, senderPid) == SEND_MESSAGE_ERROR) {
			lockSend(senderPid, channel);
			deleteMessage(newMessage);
			return sendMessage(recipientPid, message, length);
		}
		unlockReceive(senderPid, channel);
		return 0;
	}

	deleteMessage(newMessage);
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
		if(message == NULL) {
			lockReceive(recipientPid, channel);
			return receiveMessage(pid, length);
		} else {
			resultLength = length > message->length ? message->length: length;
			result = allocateMemory(length);
			memcpy(result, message->content, resultLength);
			if(message->nextMessage == NULL) {
        lockReceive(recipientPid, channel);
      }
			deleteMessage(message);
			unlockSend(recipientPid, channel);
		}
	}
	return result;
}

void unlockReceive(int senderPid, channelNode_t * channel) {
	int mutex;

	if(channel->senderPid == senderPid)
		mutex = channel->recipientReceiveMutex;
	else
		mutex = channel->senderReceiveMutex;

	mutexUp(mutex);
}

void lockReceive(int recipientPid, channelNode_t * channel) {
	int mutex;

	if(channel->recipientPid == recipientPid)
		mutex = channel->recipientReceiveMutex;
	else
		mutex = channel->senderReceiveMutex;

	mutexDown(mutex, recipientPid);
}

void unlockSend(int recipientPid, channelNode_t * channel) {
	int mutex;

	if(channel->recipientPid == recipientPid)
		mutex = channel->senderSendMutex;
	else
		mutex = channel->recipientSendMutex;

	mutexUp(mutex);
}

void lockSend(int senderPid, channelNode_t * channel) {
	int mutex;

	if(channel->senderPid == senderPid)
		mutex = channel->senderSendMutex;
	else
		mutex = channel->recipientSendMutex;

	mutexDown(mutex, senderPid);
}

















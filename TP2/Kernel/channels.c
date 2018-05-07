#include <channel.h>

static channelNode_t* firstChannel = NULL;
static int nextId = 0;

channelNode_t* getChannelFromList(int recipientPid, channelNode_t* listHead){
	channelNode_t* current = listHead;
	if (listHead == NULL)
		return NULL;
	else if (listHead->senderPid == getpid() && listHead->recipientPid == recipientPid)
		return current;
	else
		return getChannelFromList(recipientPid, current->nextChannel);
}

void addChannelToList(channelNode_t* newChannel, channelNode_t* listHead){
	if (listHead == NULL)
		firstChannel = newChannel;
	else if (listHead->nextChannel == NULL)
		listHead->nextChannel = newChannel;
	else
		addChannelToList(newChannel, listHead->nextChannel);
}

// void removeChannelFromList(channelNode_t* channel, channelNode_t* listHead){
// 	if (listHead == NULL || channel == NULL){
// 		return;
// 	}
// 	else if (listHead->nextMessage == message){
// 		checkpoint();
// 		listHead->nextMessage = message->nextMessage;
// 		free(message->content);
// 		free(message);
// 	} else{
// 		removeMessageFromList(message, listHead->nextMessage);
// 	}
// }

void addMessageToList(messageNode_t* newMessage, messageNode_t* listHead){
	if (listHead->nextMessage == NULL)
		listHead->nextMessage = newMessage;
	else
		addMessageToList(newMessage, listHead->nextMessage);
}

void removeMessageFromList(messageNode_t* message, messageNode_t* listHead){
	if (listHead == NULL || message == NULL){
		return;
	}
	else if (listHead->nextMessage == message){
		listHead->nextMessage = message->nextMessage;
		freeMemory(message->content);
    freeMemory(message);
	} else{
		removeMessageFromList(message, listHead->nextMessage);
	}
}

void addMessageToChannel(messageNode_t* newMessage, channelNode_t* channel, int recipientPid){
	if (recipientPid==channel->recipientPid && getpid()==channel->senderPid)
	{
		if (channel->senderToRecipient == NULL)
			channel->senderToRecipient = newMessage;
		else if (channel->senderToRecipient->nextMessage == NULL)
			channel->senderToRecipient->nextMessage = newMessage;
		else
			addMessageToList(newMessage, channel->senderToRecipient);
	}else if (getpid()==channel->recipientPid && recipientPid==channel->senderPid)
	{
	if (channel->recipientToSender == NULL)
		channel->recipientToSender = newMessage;
	else if (channel->recipientToSender->nextMessage == NULL)
		channel->recipientToSender->nextMessage = newMessage;
	else
		addMessageToList(newMessage, channel->recipientToSender);
	}
}

messageNode_t* getNextMessageForPid(channelNode_t* channel, int pid){
	messageNode_t * aux:
	if (pid==channel->recipientPid)
	{
		if (channel->senderToRecipient == NULL)
			return NULL;
		else 
			aux= channel->senderToRecipient;
			removeMessageFromList(channel->senderToRecipient,channel->senderToRecipient);
			return aux;
	}else{
	if (channel->recipientToSender == NULL)
		return NULL;
	else
		aux= channel->recipientToSender;
		removeMessageFromList(channel->recipientToSender,channel->recipientToSender);
		return aux;
	}
	return NULL;
}

int createChannel(int pid1){
	channelNode_t* newChannel = allocateMemory(sizeof(channelNode_t));
	newChannel->senderPid =getpid();
	newChannel->recipientPid =pid1;
	newChannel->nextChannel = NULL;
	newChannel->senderToRecipient = NULL;
	newChannel->recipientToSender = NULL;
	addChannelToList(newChannel, firstChannel);
	return newChannel->channelId;
}

int sendMessage(int recipientPid, char* message, int length){
	messageNode_t* newMessage;
	channelNode_t* channel;
	newMessage = allocateMemory(sizeof(messageNode_t));
	newMessage->length = length;
	newMessage->content = allocateMemory(length);
	newMessage->nextMessage = NULL;
	memcpy(newMessage->content, message, length);
	channel = getChannelFromList(recipientPid, getpid(), firstChannel);

	if (channel != NULL) {
		addMessageToChannel(newMessage,channel,recipientPid);
		return 0;
	}
	return SEND_MESSAGE_ERROR;
}

messageNode_t* receiveMessage(int pid){
	channelNode_t* channel = getChannelFromList(pid,getpid(), firstChannel);
	messageNode_t* message = getNextMessageForPid(channel, getpid());
	messageNode_t* result = allocateMemory(sizeof(messageNode_t));
	memcpy(result, message, sizeof(messageNode_t));
	result->content = allocateMemory(message->length);
	memcpy(result->content, message->content, message->length);
	if (channel->messages == message){
		channel->messages = NULL;
		freeMemory(message->content);
		freeMemory(message);
	} else {
		removeMessageFromList(message, channel->messages);
	}
	return result;
}

















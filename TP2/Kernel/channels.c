#include <channel.h>

static channelNode_t* firstChannel = NULL;
static int nextId = 0;

channelNode_t* getChannelFromList(int channelId, channelNode_t* listHead){
	channelNode_t* current = listHead;
	if (listHead == NULL)
		return NULL;
	else if (listHead->channelId == channelId)
		return current;
	else
		return getChannelFromList(channelId, current->nextChannel);
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

void addMessageToChannel(messageNode_t* newMessage, channelNode_t* channel){
	if (channel->messages == NULL)
		channel->messages = newMessage;
	else if (channel->messages->nextMessage == NULL)
		channel->messages->nextMessage = newMessage;
	else
		addMessageToList(newMessage, channel->messages);
}

messageNode_t* getNextMessageForPid(messageNode_t* messageList, int pid){
	if (messageList == NULL){
		return NULL;
	} else if (messageList->recipientPid == pid){
		return messageList;
	} else {
		getNextMessageForPid(messageList->nextMessage, pid);
	}
	return NULL;
}

int createChannel(){
	channelNode_t* newChannel = allocateMemory(sizeof(channelNode_t));
	newChannel->channelId = nextId++;
	newChannel->nextChannel = NULL;
	newChannel->messages = NULL;
	addChannelToList(newChannel, firstChannel);
	return newChannel->channelId;
}

int sendMessage(int channelId, int recipientPid, char* message, int length){
	messageNode_t* newMessage;
	channelNode_t* channel;
	newMessage = allocateMemory(sizeof(messageNode_t));
	newMessage->senderPid = getpid();
	newMessage->recipientPid = recipientPid;
	newMessage->length = length;
	newMessage->content = allocateMemory(length);
	newMessage->nextMessage = NULL;
	memcpy(newMessage->content, message, length);
	channel = getChannelFromList(channelId, firstChannel);

	if (channel != NULL) {
		addMessageToChannel(newMessage, channel);
		return 0;
	}
	return SEND_MESSAGE_ERROR;
}

messageNode_t* receiveMessage(int channelId){
	channelNode_t* channel = getChannelFromList(channelId, firstChannel);
	messageNode_t* message = getNextMessageForPid(channel->messages, getpid());
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

















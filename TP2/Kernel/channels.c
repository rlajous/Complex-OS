#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

typedef struct messageNode{
  int senderPid;
  int recipientPid;
  int length;
  char* content;
  struct messageNode* nextMessage;
} messageNode_t;

typedef struct channelNode{
  char* channelId;
  messageNode_t* messages;
  struct channelNode* nextChannel;
} channelNode_t;

channelNode_t* firstChannel = NULL;

void checkpoint(){
	static int id = 0;
	printf("Checkpoint %d\n", id);
	id++;
}

char * intToString(int n){
	char* string;
	int aux = n;
	int numOfDigits = 1;
	int divisor = 1;

	while(aux /= 10)
		numOfDigits++;
	string = malloc(numOfDigits+1);

	aux = n;
	for (int j = 0; j < numOfDigits; j++){
		divisor = 1;
		for (int k = 1; k < numOfDigits-j; k++)
			divisor *= 10;
		string[j] = '0' + (int)(aux/divisor);
		aux -= divisor * (int)(aux/divisor);
	}
	string[numOfDigits] = 0;
	return string;
}

int stringCompare(char* a, char* b){
	int i = 0;
	while(a[i] != 0 || b[i] != 0){
		if (a[i] > b[i])
			return 1;
		else if (a[i] < b[i])
			return -1;
		i++;
	}
	return 0;
}

channelNode_t* getChannelFromList(char* channelId, channelNode_t* listHead){
	channelNode_t* current = listHead;
	if (listHead == NULL)
		return NULL;
	else if (!stringCompare(listHead->channelId, channelId))
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

void addMessageToList(messageNode_t* newMessage, messageNode_t* listHead){
	if (listHead->nextMessage == NULL)
		listHead->nextMessage = newMessage;
	else
		addMessageToList(newMessage, listHead->nextMessage);
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
	if (messageList == NULL)
		return NULL;
	else if (messageList->recipientPid == pid)
		return messageList;
	else
		getNextMessageForPid(messageList->nextMessage, pid);
	return NULL;
}

char* createChannel(){
	static int channelId = 0;
	channelId++;
	channelNode_t* newChannel = malloc(sizeof(channelNode_t));
	newChannel->channelId = intToString(channelId);
	newChannel->nextChannel = NULL;
	addChannelToList(newChannel, firstChannel);
	return intToString(channelId);
}

void sendMessage(char* channelId, int recipientPid, char* message, int length){
	messageNode_t* newMessage;
	channelNode_t* channel;
	newMessage = malloc(sizeof(messageNode_t));
	newMessage->senderPid = getpid();
	newMessage->length = length;
	newMessage->content = malloc(length+1);
	memcpy(newMessage->content, message, length+1);
	channel = getChannelFromList(channelId, firstChannel);
	if (channel != NULL)
		addMessageToChannel(newMessage, channel);
	else
		printf("Channel not found. Fucking piece of shit motherfucker.\n");
}

messageNode_t* receiveMessage(char* channelId){
	channelNode_t* channel = getChannelFromList(channelId, firstChannel);
	messageNode_t* messages = channel->messages;
	messageNode_t* result = getNextMessageForPid(messages, getpid());
	return result;
}


// int main(int argc, char** argv){
// 	char* channelId;
// 	channelId = createChannel();
// 	printf("\n");
// 	sendMessage(channelId, getpid(), "holaaaa", strlen("holaaaa")+1);
// 	printf("Alles gut on zero: %d\n", stringCompare("hola", "hola"));
// }

















#include "channelsTest.h"
#include "assert.h"

//channelNode_t* firstChannel = NULL;
char* channelId;
char* testMessage;
int incorrectPid;
messageNode_t* receivedMessage;

int main() {
	testChannelCreation();

	testSendMessages();

	testReceiveMessages();

	freeResources();
}

void testChannelCreation() {
	printf("Test Channel creation...........");

	whenAChannelIsCreatedOnEmptyList();

	thenCheckListNotNull();

	thenCheckTheChannelCanBeFoundWithID();

	thenSuccess();
}

void testSendMessages(){
	printf("Test Send...........");

	givenAMessage();

	whenTheMessageIsSent();

	thenCheckMessageIsInList();

	thenSuccess();
}

void testReceiveMessages(){
	printf("Test Receive...........");

	whenTheMessageIsReceived();

	thenCheckReceivedMessageIsNotNull();

	thenCheckMessageContentIsCorrect();

	thenCheckMessageIsNoLongerInList();

	thenSuccess();
}

void whenTheMessageIsReceived(){
	receivedMessage = receiveMessage(channelId);
}

void thenCheckReceivedMessageIsNotNull(){
	assert(receivedMessage != NULL);
}

void thenCheckMessageContentIsCorrect(){
	assert(stringCompare(testMessage, receivedMessage->content) == 0);
}

void thenCheckMessageIsNoLongerInList(){
	assert(getNextMessageForPid(getChannelFromList(channelId, firstChannel)->messages, getpid()) == NULL);
}


void givenAMessage(){
	testMessage = "I don't feel so good Mr Stark :s";
}

void whenTheMessageIsSent(){
	sendMessage(channelId, getpid(), testMessage, strlen(testMessage) + 1);
}

void thenCheckMessageIsInList(){
	assert(getNextMessageForPid(getChannelFromList(channelId, firstChannel)->messages, getpid()) != NULL);
}


void whenAChannelIsCreatedOnEmptyList(){
	channelId = createChannel();
}

void thenCheckListNotNull(){
	assert(firstChannel != NULL);
}

void thenCheckTheChannelCanBeFoundWithID(){
	assert(getChannelFromList(channelId, firstChannel)->channelId = channelId);
}

void freeResources(){
	free(channelId);
	free(receivedMessage);
	free(firstChannel);
}

void thenSuccess(){
	printf("OK\n");
}
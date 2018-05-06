#include "channelsTest.h"
#include "assert.h"

char* channelId;
char* testMessage;
int incorrectPid;
message_t* receivedMessage;

int main() {
	testChannelCreation();

	testSendMessages();

	testReceiveMessages();
}

void testChannelCreation() {
	printf("Test Channels...........");

	whenAChannelIsCreatedOnEmptyList();

	thenCheckListNotNull();

	thenCheckTheChannelCanBeFoundWithID();

	thenSuccess();
}

void testSendMessages(){
	printf("Test Send...........");

	givenAMessage();

	whenTheMessageIsSent():

	thenCheckMessageIsInList();

	thenSuccess();
}

void testReceiveMessages(){
	printf("Test Receive...........");

	whenTheMessageIsReceived():

	thenCheckReceivedMessageIsNotNull();

	thenCheckMessageIsNoLongerInList();

	thenSuccess();
}

void whenTheMessageIsReceived(){
	receivedMessage = receiveMessage(channelId);
}

void thenCheckReceivedMessageIsNotNull(){

}

void thenCheckMessageIsNoLongerInList(){

}


void givenAMessage(){
	testMessage = "I don't feel so goos Mr Stark :s";
}

void whenTheMessageIsSent(){
	sendMessage(channelId, getpid(), testMessage, strlen(testMessage));
}

void thenCheckMessageIsInList(){
	assert(getNextMessageForPid(getChannelFromList(channelId, firstChannel)->messages, int pid) != NULL);
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

void thenSuccess(){
	printf("OK\n");
}
#include "channels.c"

void testChannelCreation();
void testSendMessages();
void testReceiveMessages();
void whenTheMessageIsReceived();
void thenCheckReceivedMessageIsNotNull();
void thenCheckMessageContentIsCorrect();
void thenCheckMessageIsNoLongerInList();
void givenAMessage();
void whenTheMessageIsSent();
void thenCheckMessageIsInList();
void whenAChannelIsCreatedOnEmptyList();
void thenCheckListNotNull();
void thenCheckTheChannelCanBeFoundWithID();
void freeResources();
void thenSuccess();
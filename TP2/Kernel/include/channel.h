#ifndef CHANNEL_H
#define CHANNEL_H

#include <buddyMemoryAllocator.h>
#include <scheduler.h>

#define SEND_MESSAGE_ERROR -1

typedef struct messageNode{
    int length;
    char* content;
    struct messageNode* nextMessage;
} messageNode_t;

typedef struct channelNode{
    int senderPid;
    int recipientPid;
    messageNode_t* senderToRecipient;
    messageNode_t* recipientToSender;
    struct channelNode* nextChannel;
} channelNode_t;

channelNode_t* getChannelFromList(int recipientPid, channelNode_t* listHead);

void addChannelToList(channelNode_t* newChannel, channelNode_t* listHead);

void removeChannelFromList(channelNode_t* channel, channelNode_t* listHead);

void addMessageToList(messageNode_t* newMessage, messageNode_t* listHead);

void removeMessageFromList(messageNode_t* message, messageNode_t* listHead)

void addMessageToChannel(messageNode_t* newMessage, channelNode_t* channel, int recipientPid);

messageNode_t* getNextMessageForPid(channelNode_t* channel, int pid);

int createChannel(int pid1);

int sendMessage(int recipientPid, char* message, int length);

messageNode_t* receiveMessage(int pid);

#endif

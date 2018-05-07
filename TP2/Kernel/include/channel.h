#ifndef CHANNEL_H
#define CHANNEL_H

#include <buddyMemoryAllocator.h>
#include <scheduler.h>

#define SEND_MESSAGE_ERROR -1

typedef struct messageNode{
    int senderPid;
    int recipientPid;
    int length;
    char* content;
    struct messageNode* nextMessage;
} messageNode_t;

typedef struct channelNode{
    int channelId;
    messageNode_t* messages;
    struct channelNode* nextChannel;
} channelNode_t;

channelNode_t* getChannelFromList(int channelId, channelNode_t* listHead);

void addChannelToList(channelNode_t* newChannel, channelNode_t* listHead);

void removeChannelFromList(channelNode_t* channel, channelNode_t* listHead);

void addMessageToList(messageNode_t* newMessage, messageNode_t* listHead);

void removeMessageFromList(messageNode_t* message, messageNode_t* listHead);

void addMessageToChannel(messageNode_t* newMessage, channelNode_t* channel);

messageNode_t* getNextMessageForPid(messageNode_t* messageList, int pid);

int createChannel();

int sendMessage(int channelId, int recipientPid, char* message, int length);

messageNode_t* receiveMessage(int channelId);

#endif

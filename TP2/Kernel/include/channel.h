#ifndef CHANNEL_H
#define CHANNEL_H

#include <buddyMemoryAllocator.h>
#include <scheduler.h>
#include <mutex.h>

#define SEND_MESSAGE_ERROR -1

typedef struct messageNode{
    int length;
    char* content;
    struct messageNode* nextMessage;
} messageNode_t;

typedef struct channelNode{
    int senderPid;
    int recipientPid;
    int senderMutex;
    int recipientMutex;
    messageNode_t* senderToRecipient;
    messageNode_t* recipientToSender;
    struct channelNode* nextChannel;
} channelNode_t;

channelNode_t* getChannelFromList(int recipientPid, int senderPid);

int channelBelongsToPids(channelNode_t * channel, int pid1, int pid2);

void addChannelToList(channelNode_t* newChannel);

void removeChannelFromList(channelNode_t* channel);

messageNode_t* addMessageToList(messageNode_t* newMessage, messageNode_t* listHead);

messageNode_t* popMessage(messageNode_t** listHead);

void addMessageToChannel(messageNode_t* newMessage, channelNode_t* channel, int recipientPid, int senderPid);

messageNode_t* getNextMessageForPid(channelNode_t* channel, int pid);

channelNode_t * createChannel(int senderPid, int recipientPid);

void generateChannelMutexName(int senderPid, int recipientPid, char * mutexName);

void deleteChannel(channelNode_t * channel);

void deleteMessages(messageNode_t * start);

messageNode_t * deleteMessage(messageNode_t * start);

int sendMessage(int recipientPid, char* message, int length);

char * receiveMessage(int pid, int length);

void unlockRecieve(int senderPid, channelNode_t * channel);

void lockRecieve(int recipientPid, channelNode_t * channel);

#endif

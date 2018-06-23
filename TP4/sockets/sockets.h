#ifndef SOCKETS_H
#define SOCKETS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_CONNECTIONS 5
#define SOCKET_BIND_ERROR -1
#define SOCKET_LISTEN_ERROR -2
#define SOCKET_CONNECTION_ERROR -3

typedef struct {
	int fileDescriptor;
	struct sockaddr_in address;
} socket_t;

socket_t * openSocket(char* host, int port);

int bindSocket(socket_t * sock);

int acceptSocket(socket_t * sock);

void closeConnection(int fileDescriptor);

int connectSocket(socket_t * sock);

int socketRecieve(int connection, char * buffer, size_t length);

int socketSend(int connection, char * buffer, size_t length);

#endif

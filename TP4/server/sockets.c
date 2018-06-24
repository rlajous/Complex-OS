#include "sockets.h"

//REFERENCES http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
socket_t * openSocket(char* host, int port) {
	int fileDescriptor;
	struct hostent * server;
	socket_t * sock = NULL;

	fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(fileDescriptor < 0){
		return NULL;
	}

	sock = (socket_t *) malloc(sizeof(socket_t));
	if(sock == NULL)
		return NULL;

	sock->fileDescriptor = fileDescriptor;

	memset((char *) &sock->address, 0, sizeof(sock->address));

  sock->address.sin_family = AF_INET;
  sock->address.sin_port = htons(port);

	if(host == NULL){
		sock->address.sin_addr.s_addr = INADDR_ANY;
	} else {
		server = gethostbyname(host);
		if(server == NULL) {
			free(sock);
			return NULL;
		}
		//Sets socket address fields as server's
	  memcpy(&sock->address.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
	}

  return sock;
}

int bindSocket(socket_t * sock) {
    if(bind(sock->fileDescriptor, (struct sockaddr *) &sock->address, sizeof(sock->address)) < 0) {
    	return SOCKET_BIND_ERROR;
    }

    if(listen(sock->fileDescriptor, MAX_CONNECTIONS) < 0) {
    	return SOCKET_LISTEN_ERROR;
    }

    return 0;
}

int acceptSocket(socket_t * sock) {
	int connection;
	connection = accept(sock->fileDescriptor, NULL, 0);

	return connection;
}

void closeConnection(int fileDescriptor) {
	close(fileDescriptor);
}

int connectSocket(socket_t * sock) {
	int connection;

	connection = socket(AF_INET, SOCK_STREAM, 0);

	if(connect(connection, (struct sockaddr *) &sock->address, sizeof(sock->address)) < 0){
		return SOCKET_CONNECTION_ERROR;
	}

	return connection;
}

int socketRecieve(int connection, char * buffer, size_t length) {
 	int r = read(connection, buffer, length);
 	return r;
}

int socketSend(int connection, char * buffer, size_t length) {
	return write(connection, buffer, length);
}

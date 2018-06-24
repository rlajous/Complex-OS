#include "sockets.h"
int main(void) {
	int connection = 0;
	socket_t* socket;
	socket = openSocket("movies", 1337);
	bindSocket(socket);
	connection = acceptSocket(socket);
	socketSend(connection, "Heyy", 5);
	return 0;
}
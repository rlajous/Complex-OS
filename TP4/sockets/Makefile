include ../Makefile.inc

SOCKET_SOURCES = sockets.c

all: $(SOCKET)

$(SOCKET): $(SOCKET_SOURCES)
	$(COMPILER) $(CFLAGS) -c $(SOCKET_SOURCES) -o $(SOCKET)

clean:
	rm -rf *.o

.PHONY: all clean

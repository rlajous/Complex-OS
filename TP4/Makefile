include Makefile.inc

all: socket server client

socket:
	cd sockets; make all

server:
	cd server; make all

client:
	cd client; make all

clean:
	cd client; make clean
	cd server; make clean
	cd sockets; make clean
	rm -rf *.out

.PHONY: all clean socket server client

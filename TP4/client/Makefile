include ../Makefile.inc

CLIENT_SOURCES = client.c

all: $(CLIENT)

$(CLIENT): $(CLIENT_SOURCES)
	$(COMPILER) $(CFLAGS) $(CLIENT_SOURCES) -o $(CLIENT)

clean:
	rm -rf *.o

.PHONY: all clean

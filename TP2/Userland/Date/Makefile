include ../Makefile.inc

MODULE=0005-date.bin
SOURCES=$(wildcard [^_]*.c)
LIBC=$(wildcard ../Libc/*.o)
LOADER=./module.ld ../Loader/_loader.c

all: $(MODULE)

$(MODULE): $(SOURCES) $(LOADER)
	$(GCC) $(GCCFLAGS) -I../Libc/include -T $(LOADER) $(SOURCES) $(LIBC) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print

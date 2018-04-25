include ../Makefile.inc

MODULE=0002-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
LOADER=../Loader/module.ld ../Loader/_loader.c

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T $(LOADER) $(SOURCES) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print

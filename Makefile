GCC=gcc
FLAGS=-Wall

SLAVE=slave.o
SLAVE_SOURCES=slave.c

APPLICATION=application.out
APPLICATION_SOURCES=application.c

all: $(SLAVE) $(APPLICATION)

$(SLAVE): $(SLAVE_SOURCES)
	$(GCC) $(FLAGS) $(SLAVE_SOURCES) -o $(SLAVE)

$(APPLICATION): $(APPLICATION_SOURCES)
	$(GCC) $(FLAGS) $(APPLICATION_SOURCES) -o $(APPLICATION)

clean:
	rm -rf *.o $(APPLICATION)

.PHONY: all clean

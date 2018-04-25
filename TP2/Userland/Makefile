include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin
MODULES:= $(filter-out Libc/.,$(wildcard */.))

all: libc sampleDataModule modules 

libc:
	cd Libc; make

modules:
	  for dir in $(MODULES); do \
	      $(MAKE) -C $$dir; \
	  done

sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd Libc; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule all clean

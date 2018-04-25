#ifndef TYPES_H
#define TYPES_H

typedef struct {
	char * name;
	void * address;
	int size;
} module_t;

typedef struct {
	char ch;
	char style;
} cell_t;

#endif

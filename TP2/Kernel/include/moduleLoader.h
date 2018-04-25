#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <types.h>

void loadModules(void * payloadStart, module_t * modules, void * dropStart);

#endif

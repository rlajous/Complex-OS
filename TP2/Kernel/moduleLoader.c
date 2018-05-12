#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>

static void loadModule(uint8_t ** module, module_t * moduleStruct, void ** lastModule);
static uint32_t readUint32(uint8_t ** address);

void loadModules(void * payloadStart, module_t * modules, void * dropStart)
{
	int i;
	uint8_t * currentModule = (uint8_t*)payloadStart;
	uint32_t moduleCount = readUint32(&currentModule);
	void * lastModule = dropStart;

	for (i = 0; i < moduleCount; i++)
		loadModule(&currentModule, &modules[i], &lastModule);
}

static void loadModule(uint8_t ** module, module_t * moduleStruct, void ** lastModule)
{
	uint32_t moduleSize = readUint32(module);
	moduleStruct->address = *lastModule;
	*lastModule += 0x0100000/*moduleSize*/;
	ncPrintHex((uint64_t)*lastModule);
	ncPrint("  Will copy module at 0x");
	ncPrintHex((uint64_t)*module);
	ncPrint(" to 0x");
	ncPrintHex((uint64_t)moduleStruct->address);
	ncPrint(" (");
	ncPrintDec(moduleSize);
	ncPrint(" bytes)");

	moduleStruct->size = moduleSize;
	memcpy(moduleStruct->address, *module, moduleSize);
	*module += moduleSize;

	ncPrint(" [Done]");
	ncNewline();
}

static uint32_t readUint32(uint8_t ** address)
{
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}

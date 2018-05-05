#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

#include <stdint.h>

void iSetHandler(int index, uint64_t handler);
void sendEOI(int irq);
void setupIDT();
void irqDispatcher(int irq);
void * tickHandler(void * rsp);

extern void irq0Handler();
extern void irq1Handler();
extern void int80Handler();
extern void sti();
extern void cli();
extern void setPicMaster(uint8_t mask);
extern void setPicSlave(uint8_t mask);

#endif

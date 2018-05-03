GLOBAL sti
GLOBAL cli
GLOBAL irq0Handler
GLOBAL irq1Handler
GLOBAL setPicMaster
GLOBAL setPicSlave
GLOBAL int80Handler

%include "./asm/macros.m"

EXTERN irqDispatcher
EXTERN tickHandler
EXTERN sysCallHandler
EXTERN sendEOI
EXTERN runModule

section .text

irq0Handler:
	pushaq

	mov rdi, rsp
	call tickHandler
	mov rsp, rax

	call sendEOI
	popaq
	iretq

irq1Handler:
	irqHandler 1

int80Handler:
	pushaq

	call sysCallHandler

	popaq
	iretq

sti:
	sti
	ret

cli:
	cli
	ret

setPicMaster:
	setPicMask 0x21

setPicSlave:
	setPicMask 0xA1

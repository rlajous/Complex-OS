GLOBAL sti
GLOBAL cli
GLOBAL irq0Handler
GLOBAL irq1Handler
GLOBAL setPicMaster
GLOBAL setPicSlave
GLOBAL int80Handler
GLOBAL yield
GLOBAL yieldHandler

%include "./asm/macros.m"

EXTERN irqDispatcher
EXTERN tickHandler
EXTERN sysCallHandler
EXTERN sendEOI
EXTERN runModule
EXTERN switchToKernelStack
EXTERN nextProcess

section .text

irq0Handler:
	pushaq

	mov rdi, rsp
	call switchToKernelStack
	mov rsp, rax

	call tickHandler
	mov rsp, rax

    mov rdi, 0

	call sendEOI
	popaq
	iretq

irq1Handler:
	irqHandler 1

int80Handler:
	pushaq

	call sysCallHandler

	mov [brax], rax

	popaq

	mov rax, [brax]
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

yield:
    int 40h
    ret

yieldHandler:
    pushaq

    mov rdi, rsp
    call switchToKernelStack
    mov rsp, rax

    call nextProcess
    mov rsp, rax

    mov rdi, 0
    call sendEOI
    popaq
    iretq

section .bss
brax    resb 8


%macro pushaq 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popaq 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro setPicMask 1
	push rbp
	mov rbp, rsp

	mov rax, rdi
	out %1, al

	mov rsp, rbp
	pop rbp
	ret
%endmacro


%macro irqHandler 1
	pushaq

	mov rdi, %1
	call irqDispatcher

	call sendEOI

	popaq

	iretq
%endmacro

GLOBAL cpuVendor
GLOBAL readPort
GLOBAL writePort
GLOBAL halt
GLOBAL startFirstProcess
GLOBAL testAndSetLock

EXTERN getCurrentStack
EXTERN getEntryPoint

%include "./asm/macros.m"

section .text

readPort:	;(Recibe el puerto a leer en rdi)
	push rbp
	mov rbp, rsp

	mov rdx, rdi
	in al, dx

	mov rsp, rbp
	pop rbp
	ret


writePort: ;(Recibe el puerto a escribir en rdi y en rsi lo que hay que escribir)
	push rbp
	mov rbp, rsp

	mov rax, rsi
	mov rdx, rdi
	out dx, al

	mov rsp, rbp
	pop rbp
	ret

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

halt:
    push rbp
    mov rbp, rsp

    hlt

    mov rsp, rbp
    pop rbp
    ret

startFirstProcess:
    call getCurrentStack
	mov rsp, rax

	call getEntryPoint

    push rax
    sti
    ret

;testAndSetLock(int * mutexLock)
testAndSetLock:
    push rbp
    mov rbp, rsp

    mov rax, 0
    mov rdx, 1

    lock CMPXCHG [rdi], rdx

    jnz .notAcquired

    mov rax, 1
    jmp .ret

.notAcquired:
    mov rax, 0
    jmp .ret

.ret:
    mov rsp, rbp
    pop rbp
    ret

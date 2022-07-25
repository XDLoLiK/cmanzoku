; test2.man.asm

global _start

section .text

_start:
		call main
		mov rdi, rax
		mov rax, 0x3c
		syscall

main:
		; preamble
		push rbp
		mov rbp, rsp
		sub rsp, 8 * 1
		; x
		; number
		push qword 500
		pop rax
		mov qword [rbp - 8 * 1], rax
		; x
		push qword [rbp - 8 * 1]
		; ret
		pop rax
		add rsp, 8 * 1
		pop rbp
		ret
		add rsp, 8 * 1

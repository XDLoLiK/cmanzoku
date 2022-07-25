; Examples/test.man.asm

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
		; num
		; number
		push qword 500
		pop rax
		mov qword [rbp - 8 * 1], rax
		; factorial_recursive call
		; num
		push qword [rbp - 8 * 1]
		call factorial_recursive
		add rsp, 8 * 1
		push rax
		add rsp, 8 * 1
		; factorial_iterative call
		; num
		push qword [rbp - 8 * 1]
		call factorial_iterative
		add rsp, 8 * 1
		push rax
		add rsp, 8 * 1
		; number
		push qword 0
		; ret
		pop rax
		add rsp, 8 * 1
		pop rbp
		ret
		add rsp, 8 * 1

factorial_recursive:
		; preamble
		push rbp
		mov rbp, rsp
		sub rsp, 8 * 1
		; num
		mov rax, [rbp + 8 * 2]
		mov qword [rbp - 8 * 1], rax
		; if condition
		; num
		push qword [rbp - 8 * 1]
		; number
		push qword 100
		; <=
		pop rbx
		pop rax
		cmp rax, rbx
		jle .L1
		push qword 0
		jmp .L2
.L1:
		push qword 100
.L2:
		pop rax
		cmp rax, 0
		je .L0
		; if body
		; number
		push qword 100
		; ret
		pop rax
		add rsp, 8 * 1
		pop rbp
		ret
		add rsp, 8 * 1
.L0:
		; num
		push qword [rbp - 8 * 1]
		; factorial_recursive call
		; num
		push qword [rbp - 8 * 1]
		; number
		push qword 100
		; -
		pop rbx
		pop rax
		sub rax, rbx
		push rax
		call factorial_recursive
		add rsp, 8 * 1
		push rax
		; *
		pop rbx
		pop rax
		imul rax, rbx
		mov rcx, 100
		idiv rcx
		push rax
		; ret
		pop rax
		add rsp, 8 * 1
		pop rbp
		ret
		add rsp, 8 * 1
		; number
		push qword 400
		add rsp, 8 * 1

factorial_iterative:
		; preamble
		push rbp
		mov rbp, rsp
		sub rsp, 8 * 2
		; num
		mov rax, [rbp + 8 * 2]
		mov qword [rbp - 8 * 1], rax
		; ans
		; number
		push qword 100
		pop rax
		mov qword [rbp - 8 * 2], rax
.L3:
		; while condition
		; num
		push qword [rbp - 8 * 1]
		; number
		push qword 100
		; >
		pop rbx
		pop rax
		cmp rax, rbx
		jg .L5
		push qword 0
		jmp .L6
.L5:
		push qword 100
.L6:
		pop rax
		cmp rax, 0
		je .L4
		; while body
		; ans
		push qword [rbp - 8 * 2]
		; ans
		push qword [rbp - 8 * 2]
		; num
		push qword [rbp - 8 * 1]
		; *
		pop rbx
		pop rax
		imul rax, rbx
		mov rcx, 100
		idiv rcx
		push rax
		; =
		pop rax
		pop rbx
		mov [rbp - 8 * 2], rax
		push rax
		add rsp, 8 * 1
		; num
		push qword [rbp - 8 * 1]
		; num
		push qword [rbp - 8 * 1]
		; number
		push qword 100
		; -
		pop rbx
		pop rax
		sub rax, rbx
		push rax
		; =
		pop rax
		pop rbx
		mov [rbp - 8 * 1], rax
		push rax
		add rsp, 8 * 1
		jmp .L3
.L4:
		; ans
		push qword [rbp - 8 * 2]
		; ret
		pop rax
		add rsp, 8 * 2
		pop rbp
		ret
		add rsp, 8 * 1

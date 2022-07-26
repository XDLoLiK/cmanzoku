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
		sub rsp, 8
		; num
		; number
		push qword 500
		pop rax
		mov qword [rbp - 8], rax
		; factorial_recursive call
		; num
		push qword [rbp - 8]
		call factorial_recursive
		add rsp, 8
		push rax
		add rsp, 8
		; factorial_iterative call
		; num
		push qword [rbp - 8]
		call factorial_iterative
		add rsp, 8
		push rax
		add rsp, 8
		; number
		push qword 0
		; ret
		pop rax
		add rsp, 8
		pop rbp
		ret
		; postamble
		add rsp, 8
		pop rbp
		ret

factorial_recursive:
		; preamble
		push rbp
		mov rbp, rsp
		sub rsp, 8
		; num
		mov rax, [rbp + 16]
		mov qword [rbp - 8], rax
		; if condition
		; num
		push qword [rbp - 8]
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
		add rsp, 8
		pop rbp
		ret
.L0:
		; num
		push qword [rbp - 8]
		; factorial_recursive call
		; num
		push qword [rbp - 8]
		; number
		push qword 100
		; -
		pop rbx
		pop rax
		sub rax, rbx
		push rax
		call factorial_recursive
		add rsp, 8
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
		add rsp, 8
		pop rbp
		ret
		; postamble
		add rsp, 8
		pop rbp
		ret

factorial_iterative:
		; preamble
		push rbp
		mov rbp, rsp
		sub rsp, 16
		; num
		mov rax, [rbp + 16]
		mov qword [rbp - 8], rax
		; ans
		; number
		push qword 100
		pop rax
		mov qword [rbp - 16], rax
.L3:
		; while condition
		; num
		push qword [rbp - 8]
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
		push qword [rbp - 16]
		; num
		push qword [rbp - 8]
		; =
		pop rbx
		pop rax
		imul rax, rbx
		mov rcx, 100
		idiv rcx
		mov [rbp - 16], rax
		push rax
		add rsp, 8
		; num
		push qword [rbp - 8]
		; number
		push qword 100
		; =
		pop rbx
		pop rax
		sub rax, rbx
		mov [rbp - 8], rax
		push rax
		add rsp, 8
		jmp .L3
.L4:
		; ans
		push qword [rbp - 16]
		; ret
		pop rax
		add rsp, 16
		pop rbp
		ret
		; postamble
		add rsp, 16
		pop rbp
		ret

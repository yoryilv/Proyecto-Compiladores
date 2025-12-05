.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl factorial
factorial:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movl %edi, -4(%rbp)
 movslq -4(%rbp), %rax
 pushq %rax
 movl $1, %eax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_0
 movl $1, %eax
 jmp .end_factorial
 jmp endif_0
 else_0:
endif_0:
 movslq -4(%rbp), %rax
 pushq %rax
 movslq -4(%rbp), %rax
 pushq %rax
 movl $1, %eax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movq %rax, %rdi
 call factorial
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 jmp .end_factorial
.end_factorial:
    leave
    ret
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
 movl $5, %eax
 movq %rax, %rdi
 call factorial
 movl $5, %eax
 movq %rax, %rdi
 call factorial
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_1: .string "%d\n"
.section .note.GNU-stack,"",@progbits

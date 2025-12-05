.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 movl $10, %eax
 movl %eax, -4(%rbp)
 movl $2, %eax
 movl %eax, -8(%rbp)
 movl $5, %eax
 movl %eax, -12(%rbp)
 # Sethi-Ullman swap
 movslq -8(%rbp), %rax
 pushq %rax
 movslq -12(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 pushq %rax
 movslq -4(%rbp), %rax
 popq %rcx
 addq %rcx, %rax
 pushq %rax
 movslq -4(%rbp), %rax
 pushq %rax
 movslq -8(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 cqo
 idivq %rcx
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movl %eax, -16(%rbp)
 movslq -16(%rbp), %rax
 movslq -16(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_0(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%d\n"
.section .note.GNU-stack,"",@progbits

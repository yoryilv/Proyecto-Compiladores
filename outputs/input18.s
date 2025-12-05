.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 movl $10, %eax
 movl %eax, -4(%rbp)
 movl $2, %eax
 movl %eax, -8(%rbp)
 movl $20, %eax
 movl %eax, -12(%rbp)
 movl $5, %eax
 movl %eax, -16(%rbp)
 # Sethi-Ullman swap
 # Sethi-Ullman swap
 movslq -12(%rbp), %rax
 pushq %rax
 movslq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 pushq %rax
 movslq -8(%rbp), %rax
 popq %rcx
 imulq %rcx, %rax
 pushq %rax
 movslq -4(%rbp), %rax
 popq %rcx
 addq %rcx, %rax
 movl %eax, -20(%rbp)
 movslq -20(%rbp), %rax
 movslq -20(%rbp), %rax
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

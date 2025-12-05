.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 # Sethi-Ullman swap
 # constant folding (Pre-calculated)
 movl $12, %eax
 pushq %rax
 movl $2, %eax
 popq %rcx
 addq %rcx, %rax
 movl %eax, -4(%rbp)
 movslq -4(%rbp), %rax
 movslq -4(%rbp), %rax
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

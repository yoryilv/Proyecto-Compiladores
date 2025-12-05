.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 movl $1, %eax
 movl %eax, -4(%rbp)
 movslq -4(%rbp), %rax
 movslq -4(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_0(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movl $0, %eax
 jmp .end_main
 movl $100, %eax
 movl %eax, -4(%rbp)
 movslq -4(%rbp), %rax
 movslq -4(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%d\n"
.LC_STR_1: .string "%d\n"
.section .note.GNU-stack,"",@progbits

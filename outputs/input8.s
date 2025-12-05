.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 movq $2147483648, %rax
 movq %rax, -8(%rbp)
 movl $10, %eax
 movl %eax, -12(%rbp)
 movq -8(%rbp), %rax
 movq -8(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_0(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movl -12(%rbp), %eax
 pushq %rax
 movl $5, %eax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 seta %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_1
 movl $1, %eax
 movl $1, %eax
 movq %rax, %rsi
 leaq .LC_STR_2(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 jmp endif_1
 else_1:
endif_1:
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%ld\n"
.LC_STR_2: .string "%d\n"
.section .note.GNU-stack,"",@progbits

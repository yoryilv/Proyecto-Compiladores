.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 # constant folding (Pre-calculated)
 movl $30, %eax
 movl %eax, -4(%rbp)
 # constant folding (Pre-calculated)
 movl $10, %eax
 pushq %rax
 # constant folding (Pre-calculated)
 movl $25, %eax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movl %eax, -8(%rbp)
 movslq -4(%rbp), %rax
 movslq -4(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_0(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movslq -8(%rbp), %rax
 movslq -8(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 # constant folding (Pre-calculated)
 movl $1, %eax
 cmpq $0, %rax
 je else_2
 movl $1, %eax
 movl $1, %eax
 movq %rax, %rsi
 leaq .LC_STR_3(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 jmp endif_2
 else_2:
 movl $0, %eax
 movl $0, %eax
 movq %rax, %rsi
 leaq .LC_STR_4(%rip), %rdi
 movl $0, %eax
 call printf@PLT
endif_2:
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%d\n"
.LC_STR_1: .string "%d\n"
.LC_STR_3: .string "%d\n"
.LC_STR_4: .string "%d\n"
.section .note.GNU-stack,"",@progbits

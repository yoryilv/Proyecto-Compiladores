.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 movl $1078530000, %eax
 movd %eax, %xmm0
 movsd %xmm0, -8(%rbp)
 movsd -8(%rbp), %xmm0
 subq $8, %rsp
 movsd %xmm0, (%rsp)
 movl $1077936128, %eax
 movd %eax, %xmm0
 # OPERACIÓN FLOTANTE
 movsd %xmm0, %xmm1
 movsd (%rsp), %xmm0
 addq $8, %rsp
 ucomisd %xmm1, %xmm0
 movl $0, %eax
 seta %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_0
 movl $1, %eax
 movl $1, %eax
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 jmp endif_0
 else_0:
 movl $0, %eax
 movl $0, %eax
 movq %rax, %rsi
 leaq .LC_STR_2(%rip), %rdi
 movl $0, %eax
 call printf@PLT
endif_0:
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_1: .string "%d\n"
.LC_STR_2: .string "%d\n"
.section .note.GNU-stack,"",@progbits

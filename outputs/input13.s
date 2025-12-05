.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 movl $1093140480, %eax
 movd %eax, %xmm0
 movsd %xmm0, -24(%rbp)
 movl $1094189056, %eax
 movd %eax, %xmm0
 movsd %xmm0, -16(%rbp)
 movl $1094713344, %eax
 movd %eax, %xmm0
 movsd %xmm0, -8(%rbp)
 movl $0, %eax
 movd %eax, %xmm0
 movsd %xmm0, -32(%rbp)
 movl $0, %eax
 movl %eax, -36(%rbp)
for_start_0:
 movslq -36(%rbp), %rax
 pushq %rax
 movl $3, %eax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
 cmpq $0, %rax
 je for_end_0
 movsd -32(%rbp), %xmm0
 subq $8, %rsp
 movsd %xmm0, (%rsp)
 movq $0, %r11
 imulq $3, %r11
 movslq -36(%rbp), %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -24(%rbp), %rax
 addq %r11, %rax
 movsd (%rax), %xmm0
 # OPERACIÓN FLOTANTE
 movsd %xmm0, %xmm1
 movsd (%rsp), %xmm0
 addq $8, %rsp
 addsd %xmm1, %xmm0
 movsd %xmm0, -32(%rbp)
 movslq -36(%rbp), %rax
 pushq %rax
 movl $1, %eax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movl %eax, -36(%rbp)
 jmp for_start_0
for_end_0:
 movsd -32(%rbp), %xmm0
 movsd -32(%rbp), %xmm0
 movsd %xmm0, %xmm0
 leaq .LC_STR_1(%rip), %rdi
 movl $1, %eax
 call printf@PLT
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_1: .string "%f\n"
.section .note.GNU-stack,"",@progbits

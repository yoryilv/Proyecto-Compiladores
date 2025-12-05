.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 movl $5, %eax
 movl %eax, -4(%rbp)
 movl $1075838976, %eax
 movd %eax, %xmm0
 movsd %xmm0, -12(%rbp)
 movslq -4(%rbp), %rax
 pushq %rax
 movsd -12(%rbp), %xmm0
 # OPERACIÓN FLOTANTE
 movsd %xmm0, %xmm1
 popq %rax
 cvtsi2sdq %rax, %xmm0
 addsd %xmm1, %xmm0
 movsd %xmm0, -20(%rbp)
 movsd -20(%rbp), %xmm0
 movsd -20(%rbp), %xmm0
 movsd %xmm0, %xmm0
 leaq .LC_STR_0(%rip), %rdi
 movl $1, %eax
 call printf@PLT
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%f\n"
.section .note.GNU-stack,"",@progbits

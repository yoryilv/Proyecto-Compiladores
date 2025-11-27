.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 movq $5, %rax
 movq %rax, -16(%rbp)
 movsd .LC_FLT_0(%rip), %xmm0
 movsd %xmm0, -24(%rbp)
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq -16(%rbp), %rax
 movsd -24(%rbp), %xmm0
 # OPERACIÓN MIXTA INT/FLOAT DETECTADA
 cvtsi2sdq %rax, %xmm1
 addsd %xmm1, %xmm0
 movsd %xmm0, -32(%rbp)
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movsd -32(%rbp), %xmm0
 # DEBUG: argumento 1 tipo=2
 # DEBUG: num_float_args=1
 movsd -32(%rbp), %xmm0
 movsd %xmm0, %xmm0
 leaq .LC_STR_1(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $1, %eax
 # DEBUG: Establecido %eax=1 para floats
 call printf@PLT
 movq $0, %rax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_FLT_0: .double 2.5
.LC_STR_1: .string "%f\n"
.section .note.GNU-stack,"",@progbits

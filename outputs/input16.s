.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 movq $10, %rax
 movq %rax, -16(%rbp)
 movq $999, %rax
 movq %rax, -24(%rbp)
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq -16(%rbp), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq -16(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_0(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
 movq $0, %rax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%d\n"
.section .note.GNU-stack,"",@progbits

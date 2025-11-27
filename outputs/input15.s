.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 movq $1, %rax
 movq %rax, -16(%rbp)
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
 movq $100, %rax
 movq %rax, -16(%rbp)
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq -16(%rbp), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq -16(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%d\n"
.LC_STR_1: .string "%d\n"
.section .note.GNU-stack,"",@progbits

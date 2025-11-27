.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl suma
suma:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq -8(%rbp), %rax
 movq -16(%rbp), %rax
 movq -8(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 jmp .end_suma
.end_suma:
    leave
    ret
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 movq $10, %rax
 movq %rax, -16(%rbp)
 movq $20, %rax
 movq %rax, -24(%rbp)
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 # DEBUG FcallExp: nombre=suma, nargs=2
 movq -16(%rbp), %rax
 movq %rax, %rdi
 movq -24(%rbp), %rax
 movq %rax, %rsi
 call suma
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 # DEBUG FcallExp: nombre=suma, nargs=2
 movq -16(%rbp), %rax
 movq %rax, %rdi
 movq -24(%rbp), %rax
 movq %rax, %rsi
 call suma
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

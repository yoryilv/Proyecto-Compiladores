.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $64, %rsp
 movq $1, %rax
 movq %rax, -40(%rbp)
 movq $2, %rax
 movq %rax, -32(%rbp)
 movq $3, %rax
 movq %rax, -24(%rbp)
 movq $4, %rax
 movq %rax, -16(%rbp)
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq $0, %r11
 imulq $2, %r11
 movq $1, %rax
 addq %rax, %r11
 imulq $2, %r11
 movq $0, %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -40(%rbp), %rax
 addq %r11, %rax
 movq (%rax), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq $0, %r11
 imulq $2, %r11
 movq $1, %rax
 addq %rax, %r11
 imulq $2, %r11
 movq $0, %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -40(%rbp), %rax
 addq %r11, %rax
 movq (%rax), %rax
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

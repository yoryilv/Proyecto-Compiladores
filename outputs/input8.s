.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 movq $2147483648, %rax
 movq %rax, -16(%rbp)
 movq $10, %rax
 movq %rax, -24(%rbp)
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq -16(%rbp), %rax
 # DEBUG: argumento 1 tipo=1
 # DEBUG: num_float_args=0
 movq -16(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_0(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
 # DEBUG BinaryExp: left_type=3, right_type=0, op=>
 movq -24(%rbp), %rax
 movq $5, %rax
 movq -24(%rbp), %rax
 pushq %rax
 movq $5, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 seta %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_1
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq $1, %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq $1, %rax
 movq %rax, %rsi
 leaq .LC_STR_2(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
 jmp endif_1
 else_1:
endif_1:
 movq $0, %rax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%ld\n"
.LC_STR_2: .string "%d\n"
.section .note.GNU-stack,"",@progbits

.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl factorial
factorial:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    movq %rdi, -8(%rbp)
 # DEBUG BinaryExp: left_type=0, right_type=0, op=<=
 movq -8(%rbp), %rax
 movq $1, %rax
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_0
 movq $1, %rax
 jmp .end_factorial
 jmp endif_0
 else_0:
endif_0:
 # DEBUG BinaryExp: left_type=0, right_type=0, op=*
 movq -8(%rbp), %rax
 # DEBUG FcallExp: nombre=factorial, nargs=1
 # DEBUG BinaryExp: left_type=0, right_type=0, op=-
 movq -8(%rbp), %rax
 movq $1, %rax
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movq %rax, %rdi
 call factorial
 movq -8(%rbp), %rax
 pushq %rax
 # DEBUG FcallExp: nombre=factorial, nargs=1
 # DEBUG BinaryExp: left_type=0, right_type=0, op=-
 movq -8(%rbp), %rax
 movq $1, %rax
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movq %rax, %rdi
 call factorial
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 jmp .end_factorial
.end_factorial:
    leave
    ret
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 # DEBUG FcallExp: nombre=factorial, nargs=1
 movq $5, %rax
 movq %rax, %rdi
 call factorial
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 # DEBUG FcallExp: nombre=factorial, nargs=1
 movq $5, %rax
 movq %rax, %rdi
 call factorial
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
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
.LC_STR_1: .string "%d\n"
.section .note.GNU-stack,"",@progbits

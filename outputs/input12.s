.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 movsd .LC_FLT_0(%rip), %xmm0
 movsd %xmm0, -16(%rbp)
 # DEBUG BinaryExp: left_type=2, right_type=2, op=>
 movsd -16(%rbp), %xmm0
 movsd .LC_FLT_2(%rip), %xmm0
 # OPERACIÓN MIXTA INT/FLOAT DETECTADA
 cmpq $0, %rax
 je else_1
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq $1, %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq $1, %rax
 movq %rax, %rsi
 leaq .LC_STR_3(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
 jmp endif_1
 else_1:
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq $0, %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq $0, %rax
 movq %rax, %rsi
 leaq .LC_STR_4(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
endif_1:
 movq $0, %rax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_FLT_0: .double 3.14159
.LC_FLT_2: .double 3
.LC_STR_3: .string "%d\n"
.LC_STR_4: .string "%d\n"
.section .note.GNU-stack,"",@progbits

.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 # constant folding
 movq $30, %rax
 movq %rax, -16(%rbp)
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 # DEBUG BinaryExp: left_type=0, right_type=0, op=*
 # constant folding
 movq $10, %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=/
 # constant folding
 movq $25, %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=*
 # constant folding
 movq $10, %rax
 pushq %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=/
 # constant folding
 movq $25, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
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
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq -24(%rbp), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq -24(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
 # DEBUG BinaryExp: left_type=0, right_type=0, op=>
 # constant folding
 movq $0, %rax
 cmpq $0, %rax
 je else_2
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
 jmp endif_2
 else_2:
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
endif_2:
 movq $0, %rax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%d\n"
.LC_STR_1: .string "%d\n"
.LC_STR_3: .string "%d\n"
.LC_STR_4: .string "%d\n"
.section .note.GNU-stack,"",@progbits

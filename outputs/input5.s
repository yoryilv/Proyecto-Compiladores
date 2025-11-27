.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 movq $10, %rax
 movq %rax, -16(%rbp)
 # DEBUG BinaryExp: left_type=0, right_type=0, op=>
 movq -16(%rbp), %rax
 movq $5, %rax
 movq -16(%rbp), %rax
 pushq %rax
 movq $5, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setg %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_0
 # DEBUG BinaryExp: left_type=0, right_type=0, op=<
 movq -16(%rbp), %rax
 movq $20, %rax
 movq -16(%rbp), %rax
 pushq %rax
 movq $20, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
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
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq $0, %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq $0, %rax
 movq %rax, %rsi
 leaq .LC_STR_3(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
endif_1:
 jmp endif_0
 else_0:
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
endif_0:
 movq $0, %rax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_2: .string "%d\n"
.LC_STR_3: .string "%d\n"
.LC_STR_4: .string "%d\n"
.section .note.GNU-stack,"",@progbits

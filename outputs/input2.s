.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.globl global_val
.align 8
global_val:
 .quad 5
.text
.globl incrementar
incrementar:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq global_val(%rip), %rax
 movq $1, %rax
 movq global_val(%rip), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, global_val(%rip)
.end_incrementar:
    leave
    ret
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 # DEBUG FcallExp: nombre=incrementar, nargs=0
 call incrementar
 # DEBUG FcallExp: nombre=incrementar, nargs=0
 call incrementar
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq global_val(%rip), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq global_val(%rip), %rax
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

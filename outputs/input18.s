.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $64, %rsp
 movq $10, %rax
 movq %rax, -16(%rbp)
 movq $2, %rax
 movq %rax, -24(%rbp)
 movq $20, %rax
 movq %rax, -32(%rbp)
 movq $5, %rax
 movq %rax, -40(%rbp)
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq -16(%rbp), %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=*
 movq -24(%rbp), %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=-
 movq -32(%rbp), %rax
 movq -40(%rbp), %rax
 movq -32(%rbp), %rax
 pushq %rax
 movq -40(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movq -24(%rbp), %rax
 pushq %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=-
 movq -32(%rbp), %rax
 movq -40(%rbp), %rax
 movq -32(%rbp), %rax
 pushq %rax
 movq -40(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 movq -16(%rbp), %rax
 pushq %rax
 # DEBUG BinaryExp: left_type=0, right_type=1, op=*
 movq -24(%rbp), %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=-
 movq -32(%rbp), %rax
 movq -40(%rbp), %rax
 movq -32(%rbp), %rax
 pushq %rax
 movq -40(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movq -24(%rbp), %rax
 pushq %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=-
 movq -32(%rbp), %rax
 movq -40(%rbp), %rax
 movq -32(%rbp), %rax
 pushq %rax
 movq -40(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 subq %rcx, %rax
 movq %rax, %rcx
 popq %rax
 imulq %rcx, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -48(%rbp)
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq -48(%rbp), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq -48(%rbp), %rax
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

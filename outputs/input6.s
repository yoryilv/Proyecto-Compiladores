.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 movq $0, %rax
 movq %rax, -16(%rbp)
 movq $0, %rax
 movq %rax, -24(%rbp)
while_0:
 # DEBUG BinaryExp: left_type=0, right_type=0, op=<
 movq -16(%rbp), %rax
 movq $5, %rax
 movq -16(%rbp), %rax
 pushq %rax
 movq $5, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
 cmpq $0, %rax
 je endwhile_0
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq -24(%rbp), %rax
 movq -16(%rbp), %rax
 movq -24(%rbp), %rax
 pushq %rax
 movq -16(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -24(%rbp)
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq -16(%rbp), %rax
 movq $1, %rax
 movq -16(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -16(%rbp)
 jmp while_0
endwhile_0:
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
 movq $0, %rax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_1: .string "%d\n"
.section .note.GNU-stack,"",@progbits

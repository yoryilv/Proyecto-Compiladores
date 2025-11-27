.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $112, %rsp
 movq $0, %rax
 movq %rax, -88(%rbp)
for_start_0:
 # DEBUG BinaryExp: left_type=0, right_type=0, op=<
 movq -88(%rbp), %rax
 movq $3, %rax
 movq -88(%rbp), %rax
 pushq %rax
 movq $3, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
 cmpq $0, %rax
 je for_end_0
 movq $1, %rax
 pushq %rax
 movq $0, %r11
 imulq $3, %r11
 movq -88(%rbp), %rax
 addq %rax, %r11
 imulq $3, %r11
 movq -88(%rbp), %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -80(%rbp), %r10
 addq %r11, %r10
 popq %rax
 movq %rax, (%r10)
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq -88(%rbp), %rax
 movq $1, %rax
 movq -88(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -88(%rbp)
 jmp for_start_0
for_end_0:
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq $0, %r11
 imulq $3, %r11
 movq $1, %rax
 addq %rax, %r11
 imulq $3, %r11
 movq $1, %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -80(%rbp), %rax
 addq %r11, %rax
 movq (%rax), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq $0, %r11
 imulq $3, %r11
 movq $1, %rax
 addq %rax, %r11
 imulq $3, %r11
 movq $1, %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -80(%rbp), %rax
 addq %r11, %rax
 movq (%rax), %rax
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq $0, %r11
 imulq $3, %r11
 movq $0, %rax
 addq %rax, %r11
 imulq $3, %r11
 movq $1, %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -80(%rbp), %rax
 addq %r11, %rax
 movq (%rax), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq $0, %r11
 imulq $3, %r11
 movq $0, %rax
 addq %rax, %r11
 imulq $3, %r11
 movq $1, %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -80(%rbp), %rax
 addq %r11, %rax
 movq (%rax), %rax
 movq %rax, %rsi
 leaq .LC_STR_2(%rip), %rdi
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
.LC_STR_2: .string "%d\n"
.section .note.GNU-stack,"",@progbits

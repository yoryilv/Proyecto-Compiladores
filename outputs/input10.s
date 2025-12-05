.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 movl $0, %eax
 movl %eax, -40(%rbp)
for_start_0:
 movslq -40(%rbp), %rax
 pushq %rax
 movl $3, %eax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
 cmpq $0, %rax
 je for_end_0
 movl $1, %eax
 pushq %rax
 movq $0, %r11
 imulq $3, %r11
 movslq -40(%rbp), %rax
 addq %rax, %r11
 imulq $3, %r11
 movslq -40(%rbp), %rax
 addq %rax, %r11
 imulq $4, %r11
 leaq -36(%rbp), %r10
 addq %r11, %r10
 popq %rax
 movl %eax, (%r10)
 movslq -40(%rbp), %rax
 pushq %rax
 movl $1, %eax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movl %eax, -40(%rbp)
 jmp for_start_0
for_end_0:
 movq $0, %r11
 imulq $3, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $3, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $4, %r11
 leaq -36(%rbp), %rax
 addq %r11, %rax
 movslq (%rax), %rax
 movq $0, %r11
 imulq $3, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $3, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $4, %r11
 leaq -36(%rbp), %rax
 addq %r11, %rax
 movslq (%rax), %rax
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movq $0, %r11
 imulq $3, %r11
 movl $0, %eax
 addq %rax, %r11
 imulq $3, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $4, %r11
 leaq -36(%rbp), %rax
 addq %r11, %rax
 movslq (%rax), %rax
 movq $0, %r11
 imulq $3, %r11
 movl $0, %eax
 addq %rax, %r11
 imulq $3, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $4, %r11
 leaq -36(%rbp), %rax
 addq %r11, %rax
 movslq (%rax), %rax
 movq %rax, %rsi
 leaq .LC_STR_2(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_1: .string "%d\n"
.LC_STR_2: .string "%d\n"
.section .note.GNU-stack,"",@progbits

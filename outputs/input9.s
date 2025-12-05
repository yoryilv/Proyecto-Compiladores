.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 movl $1, %eax
 movl %eax, -16(%rbp)
 movl $2, %eax
 movl %eax, -12(%rbp)
 movl $3, %eax
 movl %eax, -8(%rbp)
 movl $4, %eax
 movl %eax, -4(%rbp)
 movq $0, %r11
 imulq $2, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $2, %r11
 movl $0, %eax
 addq %rax, %r11
 imulq $4, %r11
 leaq -16(%rbp), %rax
 addq %r11, %rax
 movslq (%rax), %rax
 movq $0, %r11
 imulq $2, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $2, %r11
 movl $0, %eax
 addq %rax, %r11
 imulq $4, %r11
 leaq -16(%rbp), %rax
 addq %r11, %rax
 movslq (%rax), %rax
 movq %rax, %rsi
 leaq .LC_STR_0(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_0: .string "%d\n"
.section .note.GNU-stack,"",@progbits

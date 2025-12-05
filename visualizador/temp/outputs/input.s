.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $48, %rsp
 movl $5, %eax
 pushq %rax
 movq $0, %r11
 imulq $3, %r11
 movl $1, %eax
 addq %rax, %r11
 imulq $3, %r11
 movl $0, %eax
 addq %rax, %r11
 imulq $4, %r11
 leaq -36(%rbp), %r10
 addq %r11, %r10
 popq %rax
 movl %eax, (%r10)
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .note.GNU-stack,"",@progbits

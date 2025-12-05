.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .note.GNU-stack,"",@progbits

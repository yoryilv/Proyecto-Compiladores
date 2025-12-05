.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl suma
suma:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movl %edi, -4(%rbp)
    movl %esi, -8(%rbp)
 movslq -4(%rbp), %rax
 pushq %rax
 movslq -8(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 jmp .end_suma
.end_suma:
    leave
    ret
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 movl $10, %eax
 movl %eax, -4(%rbp)
 movl $20, %eax
 movl %eax, -8(%rbp)
 movslq -4(%rbp), %rax
 movq %rax, %rdi
 movslq -8(%rbp), %rax
 movq %rax, %rsi
 call suma
 movslq -4(%rbp), %rax
 movq %rax, %rdi
 movslq -8(%rbp), %rax
 movq %rax, %rsi
 call suma
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

.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 movl $0, %eax
 movl %eax, -4(%rbp)
 movl $1, %eax
 movl %eax, -8(%rbp)
for_start_0:
 movslq -8(%rbp), %rax
 pushq %rax
 movl $5, %eax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setle %al
 movzbq %al, %rax
 cmpq $0, %rax
 je for_end_0
 movslq -4(%rbp), %rax
 pushq %rax
 movslq -8(%rbp), %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movl %eax, -4(%rbp)
 movslq -8(%rbp), %rax
 pushq %rax
 movl $1, %eax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movl %eax, -8(%rbp)
 jmp for_start_0
for_end_0:
 movslq -4(%rbp), %rax
 movslq -4(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_1(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_1: .string "%d\n"
.section .note.GNU-stack,"",@progbits

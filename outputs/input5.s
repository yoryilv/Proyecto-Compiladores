.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp
 movl $10, %eax
 movl %eax, -4(%rbp)
 movslq -4(%rbp), %rax
 pushq %rax
 movl $5, %eax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setg %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_0
 movslq -4(%rbp), %rax
 pushq %rax
 movl $20, %eax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_1
 movl $1, %eax
 movl $1, %eax
 movq %rax, %rsi
 leaq .LC_STR_2(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 jmp endif_1
 else_1:
 movl $0, %eax
 movl $0, %eax
 movq %rax, %rsi
 leaq .LC_STR_3(%rip), %rdi
 movl $0, %eax
 call printf@PLT
endif_1:
 jmp endif_0
 else_0:
 movl $0, %eax
 movl $0, %eax
 movq %rax, %rsi
 leaq .LC_STR_4(%rip), %rdi
 movl $0, %eax
 call printf@PLT
endif_0:
 movl $0, %eax
 jmp .end_main
.end_main:
    leave
    ret
.section .rodata
.LC_STR_2: .string "%d\n"
.LC_STR_3: .string "%d\n"
.LC_STR_4: .string "%d\n"
.section .note.GNU-stack,"",@progbits

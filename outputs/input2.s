.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.globl global_val
.align 4
global_val:
 .long 5
.text
.globl incrementar
incrementar:
    pushq %rbp
    movq %rsp, %rbp
 movslq global_val(%rip), %rax
 pushq %rax
 movl $1, %eax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movl %eax, global_val(%rip)
.end_incrementar:
    leave
    ret
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
 call incrementar
 call incrementar
 movslq global_val(%rip), %rax
 movslq global_val(%rip), %rax
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

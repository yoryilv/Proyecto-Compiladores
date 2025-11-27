.data
print_fmt: .string "%ld \n"
print_fmt_float: .string "%f \n"
.text
# Wrapper de main para inicializar/cerrar tracing
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp
    call trace_init
    call user_main
    pushq %rax
    call trace_close
    popq %rax
    leave
    ret

# Función main del usuario (renombrada a user_main)
.globl user_main
user_main:
    pushq %rbp
    movq %rsp, %rbp
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq $2, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_0(%rip), %rdi
    leaq .LC_SRC_0(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_0: .string "Number literal"
.LC_INST_0: .string "LITERAL_INT_LOAD"
.text
 # DEBUG BinaryExp: left_type=0, right_type=0, op=*
 # constant folding
 movq $12, %rax
 # Traza: const fold: * (línea 0)
    subq $8, %rsp
    leaq .LC_INST_1(%rip), %rdi
    leaq .LC_SRC_1(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_1: .string "const fold: *"
.LC_INST_1: .string "BINARY_CONST_FOLD"
.text
 movq $2, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_2(%rip), %rdi
    leaq .LC_SRC_2(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_2: .string "Number literal"
.LC_INST_2: .string "LITERAL_INT_LOAD"
.text
 pushq %rax
 # DEBUG BinaryExp: left_type=0, right_type=0, op=*
 # constant folding
 movq $12, %rax
 # Traza: const fold: * (línea 0)
    subq $8, %rsp
    leaq .LC_INST_3(%rip), %rdi
    leaq .LC_SRC_3(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_3: .string "const fold: *"
.LC_INST_3: .string "BINARY_CONST_FOLD"
.text
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 # Traza: add (línea 0)
    subq $8, %rsp
    leaq .LC_INST_4(%rip), %rdi
    leaq .LC_SRC_4(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_4: .string "add"
.LC_INST_4: .string "BINARY_INT_OP"
.text
 movq %rax, -16(%rbp)
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movq -16(%rbp), %rax
 # DEBUG: argumento 1 tipo=0
 # DEBUG: num_float_args=0
 movq -16(%rbp), %rax
 movq %rax, %rsi
 leaq .LC_STR_5(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $0, %eax
 # DEBUG: Establecido %eax=0 para enteros
 call printf@PLT
 movq $0, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_6(%rip), %rdi
    leaq .LC_SRC_6(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_6: .string "Number literal"
.LC_INST_6: .string "LITERAL_INT_LOAD"
.text
 jmp .end_user_main
.end_user_main:
    leave
    ret
.section .rodata
.LC_STR_5: .string "%d\n"
.section .note.GNU-stack,"",@progbits

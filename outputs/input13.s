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
 movsd .LC_FLT_0(%rip), %xmm0
 movsd %xmm0, -32(%rbp)
 movsd .LC_FLT_1(%rip), %xmm0
 movsd %xmm0, -24(%rbp)
 movsd .LC_FLT_2(%rip), %xmm0
 movsd %xmm0, -16(%rbp)
 movsd .LC_FLT_3(%rip), %xmm0
 movsd %xmm0, -40(%rbp)
 movq $0, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_5(%rip), %rdi
    leaq .LC_SRC_5(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_5: .string "Number literal"
.LC_INST_5: .string "LITERAL_INT_LOAD"
.text
 movq %rax, -48(%rbp)
 # Traza: Assign to local var: i (línea 0)
    subq $8, %rsp
    leaq .LC_INST_6(%rip), %rdi
    leaq .LC_SRC_6(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_6: .string "Assign to local var: i"
.LC_INST_6: .string "ASSIGN_INT_VAR"
.text
 # Traza: Initializing FOR loop (línea 0)
    subq $8, %rsp
    leaq .LC_INST_7(%rip), %rdi
    leaq .LC_SRC_7(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_7: .string "Initializing FOR loop"
.LC_INST_7: .string "FOR_INIT"
.text
for_start_4:
 # DEBUG BinaryExp: left_type=0, right_type=0, op=<
 movq -48(%rbp), %rax
 movq $3, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_8(%rip), %rdi
    leaq .LC_SRC_8(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_8: .string "Number literal"
.LC_INST_8: .string "LITERAL_INT_LOAD"
.text
 movq -48(%rbp), %rax
 pushq %rax
 movq $3, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_9(%rip), %rdi
    leaq .LC_SRC_9(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_9: .string "Number literal"
.LC_INST_9: .string "LITERAL_INT_LOAD"
.text
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 setl %al
 movzbq %al, %rax
 # Traza: lt (línea 0)
    subq $8, %rsp
    leaq .LC_INST_10(%rip), %rdi
    leaq .LC_SRC_10(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_10: .string "lt"
.LC_INST_10: .string "BINARY_INT_OP"
.text
 # Traza: Evaluating loop condition (línea 0)
    subq $8, %rsp
    leaq .LC_INST_11(%rip), %rdi
    leaq .LC_SRC_11(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_11: .string "Evaluating loop condition"
.LC_INST_11: .string "FOR_COND"
.text
 cmpq $0, %rax
 je for_end_4
 # Traza: Executing loop body (línea 0)
    subq $8, %rsp
    leaq .LC_INST_12(%rip), %rdi
    leaq .LC_SRC_12(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_12: .string "Executing loop body"
.LC_INST_12: .string "FOR_BODY"
.text
 # DEBUG BinaryExp: left_type=2, right_type=2, op=+
 movsd -40(%rbp), %xmm0
 movq $0, %r11
 imulq $3, %r11
 movq -48(%rbp), %rax
 addq %rax, %r11
 imulq $8, %r11
 leaq -32(%rbp), %rax
 addq %r11, %rax
 movsd (%rax), %xmm0
 # OPERACIÓN MIXTA INT/FLOAT DETECTADA
 subq $8, %rsp
 movsd %xmm0, (%rsp)
 movsd -40(%rbp), %xmm0
 movsd (%rsp), %xmm1
 addq $8, %rsp
 addsd %xmm1, %xmm0
 # Traza: float + (línea 0)
    subq $8, %rsp
    leaq .LC_INST_13(%rip), %rdi
    leaq .LC_SRC_13(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_13: .string "float +"
.LC_INST_13: .string "BINARY_FLOAT_OP"
.text
 movsd %xmm0, -40(%rbp)
 # Traza: Assign to local var: suma (línea 0)
    subq $8, %rsp
    leaq .LC_INST_14(%rip), %rdi
    leaq .LC_SRC_14(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_14: .string "Assign to local var: suma"
.LC_INST_14: .string "ASSIGN_FLOAT_VAR"
.text
 # DEBUG BinaryExp: left_type=0, right_type=0, op=+
 movq -48(%rbp), %rax
 movq $1, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_15(%rip), %rdi
    leaq .LC_SRC_15(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_15: .string "Number literal"
.LC_INST_15: .string "LITERAL_INT_LOAD"
.text
 movq -48(%rbp), %rax
 pushq %rax
 movq $1, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_16(%rip), %rdi
    leaq .LC_SRC_16(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_16: .string "Number literal"
.LC_INST_16: .string "LITERAL_INT_LOAD"
.text
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 # Traza: add (línea 0)
    subq $8, %rsp
    leaq .LC_INST_17(%rip), %rdi
    leaq .LC_SRC_17(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_17: .string "add"
.LC_INST_17: .string "BINARY_INT_OP"
.text
 movq %rax, -48(%rbp)
 # Traza: Assign to local var: i (línea 0)
    subq $8, %rsp
    leaq .LC_INST_18(%rip), %rdi
    leaq .LC_SRC_18(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_18: .string "Assign to local var: i"
.LC_INST_18: .string "ASSIGN_INT_VAR"
.text
 # Traza: Applying increment (línea 0)
    subq $8, %rsp
    leaq .LC_INST_19(%rip), %rdi
    leaq .LC_SRC_19(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_19: .string "Applying increment"
.LC_INST_19: .string "FOR_INC"
.text
 jmp for_start_4
for_end_4:
 # Traza: Exiting FOR loop (línea 0)
    subq $8, %rsp
    leaq .LC_INST_20(%rip), %rdi
    leaq .LC_SRC_20(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_20: .string "Exiting FOR loop"
.LC_INST_20: .string "FOR_END"
.text
 # DEBUG FcallExp: nombre=printf, nargs=2
 # DEBUG: Entrando en caso especial printf
 movsd -40(%rbp), %xmm0
 # DEBUG: argumento 1 tipo=2
 # DEBUG: num_float_args=1
 movsd -40(%rbp), %xmm0
 movsd %xmm0, %xmm0
 leaq .LC_STR_21(%rip), %rdi
 # DEBUG: Antes de establecer %eax
 movl $1, %eax
 # DEBUG: Establecido %eax=1 para floats
 call printf@PLT
 movq $0, %rax
 # Traza: Number literal (línea 0)
    subq $8, %rsp
    leaq .LC_INST_22(%rip), %rdi
    leaq .LC_SRC_22(%rip), %rsi
    movl $0, %edx
    call trace_step
    addq $8, %rsp
.section .rodata
.LC_SRC_22: .string "Number literal"
.LC_INST_22: .string "LITERAL_INT_LOAD"
.text
 jmp .end_user_main
.end_user_main:
    leave
    ret
.section .rodata
.LC_FLT_0: .double 10.5
.LC_FLT_1: .double 11.5
.LC_FLT_2: .double 12
.LC_FLT_3: .double 0
.LC_STR_21: .string "%f\n"
.section .note.GNU-stack,"",@progbits

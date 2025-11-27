#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

typedef struct {
    unsigned long rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp;
    unsigned long r8, r9, r10, r11, r12, r13, r14, r15;
} CpuState;

static int step_counter = 0;
static FILE* trace_file = NULL;
static int trace_initialized = 0;

void trace_close_internal() {
    if (!trace_file || !trace_initialized) return;
    
    printf("🔧 Cerrando traza automáticamente...\n");
    
    fprintf(trace_file, "\n]");
    fflush(trace_file);
    fclose(trace_file);
    trace_file = NULL;
    trace_initialized = 0;
    
    printf("✅ Traza guardada en execution_trace.json (%d pasos)\n", step_counter);
}

void signal_handler(int signum) {
    printf("⚠️ Señal %d recibida, guardando traza...\n", signum);
    trace_close_internal();
    signal(signum, SIG_DFL);
    raise(signum);
}

void trace_init() {
    if (trace_initialized) {
        printf("⚠️ trace_init ya fue llamado\n");
        return;
    }
    
    printf("🔧 INIT_TRACE llamado - Directorio actual: ");
    system("pwd");
    
    trace_file = fopen("execution_trace.json", "w");
    if (trace_file) {
        printf("✅ Archivo execution_trace.json abierto correctamente\n");
        fprintf(trace_file, "[\n");
        fflush(trace_file);
        trace_initialized = 1;
        
        atexit(trace_close_internal);
        
        signal(SIGINT, signal_handler);
        signal(SIGTERM, signal_handler);
        signal(SIGSEGV, signal_handler);
        signal(SIGABRT, signal_handler);
        
    } else {
        perror("❌ Error abriendo execution_trace.json");
    }
}

void trace_close() {
    trace_close_internal();
}

void trace_step(const char* instruction_type, const char* source_info, int line_number) {
    if (!trace_file) {
        printf("❌ trace_file es NULL en trace_step\n");
        return;
    }
    
    CpuState state;
    
    asm volatile (
        "mov %%rax, %0\n\t"
        "mov %%rbx, %1\n\t" 
        "mov %%rcx, %2\n\t"
        "mov %%rdx, %3\n\t"
        "mov %%rsi, %4\n\t"
        "mov %%rdi, %5\n\t"
        "mov %%rbp, %6\n\t"
        "mov %%rsp, %7\n\t"
        : "=r"(state.rax), "=r"(state.rbx), "=r"(state.rcx),
          "=r"(state.rdx), "=r"(state.rsi), "=r"(state.rdi),
          "=r"(state.rbp), "=r"(state.rsp)
    );

    state.r8 = state.r9 = state.r10 = state.r11 = 0;
    state.r12 = state.r13 = state.r14 = state.r15 = 0;

    if (step_counter > 0) {
        fprintf(trace_file, ",\n");
    }
    
    fprintf(trace_file,
        "  {\"step\": %d, \"instruction\": \"%s\", \"source\": \"%s\", \"line\": %d, \"registers\": {"
        "\"rax\": \"0x%lx\", \"rbx\": \"0x%lx\", \"rcx\": \"0x%lx\", \"rdx\": \"0x%lx\", "
        "\"rsi\": \"0x%lx\", \"rdi\": \"0x%lx\", \"rbp\": \"0x%lx\", \"rsp\": \"0x%lx\"}}",
        step_counter, instruction_type, source_info, line_number,
        state.rax, state.rbx, state.rcx, state.rdx, state.rsi, state.rdi,
        state.rbp, state.rsp
    );
    
    step_counter++;
    fflush(trace_file);
    
    if (step_counter % 10 == 0) {
        printf("📝 Paso %d registrado\n", step_counter);
    }
}
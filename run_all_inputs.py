import os
import subprocess
import shutil

# --- CONFIGURACIÓN DE ARCHIVOS ---

# Archivos C++ del compilador
programa_compilador = ["main.cpp", "scanner.cpp", "token.cpp", "parser.cpp", "ast.cpp", "visitor_traced.cpp"]

# Nuevo archivo C del módulo de runtime
programa_runtime = ["runtime_trace.c"]

# Nombre del ejecutable del compilador (generador de .s)
COMPILER_EXEC = "./compilador"

# Nombre del ejecutable final del programa de prueba (que generará la traza)
TEST_EXEC = "./test_exec"

# --------------------------------------------------------------------------
# --- 1. COMPILACIÓN DEL COMPILADOR (Crea el generador de .s) ---
# --------------------------------------------------------------------------

# Compilar C++ y C, enlazando el runtime para que la función log_state_rt exista.
compile_cmd = ["g++"] + programa_compilador + ["-o", COMPILER_EXEC]

print("--- 1. COMPILANDO EL GENERADOR DE ENSAMBLADOR ---")
print("Compilando:", " ".join(compile_cmd))
result = subprocess.run(compile_cmd, capture_output=True, text=True)

if result.stderr:
    print("--- INICIO DE ERRORES DE COMPILACION (Generador) ---")
    print(result.stderr)
    print("--- FIN DE ERRORES DE COMPILACION (Generador) ---")

if result.returncode != 0:
    print("Error en compilación del generador: Saliendo.")
    exit(1)

print("Compilación del generador exitosa")

# --------------------------------------------------------------------------
# --- 2. EJECUCIÓN Y PRUEBA DE TRAZA (Genera el .s y lo ejecuta) ---
# --------------------------------------------------------------------------

input_dir = "inputs"
output_dir = "outputs"
os.makedirs(output_dir, exist_ok=True)

# Limpiar el archivo de log anterior
if os.path.exists("trace.log"):
    os.remove("trace.log")
    print("Limpiando trace.log anterior.")

for i in range(1, 19):
    filename = f"input{i}.txt"
    filepath = os.path.join(input_dir, filename)

    if os.path.isfile(filepath):
        print(f"\n--- Ejecutando {filename} ---")
        
        # ✅ CORRECCIÓN: El compilador ahora genera directamente en outputs/
        # Ya no necesitamos mover archivos
        final_asm_name = f"input{i}.s"
        output_asm = os.path.join(output_dir, final_asm_name)

        # 2a. Generar el código ensamblador (.s) directamente en outputs/
        # Se le pasa la ruta completa al compilador
        run_cmd = [COMPILER_EXEC, filepath]
        
        try:
            print(f"-> Generando ASM: {output_asm}")
            # Ejecutar el compilador (que crea el archivo .s en el directorio 'outputs')
            result = subprocess.run(run_cmd, capture_output=True, text=True, check=True)
            
            # ✅ VERIFICAR si el archivo se generó en outputs/
            if os.path.isfile(output_asm):
                
                # 2b. Compilar y Enlazar el ASM generado
                print("-> Compilando y Enlazando para traza...")
                
                # Comando para crear el ejecutable final (ASM en outputs + runtime_trace)
                link_cmd = ["gcc", output_asm] + programa_runtime + ["-o", TEST_EXEC]
                
                # Ejecutar la compilación/enlazado
                link_result = subprocess.run(link_cmd, capture_output=True, text=True)
                
                if link_result.returncode != 0:
                    print(f"    ERROR EN ENLAZADO de {output_asm}")
                    print("    STDERR:")
                    print(link_result.stderr)
                    print("    STDOUT:")
                    print(link_result.stdout)
                    continue

                # 2c. Ejecutar el ejecutable final para generar la traza
                print("-> Ejecutando para generar traza y log...")
                # La ejecución produce la salida normal del programa Y escribe al trace.log
                run_test_cmd = [TEST_EXEC]
                
                test_result = subprocess.run(run_test_cmd, capture_output=True, text=True, timeout=5)
                
                if test_result.returncode != 0:
                    print(f"    ERROR EN EJECUCIÓN (código {test_result.returncode})")
                    print("    STDERR:")
                    print(test_result.stderr)
                    print("    STDOUT:")
                    print(test_result.stdout)
                else:
                    print(f"    Ejecución terminada. La traza de esta prueba se añadió a 'trace.log'.")
                    print(f"    Output del programa:")
                    print(test_result.stdout)

            else:
                print(f" ADVERTENCIA: No se encontró {output_asm} ({os.path.abspath(output_asm)}). Revise la lógica de generación en main.cpp.")

        except subprocess.CalledProcessError as e:
            print(f" ERROR al ejecutar el generador/test para {filename}:")
            print("    STDERR:")
            print(e.stderr)
            print("    STDOUT:")
            print(e.stdout)
        except subprocess.TimeoutExpired:
            print(f" ERROR: Timeout ejecutando {filename} (posible bucle infinito)")

    else:
        print(filename, "no encontrado en", input_dir)
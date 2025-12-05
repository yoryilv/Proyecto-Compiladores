import os
import subprocess
import re
from flask import Flask, render_template, request, jsonify

# --- CONFIGURACIÓN DE RUTAS ---
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# Buscamos el compilador un nivel arriba ("..")
COMPILER_EXE = os.path.join(BASE_DIR, "..", "mi_compilador")

# Carpetas temporales dentro de 'visualizador'
TEMP_DIR = os.path.join(BASE_DIR, "temp")
OUTPUTS_DIR = os.path.join(TEMP_DIR, "outputs") # Tu compilador guarda aquí

# Crear carpetas necesarias
os.makedirs(TEMP_DIR, exist_ok=True)
os.makedirs(OUTPUTS_DIR, exist_ok=True)

# Inicializar Flask (busca templates en la carpeta actual '.')
app = Flask(__name__, template_folder='.')

def parse_gdb_output(output):
    """
    Parsea la salida de GDB ajustada a tu versión específica.
    """
    # Debug: Imprimir para confirmar
    # print("--- DEBUG PARSER START ---")
    # print(output)
    
    steps = []
    
    # Dividimos por bloques ---STEP---
    raw_steps = output.split("---STEP---")
    
    # REGEX AJUSTADA A TU LOG:
    # Captura: (rax) + espacios + (0x401106) + opcionalmente el valor decimal
    # La clave es \s+ para tragar todos los espacios intermedios
    reg_pattern = re.compile(r"^\s*([a-z0-9]{2,3})\s+(0x[0-9a-f]+)", re.MULTILINE | re.IGNORECASE)
    
    for raw in raw_steps:
        # Solo procesamos si hay info de registros
        if "Info registers" not in raw: 
            continue
            
        step_data = {
            "regs": {},
            "stack": []
        }
        
        # 1. Extraer Registros
        matches = reg_pattern.findall(raw)
        for name, val in matches:
            step_data["regs"][name] = val
            
        # 2. Extraer Pila
        # Tu log muestra: "0x7fffffffd598: 0xf7c2a1ca"
        for line in raw.splitlines():
            line = line.strip()
            if line.startswith("0x") and ":" in line:
                try:
                    parts = line.split(":")
                    addr = parts[0].strip()
                    # Tomamos todo lo que hay a la derecha
                    content = parts[1].strip()
                    # Tu GDB pone varios valores separados por tabuladores/espacios
                    vals = content.split()
                    if vals:
                        # Tomamos el primero que parezca hex
                        val = vals[0]
                        if val.startswith("0x"):
                            step_data["stack"].append({"addr": addr, "val": val})
                except:
                    continue

        # Solo agregamos el paso si encontramos registros válidos
        if step_data["regs"]:
            steps.append(step_data)
            
    return steps

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/compile', methods=['POST'])
def compile_code():
    data = request.json
    c_code = data.get('code', '')
    
    # Definición de nombres de archivo
    input_filename = 'input.txt'
    input_path = os.path.join(TEMP_DIR, input_filename)
    
    # El archivo .s que genera tu compilador (en la carpeta outputs)
    expected_asm_filename = 'input.s' 
    asm_path = os.path.join(OUTPUTS_DIR, expected_asm_filename)
    
    # El ejecutable final y el script de depuración
    program_exe = os.path.join(TEMP_DIR, 'program')
    gdb_script_path = os.path.join(TEMP_DIR, 'script.gdb')
    
    # 1. Guardar el código C
    with open(input_path, 'w') as f:
        f.write(c_code)

    try:
        # --- LIMPIEZA PREVIA ---
        if os.path.exists(asm_path): os.remove(asm_path)
        if os.path.exists(program_exe): os.remove(program_exe)

        # 2. EJECUTAR TU COMPILADOR (C++ -> ASM)
        # cwd=TEMP_DIR es crucial para que la carpeta 'outputs' se cree donde debe
        comp_res = subprocess.run(
            [COMPILER_EXE, input_filename], 
            cwd=TEMP_DIR, 
            capture_output=True, 
            text=True
        )
        
        # Verificar errores del compilador C++
        if comp_res.returncode != 0:
            err = comp_res.stderr if comp_res.stderr else comp_res.stdout
            return jsonify({'error': f"Error de Compilación:\n{err}"}), 400

        # Verificar que se generó el archivo .s
        if not os.path.exists(asm_path):
             return jsonify({'error': f"El compilador no generó {expected_asm_filename}.\nLogs: {comp_res.stdout}"}), 500
        
        # Leer el ASM para enviarlo al frontend
        with open(asm_path, 'r') as f:
            asm_content = f.read()

        # 3. ENSAMBLAR CON GCC (ASM -> BINARIO)
        # -g: Agrega información de debug para GDB
        # -no-pie: Deshabilita la aleatorización de direcciones (vital para debug simple)
        subprocess.run(
            ['gcc', '-g', asm_path, '-o', 'program', '-no-pie'], 
            cwd=TEMP_DIR, 
            check=True, 
            capture_output=True
        )
        
        # 4. PREPARAR SCRIPT DE GDB (SOLUCIÓN ROBUSTA)
        # Usamos la ruta absoluta del ejecutable para evitar confusiones en GDB
        program_abs_path = os.path.abspath(program_exe)
        
        # CAMBIOS CLAVE:
        # 1. Usamos 'nexti' en lugar de 'stepi' para saltar librerías (printf).
        # 2. Agregamos un contador ($cnt) para evitar bucles infinitos o salirnos del main.
        
        gdb_commands = f"""
file {program_abs_path}
set pagination off
set width 0
break main
run

set $cnt = 0

# Bucle con límite de seguridad (Max 200 pasos)
while ($cnt < 200) 
  echo \\n---STEP---\\n
  echo Info registers:\\n
  info registers rax rbx rcx rdx rsi rdi rsp rbp
  echo Stack dump:\\n
  x/10xw $rsp
  
  # IMPORTANTE: nexti ejecuta la instrucción actual. 
  # Si es un CALL (como printf), lo pasa por encima sin entrar.
  nexti
  
  set $cnt = $cnt + 1
  
  # Truco: Si el Stack Pointer ($rsp) sube demasiado, significa que salimos del main.
  # (Esto es opcional, el límite de 200 suele bastar).
end
quit
"""
        # Guardamos el script en un archivo físico
        with open(gdb_script_path, 'w') as f:
            f.write(gdb_commands)

        # 5. EJECUTAR GDB USANDO EL SCRIPT
        print(f"DEBUG: Ejecutando GDB con {gdb_script_path}...")
        
        gdb_res = subprocess.run(
            ['gdb', '-batch', '-x', 'script.gdb'],
            cwd=TEMP_DIR,
            capture_output=True,
            text=True,
            timeout=5 # Timeout de seguridad (5 segundos)
        )
        
        # DEBUG: Imprimir errores de GDB en la terminal de Python si los hay
        if gdb_res.stderr:
            print("--- GDB STDERR ---")
            print(gdb_res.stderr)
            print("------------------")

        # 6. PROCESAR SALIDA DE GDB
        debug_steps = parse_gdb_output(gdb_res.stdout)
        
        # 7. EJECUTAR PROGRAMA FINAL (Para obtener el output limpio del usuario)
        run_res = subprocess.run(
            ['./program'], 
            cwd=TEMP_DIR, 
            capture_output=True, 
            text=True, 
            timeout=2
        )

        return jsonify({
            'asm': asm_content,
            'output': run_res.stdout,
            'debug_steps': debug_steps
        })

    except subprocess.TimeoutExpired:
         return jsonify({'error': "El programa tardó demasiado (posible bucle infinito)."}), 400
    except subprocess.CalledProcessError as e:
        err = e.stderr.decode() if e.stderr else str(e)
        return jsonify({'error': f"Error GCC/GDB:\n{err}"}), 400
    except Exception as e:
        return jsonify({'error': f"Error interno: {str(e)}"}), 500

if __name__ == '__main__':
    app.run(debug=True, port=5000)
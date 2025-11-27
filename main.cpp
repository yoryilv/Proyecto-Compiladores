#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>  // Para manejo de rutas
#include "scanner.h"
#include "parser.h"
#include "visitor.h"

using namespace std;

int main(int argc, char* argv[]) {
    bool enable_tracing = false;
    string input_file;
    string output_file;
    
    // Procesar argumentos
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--trace") {
            enable_tracing = true;
            cout << "✅ Modo traza activado" << endl;
        } else if (arg == "-o" && i + 1 < argc) {
            output_file = argv[++i];
        } else {
            input_file = arg;
        }
    }
    
    if (input_file.empty()) {
        cerr << "Uso: " << argv[0] << " [--trace] [-o output.s] input.c" << endl;
        return 1;
    }
    
    // ✅ CORRECCIÓN: Generar archivo .s en carpeta outputs/
    if (output_file.empty()) {
        // Extraer nombre base del archivo de entrada
        size_t last_slash = input_file.find_last_of("/\\");
        string base_name = (last_slash == string::npos) ? input_file : input_file.substr(last_slash + 1);
        
        // Cambiar extensión a .s
        size_t last_dot = base_name.find_last_of(".");
        if (last_dot != string::npos) {
            base_name = base_name.substr(0, last_dot);
        }
        base_name += ".s";
        
        // Usar carpeta outputs
        output_file = "outputs/" + base_name;
    }
    
    try {
        ifstream in(input_file);
        if (!in) {
            cerr << "Error: No se puede abrir " << input_file << endl;
            return 1;
        }
        
        Scanner scanner(in);
        Parser parser(&scanner);
        Program* program = parser.parseProgram();
        
        // ✅ Asegurarse de que la carpeta outputs existe
        system("mkdir -p outputs");
        
        ofstream out(output_file);
        if (!out) {
            cerr << "Error: No se puede crear " << output_file << endl;
            return 1;
        }
        
        GenCodeVisitor generator(out);
        generator.enableTracing(enable_tracing);
        generator.generar(program);
        
        cout << "✅ Compilación exitosa: " << output_file << endl;
        
        delete program;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "token.h"
#include "scanner.h"

using namespace std;

// -----------------------------
// Constructor
// -----------------------------
Scanner::Scanner(const char* s): input(s), first(0), current(0) { 
    }

Scanner::Scanner(std::istream& in_s) {
    std::stringstream buffer;
    buffer << in_s.rdbuf();
    input = buffer.str();
    first = 0;
    current = 0;
}
// -----------------------------
// Función auxiliar
// -----------------------------

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

// -----------------------------
// nextToken: obtiene el siguiente token
// -----------------------------


Token* Scanner::nextToken() {
    Token* token;

    while (current < input.length()) {
        if (is_white_space(input[current])) {
            current++;
            continue; 
        }

        if (input[current] == '#') {
            while (current < input.length() && input[current] != '\n') {
                current++;
            }
            continue; 
        }

        if (input[current] == '/' && current + 1 < input.length() && input[current+1] == '/') {
            while (current < input.length() && input[current] != '\n') {
                current++;
            }
            continue; 
        }

        break;
    }


    if (current >= input.length()) 
        return new Token(Token::END);

    char c = input[current];
    first = current;

    // Números: int, long, unsigned, float
    if (isdigit(c)) {
        while (current < input.length() && isdigit(input[current])) {
            current++;
        }

        Token::Type type = Token::INT_CONST;

        if (current < input.length() && input[current] == '.') {
            type = Token::FLOAT_CONST;
            current++;
            
            while (current < input.length() && isdigit(input[current])) {
                current++;
            }
        }
        else if (current < input.length() && (input[current] == 'L' || input[current] == 'l')) {
            type = Token::LONG_CONST;
            current++;
        }
        else if (current < input.length() && (input[current] == 'U' || input[current] == 'u')) {
            type = Token::UINT_CONST;
            current++;
        }
        
        token = new Token(type, input, first, current - first);
    }

    // ID
    else if (isalpha(c) || c == '_') {
        current++;
        while (current < input.length() && (isalnum(input[current]) || input[current] == '_'))
            current++;
        
        string lexema = input.substr(first, current - first);
        
        if (lexema=="if") return new Token(Token::IF, input, first, current - first);
        else if (lexema=="else") return new Token(Token::ELSE, input, first, current - first);
        else if (lexema=="while") return new Token(Token::WHILE, input, first, current - first);
        else if (lexema=="do") return new Token(Token::DO, input, first, current - first);
        else if (lexema=="for") return new Token(Token::FOR, input, first, current - first);
        else if (lexema=="break") return new Token(Token::BREAK, input, first, current - first);
        else if (lexema=="return") return new Token(Token::RETURN, input, first, current - first);
        
        else if (lexema=="int") return new Token(Token::INT, input, first, current - first);
        else if (lexema=="long") return new Token(Token::LONG, input, first, current - first);
        else if (lexema=="float") return new Token(Token::FLOAT, input, first, current - first);
        else if (lexema=="unsigned") return new Token(Token::UNSIGNED, input, first, current - first);
        else if (lexema=="void") return new Token(Token::VOID, input, first, current - first);
        

        else return new Token(Token::ID, input, first, current - first);
    }

    // Operadores
    else if (strchr("+-*/(){}[];,=!<>\"", c)) { // <-- Cadena actualizada para C
        switch (c) {
            case '+': token = new Token(Token::PLUS,  c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL,   c); break;
            case '/': token = new Token(Token::DIV,   c); break;
            case '(': token = new Token(Token::LPAREN,c); break;
            case ')': token = new Token(Token::RPAREN,c); break;
            case '{': token = new Token(Token::LBRACE,c); break;
            case '}': token = new Token(Token::RBRACE,c); break;
            case ';': token = new Token(Token::SEMICOL,c); break;
            case ',': token = new Token(Token::COMA,c); break;
            case '[': token = new Token(Token::LBRACKET, c); break;
            case ']': token = new Token(Token::RBRACKET, c); break;

            // Tokens compuestos 
            case '=': 
                if (input[current+1]=='=') {
                    current++;
                    token = new Token(Token::EQ, input, first, current + 1 - first);
                } 
                else token = new Token(Token::ASSIGN, c);
                break;
            
            case '!':
                if (input[current+1]=='=') {
                    current++;
                    token = new Token(Token::NEQ, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::ERR, c);
                }
                break;

            case '<':
                if (input[current+1]=='=') {
                    current++;
                    token = new Token(Token::LE, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::LT, c);
                }
                break;

            case '>':
                if (input[current+1]=='=') {
                    current++;
                    token = new Token(Token::GEQ, input, first, current + 1 - first);
                } else {
                    token = new Token(Token::GT, c);
                }
                break;

            case '"':
                while (current + 1 < input.length() && input[current + 1] != '"') {
                    
                    if (input[current + 1] == '\\' && current + 2 < input.length()) {
                        current += 2;
                    } else {
                        current++;
                    }
                }

                if (current + 1 >= input.length()) {
                    token = new Token(Token::ERR, input, first, current + 1 - first);
                    current++;
                } else {
                    current++;
                    token = new Token(Token::LIT_STRING, input, first, current + 1 - first);
                }
                break;
        }
        current++;
    }

    // Carácter inválido
    else {
        token = new Token(Token::ERR, c);
        current++;
    }

    return token;
}




// -----------------------------
// Destructor
// -----------------------------
Scanner::~Scanner() { }

// -----------------------------
// Función de prueba
// -----------------------------

int ejecutar_scanner(Scanner* scanner, const string& InputFile) {
    Token* tok;

    // Crear nombre para archivo de salida
    string OutputFileName = InputFile;
    size_t pos = OutputFileName.find_last_of(".");
    if (pos != string::npos) {
        OutputFileName = OutputFileName.substr(0, pos);
    }
    OutputFileName += "_tokens.txt";

    ofstream outFile(OutputFileName);
    if (!outFile.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << OutputFileName << endl;
        return 0;
    }

    outFile << "Scanner\n" << endl;

    while (true) {
        tok = scanner->nextToken();

        if (tok->type == Token::END) {
            outFile << *tok << endl;
            delete tok;
            outFile << "\nScanner exitoso" << endl << endl;
            outFile.close();
            return 0;
        }

        if (tok->type == Token::ERR) {
            outFile << *tok << endl;
            delete tok;
            outFile << "Caracter invalido" << endl << endl;
            outFile << "Scanner no exitoso" << endl << endl;
            outFile.close();
            return 0;
        }

        outFile << *tok << endl;
        delete tok;
    }
}

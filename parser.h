#ifndef PARSER_H       
#define PARSER_H

#include "scanner.h"    // Incluye la definición del escáner (provee tokens al parser)
#include "ast.h"        // Incluye las definiciones para construir el Árbol de Sintaxis Abstracta (AST)

class Parser {
private:
    Scanner* scanner;       // Puntero al escáner, de donde se leen los tokens
    Token *current, *previous; // Punteros al token actual y al anterior
    bool match(Token::Type ttype);   // Verifica si el token actual coincide con un tipo esperado y avanza si es así
    bool check(Token::Type ttype);   // Comprueba si el token actual es de cierto tipo, sin avanzar
    bool advance();                  // Avanza al siguiente token
    bool isAtEnd();                  // Comprueba si ya se llegó al final de la entrada

public:
    Parser(Scanner* scanner);       //
    Program* parseProgram();      //

    // Reglas de Nivel Superior y Declaraciones
    CType parseTypeSpecifier();     // Parsea 'int', 'float', 'void', etc.
    VarDec* parseVarDec(CType type, string name); // Parsea 'x[10]' (llamado por parseProgram)
    FunDec* parseFunDec(CType type, string name); // Parsea 'main(...){...}' (llamado por parseProgram)
    
    // Reglas de Sentencias (Statements)
    Body* parseBlock();             // Parsea un bloque '{ ... }' (reemplaza a parseBody)
    Stm* parseStm();                // Parsea una sentencia (if, while, return, block, etc.)

    // --- Jerarquía de Expresiones de C (Precedencia) ---
    // (Reemplazan a parseCE, parseBE, parseE, parseT, parseF)

    Exp* parseAssignment();         // Nivel 1: Asignación (ej. x = 10)
    Exp* parseComparison();         // Nivel 2: Comparación (ej. a < b, a == b)
    Exp* parseE();                  // Nivel 3: Suma/Resta (ej. 1 + 2)
    Exp* parseT();                  // Nivel 4: Multiplicación/División (ej. 3 * 4)
    Exp* parseF();                  // Nivel 5: Primitivos (ej. 5, 'id', 'id[0]', '(exp)')
    ForStm* parseForStm();
};

#endif // PARSER_H
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ostream>

using namespace std;

class Token {
public:
    // Tipos de token
    enum Type {
        // --- Operadores ---
        PLUS,    // +
        MINUS,   // -
        MUL,     // *
        DIV,     // /
        
        // --- Asignación y Comparación ---
        ASSIGN,  // =
        EQ,      // ==
        NEQ,     // !=
        LT,      // <
        LE,      // <=
        GT,      // >
        GEQ,     // >=

        // --- Delimitadores ---
        LPAREN,   // (
        RPAREN,   // )
        LBRACE,   // {
        RBRACE,   // }
        LBRACKET, // [
        RBRACKET, // ]
        SEMICOL,  // ;
        COMA,     // ,

        // --- Literales y otros ---
        INT_CONST,      // 123
        LONG_CONST,     // 123L
        UINT_CONST,     // 123U
        FLOAT_CONST,   // 1.23
        LIT_STRING,     // "abc"
        ID,        // miVar
        ERR,       // Error
        END,       // Fin de entrada

        // --- Palabras Clave (Keywords) ---
        RETURN,  // return
        IF,      // if
        ELSE,    // else
        WHILE,   // while
        DO,      // do
        FOR,     // for
        BREAK,
        
        // --- Tipos como Palabras Clave ---
        INT,
        LONG,
        FLOAT,
        UNSIGNED,
        VOID
    };

    // Atributos
    Type type;
    string text;

    // Constructores
    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const string& source, int first, int last);

    // Sobrecarga de operadores de salida
    friend ostream& operator<<(ostream& outs, const Token& tok);
    friend ostream& operator<<(ostream& outs, const Token* tok);
};

#endif // TOKEN_H
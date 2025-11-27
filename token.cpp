#include <iostream>
#include "token.h"

using namespace std;

// -----------------------------
// Constructores
// -----------------------------

Token::Token(Type type) 
    : type(type), text("") { }

Token::Token(Type type, char c) 
    : type(type), text(string(1, c)) { }

Token::Token(Type type, const string& source, int first, int last) 
    : type(type), text(source.substr(first, last)) { }

// -----------------------------
// Sobrecarga de operador <<
// -----------------------------

// Para Token por referencia
ostream& operator<<(ostream& outs, const Token& tok) {
    switch (tok.type) {
        case Token::PLUS:    outs << "TOKEN(PLUS, \""   << tok.text << "\")"; break;
        case Token::MINUS:   outs << "TOKEN(MINUS, \""  << tok.text << "\")"; break;
        case Token::MUL:     outs << "TOKEN(MUL, \""    << tok.text << "\")"; break;
        case Token::DIV:     outs << "TOKEN(DIV, \""    << tok.text << "\")"; break;
        
        case Token::ASSIGN:  outs << "TOKEN(ASSIGN, \"" << tok.text << "\")"; break;
        case Token::EQ:      outs << "TOKEN(EQ, \""     << tok.text << "\")"; break;
        case Token::NEQ:     outs << "TOKEN(NEQ, \""    << tok.text << "\")"; break;
        case Token::LT:      outs << "TOKEN(LT, \""     << tok.text << "\")"; break;
        case Token::LE:      outs << "TOKEN(LE, \""     << tok.text << "\")"; break;
        case Token::GT:      outs << "TOKEN(GT, \""     << tok.text << "\")"; break;
        case Token::GEQ:     outs << "TOKEN(GEQ, \""    << tok.text << "\")"; break;

        case Token::LPAREN:  outs << "TOKEN(LPAREN, \"" << tok.text << "\")"; break;
        case Token::RPAREN:  outs << "TOKEN(RPAREN, \"" << tok.text << "\")"; break;
        case Token::LBRACE:  outs << "TOKEN(LBRACE, \"" << tok.text << "\")"; break;
        case Token::RBRACE:  outs << "TOKEN(RBRACE, \"" << tok.text << "\")"; break;
        case Token::LBRACKET:outs << "TOKEN(LBRACKET, \""<< tok.text << "\")"; break;
        case Token::RBRACKET:outs << "TOKEN(RBRACKET, \""<< tok.text << "\")"; break;
        case Token::SEMICOL: outs << "TOKEN(SEMICOL, \"" << tok.text << "\")"; break;
        case Token::COMA:    outs << "TOKEN(COMA, \""   << tok.text << "\")"; break;

        case Token::INT_CONST: outs << "TOKEN(INT_CONST, \"" << tok.text << "\")"; break;
        case Token::LONG_CONST: outs << "TOKEN(LONG_CONST, \"" << tok.text << "\")"; break;
        case Token::UINT_CONST: outs << "TOKEN(UINT_CONST, \"" << tok.text << "\")"; break;
        case Token::FLOAT_CONST:outs << "TOKEN(FLOAT_CONST, \"" << tok.text << "\")"; break;
        case Token::LIT_STRING:outs << "TOKEN(LIT_STRING, \"" << tok.text << "\")"; break;
        case Token::ID:      outs << "TOKEN(ID, \""     << tok.text << "\")"; break;
        case Token::ERR:     outs << "TOKEN(ERR, \""    << tok.text << "\")"; break;
        case Token::END:     outs << "TOKEN(END)"; break;

        case Token::RETURN:  outs << "TOKEN(RETURN, \"" << tok.text << "\")"; break;
        case Token::IF:      outs << "TOKEN(IF, \""     << tok.text << "\")"; break;
        case Token::ELSE:    outs << "TOKEN(ELSE, \""   << tok.text << "\")"; break;
        case Token::WHILE:   outs << "TOKEN(WHILE, \""  << tok.text << "\")"; break;
        case Token::DO:      outs << "TOKEN(DO, \""     << tok.text << "\")"; break;
        case Token::FOR:      outs << "TOKEN(FOR, \""     << tok.text << "\")"; break;
        case Token::BREAK:      outs << "TOKEN(BREAK, \""     << tok.text << "\")"; break;

        case Token::INT:     outs << "TOKEN(INT, \""    << tok.text << "\")"; break;
        case Token::LONG:    outs << "TOKEN(LONG, \""   << tok.text << "\")"; break;
        case Token::FLOAT:   outs << "TOKEN(FLOAT, \""  << tok.text << "\")"; break;
        case Token::UNSIGNED:outs << "TOKEN(UNSIGNED, \"" << tok.text << "\")"; break;
        case Token::VOID:    outs << "TOKEN(VOID, \""   << tok.text << "\")"; break;
    }
    return outs;
}

// Para Token puntero
ostream& operator<<(ostream& outs, const Token* tok) {
    if (!tok) return outs << "TOKEN(NULL)";
    return outs << *tok;  // delega al otro
}
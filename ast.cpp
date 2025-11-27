#include "visitor.h"
#include "ast.h"
#include <iostream>

using namespace std;

// ------------------ Exp ------------------
Exp::~Exp() {}

string Exp::binopToChar(BinaryOp op) {
    switch (op) {
        case PLUS_OP:  return "+";
        case MINUS_OP: return "-";
        case MUL_OP:   return "*";
        case DIV_OP:   return "/";
        case POW_OP:   return "**";
        case LE_OP:    return "<=";
        case EQ_OP:    return "==";
        case NEQ_OP:   return "!=";
        case LT_OP:    return "<";
        case GT_OP:    return ">";
        case GEQ_OP:   return ">=";
        default:       return "?";
    }
}

// ------------------ BinaryExp ------------------
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp o)
    : left(l), right(r), op(o) {}

    
BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}



// ------------------ NumberExp ------------------
NumberExp::NumberExp(int v) : value(v) {}

NumberExp::~NumberExp() {}


// ------------------idExp ------------------
IdExp::IdExp(string v) : value(v) {}

IdExp::~IdExp() {}


Stm::~Stm(){}

AssignStm::~AssignStm(){}

IfStm::IfStm(Exp* c, Body* t, Body* e): condition(c), then(t), els(e) {}

WhileStm::WhileStm(Exp* c, Body* t): condition(c), b(t) {}

ForStm::ForStm(Stm* init, Exp* cond, Stm* incr, Body* body) 
    : init(init), condition(cond), increment(incr), body(body) {}

ForStm::~ForStm() {
    delete init;
    delete condition;
    delete increment;
    delete body;
}

AssignStm::AssignStm(Exp* lval, Exp* expresion){
    this->lval = lval;
    this->e = expresion;
}


VarDec::VarDec() {}

VarDec::~VarDec() {
    for (Exp* e : dimensions) {
        if (e) delete e;
    }
    for (Exp* e : initValues) {
        if (e) delete e;
    }
}

Body::Body(){
    declarations=list<VarDec*>();
    StmList=list<Stm*>();
}

Body::~Body(){}

FloatExp::FloatExp(double v) : value(v) {
    m_type = CType::TYPE_FLOAT;
}

LongExp::LongExp(long long v) : value(v) {
    m_type = CType::TYPE_LONG;
}

UnsignedExp::UnsignedExp(unsigned int v) : value(v) {
    m_type = CType::TYPE_UNSIGNED;
}

StringExp::StringExp(string v) : value(v) {
    m_type = CType::TYPE_STRING;
}
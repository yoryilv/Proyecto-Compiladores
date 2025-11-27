#ifndef AST_H
#define AST_H

#include <string>
#include <list>
#include <ostream>
#include <vector>
using namespace std;

class Visitor;
class VarDec;

enum CType {
    TYPE_INT,
    TYPE_LONG,
    TYPE_FLOAT,
    TYPE_UNSIGNED,
    TYPE_VOID,
    TYPE_STRING
};

// Operadores binarios soportados
enum BinaryOp { 
    PLUS_OP, 
    MINUS_OP, 
    MUL_OP, 
    DIV_OP,
    POW_OP,
    LE_OP,
    EQ_OP,
    NEQ_OP,
    LT_OP,
    GT_OP,
    GEQ_OP
};

// Clase abstracta Exp
class Exp {
public:
    CType m_type;
    int et = 0;
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToChar(BinaryOp op);
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    int accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();

};

// Expresión numérica
class NumberExp : public Exp {
public:
    int value;
    int accept(Visitor* visitor);
    NumberExp(int v);
    ~NumberExp();
};

class FloatExp : public Exp {
public:
    double value;
    int accept(Visitor* visitor) override;
    FloatExp(double v);
    ~FloatExp() {}
};

class LongExp : public Exp {
public:
    long long value;
    int accept(Visitor* visitor) override;
    LongExp(long long v);
    ~LongExp() {}
};

class UnsignedExp : public Exp {
public:
    unsigned int value;
    int accept(Visitor* visitor) override;
    UnsignedExp(unsigned int v);
    ~UnsignedExp() {}
};

class StringExp: public Exp{
public:
    string value;
    int accept(Visitor* visitor) override;
    StringExp(string v);
    ~StringExp() {}
    
};

// Expresión numérica
class IdExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};


class Stm{
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class VarDec: public Stm{
public:
    CType m_type;
    list<string> vars;
    vector<Exp*> dimensions;
    vector<Exp*> initValues;
    bool isInitialized = false;
    VarDec();
    int accept(Visitor* visitor) override;
    ~VarDec();
};


class Body: public Stm{
public:
    list<Stm*> StmList;
    list<VarDec*> declarations;
    int accept(Visitor* visitor);
    Body();
    ~Body();
};

class IfStm: public Stm {
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStm(Exp* condition, Body* then, Body* els);
    int accept(Visitor* visitor);
    ~IfStm(){};
};

class WhileStm: public Stm {
public:
    Exp* condition;
    Body* b;
    WhileStm(Exp* condition, Body* b);
    int accept(Visitor* visitor);
    ~WhileStm(){};
};

class ForStm : public Stm {
public:
    Stm* init;
    Exp* condition;
    Stm* increment;
    Body* body;

    ForStm(Stm* init, Exp* condition, Stm* increment, Body* body);
    ~ForStm();
    int accept(Visitor* visitor) override;
};

class AssignStm: public Exp {
public:
    Exp* lval; 
    Exp* e;
    AssignStm(Exp*, Exp*);
    ~AssignStm();
    int accept(Visitor* visitor);
};


class ReturnStm: public Stm {
public:
    Exp* e;
    ReturnStm(){};
    ~ReturnStm(){};
    int accept(Visitor* visitor);
};

class FcallExp: public Exp {
public:
    string nombre;
    vector<Exp*> argumentos;
    int accept(Visitor* visitor);
    FcallExp(){};
    ~FcallExp(){};
};


class FunDec{
public:
    string nombre;
    CType tipo;
    Body* cuerpo;
    vector<CType> Ptipos;
    vector<string> Pnombres;
    int accept(Visitor* visitor);
    FunDec(){};
    ~FunDec(){};
};

class ArrayAccessExp : public Exp {
public:
    string id;
    vector<Exp*> indices;
    
    ArrayAccessExp(string id) : id(id) {}
    
    int accept(Visitor* visitor) override;
    ~ArrayAccessExp() {
        for (Exp* e : indices) delete e;
    }
};

class ExpStm : public Stm {
public:
    Exp* e;
    ExpStm(Exp* exp) : e(exp) {}
    ~ExpStm() { delete e; }
    int accept(Visitor* visitor) override;
};

class Program{
public:
    list<VarDec*> vdlist;
    list<FunDec*> fdlist;
    Program(){};
    ~Program(){};
    int accept(Visitor* visitor);
};



#endif // AST_H

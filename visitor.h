#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

class BinaryExp;
class NumberExp;
class FloatExp;
class Program;
class PrintStm;
class WhileStm;
class IfStm;
class ForStm;
class AssignStm;
class Body;
class Vardec;
class FcallExp;
class ReturnStm;
class FunDec;
class ArrayAccessExp;
class StringExp;
class LongExp;
class UnsignedExp;



class Visitor {
public:
    // 
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(FloatExp* exp) = 0; // nuevo para floats 
    virtual int visit(StringExp* exp) = 0;
    virtual int visit(LongExp* exp) = 0;
    virtual int visit(UnsignedExp* exp) = 0;
    virtual int visit(IdExp* exp) = 0;
    virtual int visit(ArrayAccessExp* exp) = 0; // PARA ARRS WA
    virtual int visit(Program* p) = 0;
    virtual int visit(WhileStm* stm) = 0;
    virtual int visit(ForStm* stm) = 0;
    virtual int visit(IfStm* stm) = 0;
    virtual int visit(AssignStm* stm) = 0;
    virtual int visit(ExpStm* stm) = 0;
    virtual int visit(Body* body) = 0;
    virtual int visit(VarDec* vd) = 0;
    virtual int visit(FcallExp* fcall) = 0;
    virtual int visit(ReturnStm* r) = 0;
    virtual int visit(FunDec* fd) = 0;
};

struct VarInfo {
    int offset;
    CType type;
    vector<int> dimensions;
    int totalSize = 1;
};

class TypeCheckVisitor : public Visitor {
public:
    unordered_map<string, VarInfo> memoria;
    unordered_map<string, bool> memoriaGlobal;
    unordered_map<string, CType> globalTypes;
    unordered_map<string, int> fun_memoria;
    int offset = -8;
    CType currentFunReturnType; 
    bool entornoFuncion = false;

    TypeCheckVisitor() {}
    int typeCheck(Program* program);

    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(FloatExp* exp) override;
    int visit(LongExp* exp) override;
    int visit(UnsignedExp* exp) override;
    int visit(StringExp* exp) override;
    int visit(IdExp* exp) override;
    int visit(ArrayAccessExp* exp) override;
    int visit(Program* p) override ;
    int visit(AssignStm* stm) override;
    int visit(WhileStm* stm) override;
    int visit(ForStm* stm) override;
    int visit(IfStm* stm) override;
    int visit(ExpStm* stm) override;
    int visit(Body* body) override;
    int visit(VarDec* vd) override;
    int visit(FcallExp* fcall) override;
    int visit(ReturnStm* r) override;
    int visit(FunDec* fd) override;
};


class GenCodeVisitor : public Visitor {
private:
    std::ostream& out;
    bool enable_tracing = true;
public:
    TypeCheckVisitor type_checker;

    vector<pair<int, double>> float_literals; 
    vector<pair<int, string>> string_literals;

    unordered_map<string, VarInfo> local_vars;
    int gen_offset = -8;

    int labelcont = 0;
    string nombreFuncion;

    void injectLogCall(const std::string& asm_instruction_type, 
                      const std::string& source_context = "", 
                      int line_number = 0);

    void enableTracing(bool enable) { enable_tracing = enable; }
    
    GenCodeVisitor(std::ostream& out) : out(out) {}
    int generar(Program* program);

    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(FloatExp* exp) override;
    int visit(LongExp* exp) override;
    int visit(UnsignedExp* exp) override;
    int visit(StringExp* exp) override;
    int visit(IdExp* exp) override;
    int visit(ArrayAccessExp* exp) override;
    int visit(Program* p) override ;
    int visit(AssignStm* stm) override;
    int visit(WhileStm* stm) override;
    int visit(ForStm* stm) override;
    int visit(IfStm* stm) override;
    int visit(ExpStm* stm) override;
    int visit(Body* body) override;
    int visit(VarDec* vd) override;
    int visit(FcallExp* fcall) override;
    int visit(ReturnStm* r) override;
    int visit(FunDec* fd) override;
};

#endif // VISITOR_H
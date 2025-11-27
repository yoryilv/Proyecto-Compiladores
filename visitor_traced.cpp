#include <iostream>
#define TRACE(msg) std::cerr << "[TRACE] " << msg << "\n"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "ast.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////

int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FloatExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int UnsignedExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int LongExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int StringExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int ArrayAccessExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IfStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int WhileStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int ForStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Body::accept(Visitor* visitor){
    return visitor->visit(this);
}

int VarDec::accept(Visitor* visitor){
    return visitor->visit(this);
}

int FcallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FunDec::accept(Visitor* visitor){
    return visitor->visit(this);
}

int ReturnStm::accept(Visitor* visitor){
    return visitor->visit(this);
}

int ExpStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

///////////////////////////////////////////////////////////////////////////////////

void GenCodeVisitor::injectLogCall(const std::string& asm_instruction_type,
                                   const std::string& source_context,
                                   int line_number) {
    if (!enable_tracing) return;

    out << " # Traza: " << source_context << " (línea " << line_number << ")\n";

    out << "    subq $8, %rsp\n";

    if (!asm_instruction_type.empty()) {
        out << "    leaq .LC_INST_" << labelcont << "(%rip), %rdi\n";
    } else {
        out << "    movq $0, %rdi\n";
    }

    if (!source_context.empty()) {
        out << "    leaq .LC_SRC_" << labelcont << "(%rip), %rsi\n";
    } else {
        out << "    movq $0, %rsi\n";
    }

    out << "    movl $" << line_number << ", %edx\n";
    out << "    call trace_step\n";
    out << "    addq $8, %rsp\n";

    if (!asm_instruction_type.empty() || !source_context.empty()) {
        out << ".section .rodata\n";
        if (!source_context.empty()) {
            out << ".LC_SRC_" << labelcont << ": .string \"" << source_context << "\"\n";
        } else {
            out << ".LC_SRC_" << labelcont << ": .string \"\"\n";
        }
        if (!asm_instruction_type.empty()) {
            out << ".LC_INST_" << labelcont << ": .string \"" << asm_instruction_type << "\"\n";
        } else {
            out << ".LC_INST_" << labelcont << ": .string \"\"\n";
        }
        out << ".text\n";
        labelcont++;
    }
}



int GenCodeVisitor::generar(Program* program) {
    this->type_checker.typeCheck(program); 
    program->accept(this);
    return 0;
}

int GenCodeVisitor::visit(Program* program) {
    TRACE("GenCodeVisitor::Program");

    out << ".data\n"
        << "print_fmt: .string \"%ld \\n\"\n"
        << "print_fmt_float: .string \"%f \\n\"" << endl;
    
    for (auto dec : program->vdlist){
        dec->accept(this);
    }

    out << ".text\n";
    
    if (enable_tracing) {
        out << "# Wrapper de main para inicializar/cerrar tracing\n";
        out << ".globl main\n";
        out << "main:\n";
        out << "    pushq %rbp\n";
        out << "    movq %rsp, %rbp\n";
        out << "    call trace_init\n";
        out << "    call user_main\n";
        out << "    pushq %rax\n";
        out << "    call trace_close\n";
        out << "    popq %rax\n";
        out << "    leave\n";
        out << "    ret\n\n";
    }
    
    for (auto dec : program->fdlist){
        if (enable_tracing && dec->nombre == "main") {
            string original_name = dec->nombre;
            dec->nombre = "user_main";
            out << "# Función main del usuario (renombrada a user_main)\n";
            dec->accept(this);
            dec->nombre = original_name;
        } else {
            dec->accept(this);
        }
    }

    if (!float_literals.empty() || !string_literals.empty()) {
        out << ".section .rodata\n";
        
        for (auto& f : float_literals) {
            out << ".LC_FLT_" << f.first << ": .double " << f.second << endl;
        }
        
        for (auto& s : string_literals) {
            out << ".LC_STR_" << s.first << ": .string \"" << s.second << "\"\n";
        }
    }

    out << ".section .note.GNU-stack,\"\",@progbits" << endl;
    return 0;
}

int GenCodeVisitor::visit(VarDec* stm) {
    TRACE("GenCodeVisitor::VarDec");

    if (stm->vars.empty()) return 0;

    CType type = stm->m_type;
    int init_idx = 0;

    for (const string& varName : stm->vars) {
        if (this->type_checker.memoriaGlobal.count(varName)) {
            out << ".globl " << varName << endl;
            out << ".align 8" << endl;
            
            if (stm->isInitialized && init_idx < stm->initValues.size()) {
                out << varName << ":\n";
                Exp* initExp = stm->initValues[init_idx];
                
                if (type == CType::TYPE_FLOAT) {
                    if (FloatExp* f = dynamic_cast<FloatExp*>(initExp)) 
                        out << " .double " << f->value << endl;
                    else if (NumberExp* n = dynamic_cast<NumberExp*>(initExp)) 
                        out << " .double " << (double)n->value << endl;
                } else {
                    if (NumberExp* n = dynamic_cast<NumberExp*>(initExp)) 
                        out << " .quad " << n->value << endl;
                    else if (LongExp* l = dynamic_cast<LongExp*>(initExp)) 
                        out << " .quad " << l->value << endl;
                    else if (UnsignedExp* u = dynamic_cast<UnsignedExp*>(initExp)) 
                        out << " .quad " << u->value << endl;
                }
                init_idx++;
            } else {
                out << ".comm " << varName << ", 8, 8" << endl;
            }
        } else {
            bool is_array = !stm->dimensions.empty();

            if (is_array) {
                int total_elements = 1;
                vector<int> dims;
                for (Exp* dim_exp : stm->dimensions) {
                    if (NumberExp* num = dynamic_cast<NumberExp*>(dim_exp)) {
                        total_elements *= num->value;
                        dims.push_back(num->value);
                    }
                }
                int array_bytes = total_elements * 8;
                
                gen_offset -= array_bytes;
                int array_start_offset = gen_offset;

                VarInfo info;
                info.offset = array_start_offset;
                info.type = type;
                info.dimensions = dims;
                info.totalSize = total_elements;
                local_vars[varName] = info;
                
                if (stm->isInitialized && !stm->initValues.empty()) {
                    for (size_t i = 0; i < stm->initValues.size() && i < (size_t)total_elements; i++) {
                        Exp* initExp = stm->initValues[i];
                        initExp->accept(this);
                        int element_offset = array_start_offset + (i * 8);
                        
                        if (type == CType::TYPE_FLOAT) {
                            if (initExp->m_type != CType::TYPE_FLOAT) {
                                out << " cvtsi2sdq %rax, %xmm0\n";
                            }
                            out << " movsd %xmm0, " << element_offset << "(%rbp)\n";
                        } else {
                            out << " movq %rax, " << element_offset << "(%rbp)\n";
                        }
                    }
                }
            } 
            else {
                gen_offset -= 8;
                int current_offset = gen_offset;
                
                VarInfo info;
                info.offset = current_offset;
                info.type = type;
                info.dimensions = {};
                info.totalSize = 1;
                local_vars[varName] = info;
                
                if (stm->isInitialized && init_idx < stm->initValues.size()) {
                    Exp* initExp = stm->initValues[init_idx];
                    initExp->accept(this);
                    
                    if (type == CType::TYPE_FLOAT) {
                        if (initExp->m_type != CType::TYPE_FLOAT) {
                            out << " cvtsi2sdq %rax, %xmm0\n";
                        }
                        out << " movsd %xmm0, " << current_offset << "(%rbp)\n";
                    } else {
                        out << " movq %rax, " << current_offset << "(%rbp)\n";
                    }
                    init_idx++;
                }
            }
        }
    }
    return 0;
}

int GenCodeVisitor::visit(NumberExp* exp) {
    TRACE("GenCodeVisitor::NumberExp");
 
    out << " movq $" << exp->value << ", %rax\n";
    injectLogCall("LITERAL_INT_LOAD", "Number literal", 0);
    return 0; 
}

int GenCodeVisitor::visit(LongExp* exp) {
    TRACE("GenCodeVisitor::LongExp");
 
    out << " movq $" << exp->value << ", %rax\n"; 
    injectLogCall("LITERAL_LONG_LOAD", "Long literal", 0);
    return 0; 
}

int GenCodeVisitor::visit(UnsignedExp* exp) {
    TRACE("GenCodeVisitor::UnsignedExp");

    out << " movq $" << exp->value << ", %rax\n";
    injectLogCall("LITERAL_UNSIGNED_LOAD", "Unsigned literal", 0);
    return 0;
}


int GenCodeVisitor::visit(FloatExp* exp) {
    TRACE("GenCodeVisitor::FloatExp");

    int id = labelcont++;
    float_literals.push_back({id, exp->value});
    out << " movsd .LC_FLT_" << id << "(%rip), %xmm0\n";
    return 0;
}

int GenCodeVisitor::visit(StringExp* exp) {
    TRACE("GenCodeVisitor::StringExp");

    int id = labelcont++;
    string_literals.push_back({id, exp->value});
    out << " leaq .LC_STR_" << id << "(%rip), %rax\n";
    return 0;
}

int GenCodeVisitor::visit(IdExp* exp) {
    TRACE("GenCodeVisitor::IdExp");

    string name = exp->value;
    CType type;
    int offset = 0;

    if (this->type_checker.memoriaGlobal.count(name)) {
        type = this->type_checker.globalTypes[name];
        if (type == CType::TYPE_FLOAT){
            out << " movsd " << name << "(%rip), %xmm0\n";
        } else{
            out << " movq " << name << "(%rip), %rax\n";
        }
        exp->m_type = type;
        return 0;
    }

    if (local_vars.count(name)) {
        offset = local_vars[name].offset;
        type = local_vars[name].type;
    } else {
        VarInfo info = this->type_checker.memoria[name];
        offset = info.offset;
        type = info.type;
    }

    if (type == CType::TYPE_FLOAT){
        out << " movsd " << offset << "(%rbp), %xmm0\n";
    } else {
        out << " movq " << offset << "(%rbp), %rax\n";
    }
    
    exp->m_type = type;
    return 0;
}


int GenCodeVisitor::visit(BinaryExp* exp) {
    TRACE("GenCodeVisitor::BinaryExp");

    out << " # DEBUG BinaryExp: left_type=" << exp->left->m_type 
        << ", right_type=" << exp->right->m_type 
        << ", op=" << Exp::binopToChar(exp->op) << endl;

    NumberExp* L = dynamic_cast<NumberExp*>(exp->left);
    NumberExp* R = dynamic_cast<NumberExp*>(exp->right);
    if (L && R) {
        long long res;
        switch (exp->op) {
            case PLUS_OP:  res = L->value + R->value; break;
            case MINUS_OP: res = L->value - R->value; break;
            case MUL_OP:   res = L->value * R->value; break;
            case DIV_OP:   res = R->value != 0 ? L->value / R->value : 0; break;
            default: res = 0;
        }
        out << " # constant folding\n";
        out << " movq $" << res << ", %rax\n";
        exp->m_type = CType::TYPE_LONG;
        injectLogCall("BINARY_CONST_FOLD", string("const fold: ") + Exp::binopToChar(exp->op), 0);
        return 0;
    }

    exp->left->accept(this);
    exp->right->accept(this);

    bool leftIsFloat = (exp->left->m_type == CType::TYPE_FLOAT);
    bool rightIsFloat = (exp->right->m_type == CType::TYPE_FLOAT);
    bool hasFloat = leftIsFloat || rightIsFloat;

    string op_name = string("BINOP_") + Exp::binopToChar(exp->op);

    if (hasFloat) {
        out << " # OPERACIÓN MIXTA INT/FLOAT DETECTADA\n";
        
        if (exp->op == PLUS_OP) {
            if (!leftIsFloat && rightIsFloat) {
                out << " cvtsi2sdq %rax, %xmm1\n";
                out << " addsd %xmm1, %xmm0\n";
                injectLogCall("BINARY_FLOAT_OP", "mixed int+float +", 0);
            }
            else if (leftIsFloat && !rightIsFloat) {
                out << " cvtsi2sdq %rax, %xmm1\n";
                out << " addsd %xmm1, %xmm0\n";
                injectLogCall("BINARY_FLOAT_OP", "mixed float+int +", 0);
            }
            else {
                out << " subq $8, %rsp\n";
                out << " movsd %xmm0, (%rsp)\n";
                exp->left->accept(this);
                out << " movsd (%rsp), %xmm1\n";
                out << " addq $8, %rsp\n";
                out << " addsd %xmm1, %xmm0\n";
                injectLogCall("BINARY_FLOAT_OP", "float +", 0);
            }
            exp->m_type = CType::TYPE_FLOAT;
        } else {
            injectLogCall("BINARY_FLOAT_OP", string("float op ") + Exp::binopToChar(exp->op), 0);
        }
        return 0;
    }

    int Lcost = exp->left->et;
    int Rcost = exp->right->et;

    if (Rcost > Lcost) {
        exp->right->accept(this);
        out << " pushq %rax\n";
        exp->left->accept(this);
        out << " popq %rcx\n";
    } else {
        exp->left->accept(this);
        out << " pushq %rax\n";
        exp->right->accept(this);
        out << " movq %rax, %rcx\n";
        out << " popq %rax\n";
    }

    bool isUnsigned = (exp->left->m_type == CType::TYPE_UNSIGNED || 
                       exp->right->m_type == CType::TYPE_UNSIGNED);
    
    exp->m_type = isUnsigned ? CType::TYPE_UNSIGNED : CType::TYPE_LONG;

    switch (exp->op) {
        case PLUS_OP:
            out << " addq %rcx, %rax\n";
            injectLogCall("BINARY_INT_OP", "add", 0);
            break;

        case MINUS_OP:
            out << " subq %rcx, %rax\n";
            injectLogCall("BINARY_INT_OP", "sub", 0);
            break;

        case MUL_OP:
            out << " imulq %rcx, %rax\n";
            injectLogCall("BINARY_INT_OP", "mul", 0);
            break;

        case DIV_OP:
            out << " cqo\n";
            if (isUnsigned) {
                out << " xorq %rdx, %rdx\n";
                out << " divq %rcx\n";
            } else {
                out << " idivq %rcx\n";
            }
            injectLogCall("BINARY_INT_OP", "div", 0);
            break;

        case GT_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            if (isUnsigned) out << " seta %al\n";
            else out << " setg %al\n";
            out << " movzbq %al, %rax\n";
            injectLogCall("BINARY_INT_OP", "gt", 0);
            exp->m_type = CType::TYPE_LONG;
            break;

        case LT_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            if (isUnsigned) out << " setb %al\n";
            else out << " setl %al\n";
            out << " movzbq %al, %rax\n";
            injectLogCall("BINARY_INT_OP", "lt", 0);
            exp->m_type = CType::TYPE_LONG;
            break;

        case GEQ_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            if (isUnsigned) out << " setae %al\n";
            else out << " setge %al\n";
            out << " movzbq %al, %rax\n";
            injectLogCall("BINARY_INT_OP", "geq", 0);
            exp->m_type = CType::TYPE_LONG;
            break;

        case LE_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            if (isUnsigned) out << " setbe %al\n";
            else out << " setle %al\n";
            out << " movzbq %al, %rax\n";
            injectLogCall("BINARY_INT_OP", "le", 0);
            exp->m_type = CType::TYPE_LONG;
            break;

        case EQ_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " sete %al\n";
            out << " movzbq %al, %rax\n";
            injectLogCall("BINARY_INT_OP", "eq", 0);
            exp->m_type = CType::TYPE_LONG;
            break;

        case NEQ_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " setne %al\n";
            out << " movzbq %al, %rax\n";
            injectLogCall("BINARY_INT_OP", "neq", 0);
            exp->m_type = CType::TYPE_LONG;
            break;
    }
    
    return 0;
}

int GenCodeVisitor::visit(AssignStm* stm) {
    TRACE("GenCodeVisitor::AssignStm");

    stm->e->accept(this);
    CType value_type = stm->e->m_type;

    if (IdExp* lval_id = dynamic_cast<IdExp*>(stm->lval)) {
        string name = lval_id->value;

        if (this->type_checker.memoriaGlobal.count(name)) {
            if (value_type == CType::TYPE_FLOAT) {
                out << " movsd %xmm0, " << name << "(%rip)\n";
                injectLogCall("ASSIGN_FLOAT_VAR", "Assign to global var: " + name, 0);
            } else {
                out << " movq %rax, " << name << "(%rip)\n";
                injectLogCall("ASSIGN_INT_VAR", "Assign to global var: " + name, 0);
            }
        } else {
            if (local_vars.count(name)) {
                int pos = local_vars[name].offset;

                if (value_type == CType::TYPE_FLOAT) {
                    out << " movsd %xmm0, " << pos << "(%rbp)\n";
                    injectLogCall("ASSIGN_FLOAT_VAR", "Assign to local var: " + name, 0);
                } else {
                    out << " movq %rax, " << pos << "(%rbp)\n";
                    injectLogCall("ASSIGN_INT_VAR", "Assign to local var: " + name, 0);
                }
            } else {
                if (this->type_checker.memoria.count(name)) {
                    int pos = this->type_checker.memoria[name].offset;
                    if (value_type == CType::TYPE_FLOAT) {
                        out << " movsd %xmm0, " << pos << "(%rbp)\n";
                        injectLogCall("ASSIGN_FLOAT_VAR", "Assign to mem var: " + name, 0);
                    } else {
                        out << " movq %rax, " << pos << "(%rbp)\n";
                        injectLogCall("ASSIGN_INT_VAR", "Assign to mem var: " + name, 0);
                    }
                }
            }
        }
    }
    else if (ArrayAccessExp* lval_arr = dynamic_cast<ArrayAccessExp*>(stm->lval)) {

        if (value_type == CType::TYPE_FLOAT) {
            out << " movq %xmm0, %rax\n";
            out << " pushq %rax\n";
        } else {
            out << " pushq %rax\n";
        }

        VarInfo info = this->type_checker.memoria[lval_arr->id];
        vector<int> dims = info.dimensions;

        out << " movq $0, %r11\n";
        for (size_t k = 0; k < dims.size(); k++) {
            int D_k = dims[k];
            out << " imulq $" << D_k << ", %r11\n"; 
            lval_arr->indices[k]->accept(this);
            out << " addq %rax, %r11\n";
        }
        out << " imulq $8, %r11\n";

        if (this->type_checker.memoriaGlobal.count(lval_arr->id)) {
             out << " leaq " << lval_arr->id << "(%rip), %r10\n";
        } else {
            int base_offset = this->type_checker.memoria[lval_arr->id].offset; 
            out << " leaq " << base_offset << "(%rbp), %r10\n";
        }
        
        out << " addq %r11, %r10\n";

        out << " popq %rax\n"; 

        if (value_type == CType::TYPE_FLOAT) {
            out << " movq %rax, %xmm0\n";
            out << " movsd %xmm0, (%r10)\n";
            injectLogCall("ASSIGN_FLOAT_ARR", "Assign to array: " + lval_arr->id, 0);
        } else {
            out << " movq %rax, (%r10)\n";
            injectLogCall("ASSIGN_INT_ARR", "Assign to array: " + lval_arr->id, 0);
        }
    }

    return 0;
}


int GenCodeVisitor::visit(ArrayAccessExp* exp) {
    TRACE("GenCodeVisitor::ArrayAccessExp");

    VarInfo info;
    bool isGlobal = false;
    
    if (this->type_checker.memoriaGlobal.count(exp->id)) {
        isGlobal = true;
        info.type = this->type_checker.globalTypes[exp->id];
        throw runtime_error("Arrays globales no soportados aún");
    } else {
        info = this->type_checker.memoria[exp->id];
    }

    CType type = info.type;
    vector<int> dims = info.dimensions;
    int base_offset = info.offset;

    out << " movq $0, %r11\n";
    for (size_t k = 0; k < dims.size(); k++) {
        int D_k = dims[k];
        out << " imulq $" << D_k << ", %r11\n"; 
        exp->indices[k]->accept(this);
        out << " addq %rax, %r11\n";
    }
    out << " imulq $8, %r11\n";

    if (isGlobal) {
        out << " leaq " << exp->id << "(%rip), %rax\n";
    } else {
        out << " leaq " << base_offset << "(%rbp), %rax\n";
    }
    out << " addq %r11, %rax\n";
    
    if (type == CType::TYPE_FLOAT) {
        out << " movsd (%rax), %xmm0\n";
    } else {
        out << " movq (%rax), %rax\n";
    }

    exp->m_type = type;
    return 0;
}


int GenCodeVisitor::visit(ExpStm* stm) {
    TRACE("GenCodeVisitor::ExpStm");
    stm->e->accept(this);
    return 0;
}


int GenCodeVisitor::visit(Body* b) {
    TRACE("GenCodeVisitor::Body");

    for (auto dec : b->declarations) {
        dec->accept(this);
    }

    for (auto s : b->StmList){
        s->accept(this);
    }
    return 0;
}

int GenCodeVisitor::visit(IfStm* stm) {
    TRACE("GenCodeVisitor::IfStm");

    int label = labelcont++;

    stm->condition->accept(this);
    injectLogCall("IF_CONDITION", "Evaluating IF condition", 0);

    out << " cmpq $0, %rax" << endl;
    out << " je else_" << label << endl;

    injectLogCall("IF_THEN", "Entering THEN block", 0);
    stm->then->accept(this);

    out << " jmp endif_" << label << endl;

    out << " else_" << label << ":" << endl;
    if (stm->els) {
        injectLogCall("IF_ELSE", "Entering ELSE block", 0);
        stm->els->accept(this);
    }

    out << "endif_" << label << ":" << endl;
    injectLogCall("IF_END", "Exiting IF", 0);

    return 0;
}


int GenCodeVisitor::visit(WhileStm* stm) {
    TRACE("GenCodeVisitor::WhileStm");

    int label = labelcont++;
    out << "while_" << label << ":"<<endl;
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je endwhile_" << label << endl;
    stm->b->accept(this);
    out << " jmp while_" << label << endl;
    out << "endwhile_" << label << ":"<< endl;
    return 0;
}


int GenCodeVisitor::visit(ForStm* stm) {
    TRACE("GenCodeVisitor::ForStm");

    int label = labelcont++;
    string start_label = "for_start_" + to_string(label);
    string end_label   = "for_end_" + to_string(label);

    stm->init->accept(this);
    injectLogCall("FOR_INIT", "Initializing FOR loop", 0);

    out << start_label << ":" << endl;

    stm->condition->accept(this);
    injectLogCall("FOR_COND", "Evaluating loop condition", 0);

    out << " cmpq $0, %rax" << endl;
    out << " je " << end_label << endl;

    injectLogCall("FOR_BODY", "Executing loop body", 0);
    stm->body->accept(this);

    stm->increment->accept(this);
    injectLogCall("FOR_INC", "Applying increment", 0);

    out << " jmp " << start_label << endl;

    out << end_label << ":" << endl;
    injectLogCall("FOR_END", "Exiting FOR loop", 0);

    return 0;
}


int GenCodeVisitor::visit(ReturnStm* stm) {
    TRACE("GenCodeVisitor::ReturnStm");

    if (stm->e) stm->e->accept(this);

    CType expresion_tipo = stm->e->m_type;
    CType funcion_tipo = this->type_checker.currentFunReturnType;

    if (expresion_tipo == CType::TYPE_LONG && funcion_tipo == CType::TYPE_FLOAT) {
        out << " cvtsi2sdq %rax, %xmm0\n"; 
    } 
    else if (expresion_tipo == CType::TYPE_FLOAT && funcion_tipo == CType::TYPE_LONG) {
        out << " cvttsd2siq %xmm0, %rax\n";
    }

    out << " jmp .end_" << nombreFuncion << endl;
    return 0;
}

int GenCodeVisitor::visit(FunDec* f) {
    TRACE("GenCodeVisitor::FunDec");

    nombreFuncion = f->nombre;
    out << ".globl " << f->nombre << "\n" << f->nombre << ":\n";

    out << "    pushq %rbp\n";
    out << "    movq %rsp, %rbp\n";

    int raw_size = this->type_checker.fun_memoria[f->nombre];
    int aligned_local_size = 0;
    if (raw_size > 0) {
        aligned_local_size = (raw_size + 15) & ~15;
        out << "    subq $" << aligned_local_size << ", %rsp\n";
    }

    local_vars.clear();
    gen_offset = -8;

    vector<string> int_regs   = {"%rdi","%rsi","%rdx","%rcx","%r8","%r9"};
    vector<string> float_regs = {"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7"};

    int i_idx = 0, f_idx = 0;

    for (size_t i = 0; i < f->Pnombres.size(); i++) {
        string name = f->Pnombres[i];
        CType type  = f->Ptipos[i];

        local_vars[name] = {gen_offset, type, {}};

        if (type == CType::TYPE_FLOAT) {
            if (f_idx < (int)float_regs.size())
                out << "    movsd " << float_regs[f_idx++] << ", " << gen_offset << "(%rbp)\n";
        } else {
            if (i_idx < (int)int_regs.size())
                out << "    movq " << int_regs[i_idx++] << ", " << gen_offset << "(%rbp)\n";
        }
        gen_offset -= 8;
    }

    f->cuerpo->accept(this);

    out << ".end_" << f->nombre << ":\n";
    
    out << "    leave\n";
    out << "    ret\n";

    return 0;
}

int GenCodeVisitor::visit(FcallExp* exp) {
    TRACE("GenCodeVisitor::FcallExp");


    out << " # DEBUG FcallExp: nombre=" << exp->nombre << ", nargs=" << exp->argumentos.size() << endl;

    vector<string> int_regs   = {"%rdi","%rsi","%rdx","%rcx","%r8","%r9"};
    vector<string> float_regs = {"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5"};

    int nargs = (int)exp->argumentos.size();

    if (exp->nombre == "printf") {
        out << " # DEBUG: Entrando en caso especial printf" << endl;
        
        if (nargs == 0) {
            out << " xorl %eax, %eax\n";
            out << " call printf@PLT\n";
            exp->m_type = CType::TYPE_INT;
            return 0;
        }

        int num_float_args = 0;
        for (int i = 1; i < nargs; ++i) {
            exp->argumentos[i]->accept(this);
            out << " # DEBUG: argumento " << i << " tipo=" << exp->argumentos[i]->m_type << endl;
            if (exp->argumentos[i]->m_type == CType::TYPE_FLOAT) {
                num_float_args++;
            }
        }

        out << " # DEBUG: num_float_args=" << num_float_args << endl;

        int int_reg_idx   = 1; 
        int float_reg_idx = 0; 
        int stack_bytes_pushed = 0;

        for (int i = 1; i < nargs; ++i) {
            Exp* a = exp->argumentos[i];
            a->accept(this);
            bool isFloat = (a->m_type == CType::TYPE_FLOAT);

            if (isFloat) {
                if (float_reg_idx < (int)float_regs.size()) {
                    out << " movsd %xmm0, " << float_regs[float_reg_idx++] << "\n";
                } else {
                    out << " subq $8, %rsp\n";
                    out << " movsd %xmm0, (%rsp)\n";
                    stack_bytes_pushed += 8;
                }
            } else {
                if (int_reg_idx < (int)int_regs.size()) {
                    out << " movq %rax, " << int_regs[int_reg_idx++] << "\n";
                } else {
                    out << " subq $8, %rsp\n";
                    out << " movq %rax, (%rsp)\n";
                    stack_bytes_pushed += 8;
                }
            }
        }

        Exp* fmt = exp->argumentos[0];
        if (StringExp* s = dynamic_cast<StringExp*>(fmt)) {
            int id = labelcont++;
            string_literals.push_back({id, s->value});
            out << " leaq .LC_STR_" << id << "(%rip), %rdi\n";
        } else {
            fmt->accept(this);
            out << " movq %rax, %rdi\n";
        }

        out << " # DEBUG: Antes de establecer %eax" << endl;
        if (num_float_args > 0) {
            out << " movl $1, %eax\n";
            out << " # DEBUG: Establecido %eax=1 para floats" << endl;
        } else {
            out << " movl $0, %eax\n";
            out << " # DEBUG: Establecido %eax=0 para enteros" << endl;
        }
        
        if (stack_bytes_pushed % 16 != 0) {
            out << " subq $8, %rsp\n";
            stack_bytes_pushed += 8;
        }
        
        out << " call printf@PLT\n";

        if (stack_bytes_pushed > 0) {
            out << " addq $" << stack_bytes_pushed << ", %rsp\n";
        }

        exp->m_type = CType::TYPE_INT;
        return 0;
    }

    int int_reg_idx = 0;
    int float_reg_idx = 0;
    int stack_bytes_pushed = 0;

    for (int i = 0; i < nargs; ++i) {
        Exp* a = exp->argumentos[i];
        a->accept(this);
        bool isFloat = (a->m_type == CType::TYPE_FLOAT);

        if (isFloat) {
            if (float_reg_idx < (int)float_regs.size()) {
                if (float_reg_idx > 0) {
                    out << " movsd %xmm0, " << float_regs[float_reg_idx] << "\n";
                }
                float_reg_idx++;
            } else {
                out << " subq $8, %rsp\n";
                out << " movsd %xmm0, (%rsp)\n";
                stack_bytes_pushed += 8;
            }
        } else {
            if (int_reg_idx < (int)int_regs.size()) {
                out << " movq %rax, " << int_regs[int_reg_idx++] << "\n";
            } else {
                out << " subq $8, %rsp\n";
                out << " movq %rax, (%rsp)\n";
                stack_bytes_pushed += 8;
            }
        }
    }

    if (stack_bytes_pushed % 16 != 0) {
        out << " subq $8, %rsp\n";
        stack_bytes_pushed += 8;
    }

    out << " call " << exp->nombre << "\n";

    if (stack_bytes_pushed > 0) {
        out << " addq $" << stack_bytes_pushed << ", %rsp\n";
    }

    exp->m_type = this->type_checker.globalTypes.count(exp->nombre)
                    ? this->type_checker.globalTypes[exp->nombre]
                    : CType::TYPE_INT;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

int TypeCheckVisitor::typeCheck(Program* program) {
    program->accept(this);
    return 0;
}

int TypeCheckVisitor::visit(Program* p) {
    TRACE("TypeCheckVisitor::Program");

    for (auto dec : p->vdlist){
        dec->accept(this);
    }
    for (auto dec : p->fdlist){
        dec->accept(this);
    }
    return 0;
}

int TypeCheckVisitor::visit(VarDec* stm) {
    TRACE("TypeCheckVisitor::VarDec");

    CType type = stm->m_type; 
    vector<int> dims;
    int total_size = 1;

    for (Exp* dim_exp : stm->dimensions) {
        NumberExp* num_exp = dynamic_cast<NumberExp*>(dim_exp);
        if (!num_exp) {
            throw runtime_error("Las dimensiones del arreglo deben ser enteros literales.");
        }
        int dim_val = num_exp->value;
        if (dim_val <= 0) {
            throw runtime_error("Las dimensiones del arreglo deben ser positivas.");
        }
        dims.push_back(dim_val);
        total_size *= dim_val;
    }

    for (auto var : stm->vars) {
        if (!entornoFuncion) {
            if (globalTypes.count(var)) {
                throw runtime_error("Variable global redefinida: " + var);
            }
            memoriaGlobal[var] = true;
            globalTypes[var] = type;
        } else {
            if (memoria.count(var)) {
                throw runtime_error("Variable local redefinida: " + var);
            }
            
            if (dims.empty()) {
                memoria[var] = {offset, type, {}};
                offset -= 8;
            } else {
                int array_bytes = total_size * 8;
                offset -= array_bytes;
                memoria[var] = {offset, type, dims};
            }
        }
    }

    return 0;
}


int TypeCheckVisitor::visit(NumberExp* exp) {
    TRACE("TypeCheckVisitor::NumberExp");

    exp->m_type = CType::TYPE_INT;
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(LongExp* exp) {
    TRACE("TypeCheckVisitor::LongExp");

    exp->m_type = CType::TYPE_LONG;
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(UnsignedExp* exp) {
    TRACE("TypeCheckVisitor::UnsignedExp");

    exp->m_type = CType::TYPE_UNSIGNED;
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(FloatExp* exp) {
    TRACE("TypeCheckVisitor::FloatExp");

    exp->m_type = CType::TYPE_FLOAT;
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(StringExp* exp) {
    TRACE("TypeCheckVisitor::StringExp");

    exp->m_type = CType::TYPE_STRING;

    return 0;
}


int TypeCheckVisitor::visit(IdExp* exp) {
    TRACE("TypeCheckVisitor::IdExp");

    if (memoria.count(exp->value)) {
        exp->m_type = memoria[exp->value].type;
    } else if (globalTypes.count(exp->value)) {
        exp->m_type = globalTypes[exp->value];
    } else {
        throw runtime_error("Variable no declarada: " + exp->value);
    }
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(BinaryExp* exp) {
    TRACE("TypeCheckVisitor::BinaryExp");

    exp->left->accept(this);
    exp->right->accept(this);

    CType left_type = exp->left->m_type;
    CType right_type = exp->right->m_type;

    if (left_type == CType::TYPE_FLOAT || right_type == CType::TYPE_FLOAT) {
        exp->m_type = CType::TYPE_FLOAT;
    } 
    else if (left_type == CType::TYPE_LONG || right_type == CType::TYPE_LONG) {
        exp->m_type = CType::TYPE_LONG;
    }
    else if (left_type == CType::TYPE_UNSIGNED || right_type == CType::TYPE_UNSIGNED) {
        exp->m_type = CType::TYPE_UNSIGNED;
    }
    else {
        exp->m_type = CType::TYPE_INT;
    }

    int l = exp->left->et;
    int r = exp->right->et;
    if (l == r) {
        exp->et = l + 1;
    } else {
        exp->et = std::max(l, r);
    }

    return 0;
}

int TypeCheckVisitor::visit(AssignStm* stm) {
    TRACE("TypeCheckVisitor::AssignStm");

    stm->lval->accept(this);
    stm->e->accept(this);

    CType lval_type = stm->lval->m_type;
    CType rval_type = stm->e->m_type;

    bool compatible = (lval_type == rval_type) ||
                      (lval_type == CType::TYPE_FLOAT && rval_type == CType::TYPE_LONG) ||
                      (lval_type == CType::TYPE_FLOAT && rval_type == CType::TYPE_INT) ||
                      (lval_type == CType::TYPE_LONG && rval_type == CType::TYPE_INT) ||
                      (lval_type == CType::TYPE_UNSIGNED && rval_type == CType::TYPE_INT);
    
    if (!compatible) {
        throw runtime_error("Tipos incompatibles en asignación.");
    }
    
    stm->m_type = lval_type;
    return 0;
}


int TypeCheckVisitor::visit(ArrayAccessExp* exp) {
    TRACE("TypeCheckVisitor::ArrayAccessExp");

    if (!memoria.count(exp->id) && !memoriaGlobal.count(exp->id) && !globalTypes.count(exp->id)) {
        throw runtime_error("Arreglo no declarado: " + exp->id);
    }

    VarInfo info;
    bool isLocal = false;
    if (memoria.count(exp->id)) {
        info = memoria[exp->id];
        isLocal = true;
    } else {
        if (memoriaGlobal.count(exp->id)) {
            throw runtime_error("Arrays globales no soportados aún: " + exp->id);
        } else {
            throw runtime_error("Arreglo no declarado: " + exp->id);
        }
    }

    if (info.dimensions.empty()) {
        throw runtime_error(exp->id + " no es un arreglo.");
    }

    if (info.dimensions.size() != exp->indices.size()) {
        throw runtime_error("Número incorrecto de dimensiones para el arreglo " + exp->id);
    }

    for (Exp* index_exp : exp->indices) {
        index_exp->accept(this);
        if (index_exp->m_type != CType::TYPE_INT && index_exp->m_type != CType::TYPE_LONG) {
            throw runtime_error("Los índices del arreglo deben ser de tipo entero.");
        }
    }

    exp->m_type = info.type;
    exp->et = 1;
    return 0;
}



int TypeCheckVisitor::visit(ExpStm* stm) {
    TRACE("TypeCheckVisitor::ExpStm");

    stm->e->accept(this);
    return 0;
}

int TypeCheckVisitor::visit(Body* b) {
    TRACE("TypeCheckVisitor::Body");

    for (auto dec : b->declarations){
        dec->accept(this);
    }
    for (auto s : b->StmList){
        s->accept(this);
    }
    return 0;
}

int TypeCheckVisitor::visit(IfStm* stm) {
    TRACE("TypeCheckVisitor::IfStm");

    stm->condition->accept(this);
    stm->then->accept(this);
    if (stm->els) stm->els->accept(this);
    return 0;
}

int TypeCheckVisitor::visit(WhileStm* stm) {
    TRACE("TypeCheckVisitor::WhileStm");

    stm->condition->accept(this);
    stm->b->accept(this);
    return 0;
}

int TypeCheckVisitor::visit(ForStm* stm) {
    TRACE("TypeCheckVisitor::ForStm");

    vector<string> vars_declared_in_loop;

    stm->init->accept(this);
    stm->condition->accept(this);
    stm->increment->accept(this);
    stm->body->accept(this);

    return 0;
}

int TypeCheckVisitor::visit(ReturnStm* stm) {
    TRACE("TypeCheckVisitor::ReturnStm");

    if (stm->e) stm->e->accept(this);
    
    if (!entornoFuncion) {
        throw runtime_error("Sentencia 'return' fuera de una función.");
    }
    
    CType expresion_tipo = stm->e->m_type;
    CType funcion_tipo = currentFunReturnType;

    if (expresion_tipo != funcion_tipo && !(funcion_tipo == CType::TYPE_FLOAT && expresion_tipo == CType::TYPE_LONG)) {
        throw runtime_error("Tipo de retorno no coincide con la firma de la función.");
    }

    return 0;
}

int TypeCheckVisitor::visit(FunDec* f) {
    TRACE("TypeCheckVisitor::FunDec");

    bool old_entorno = entornoFuncion;
    entornoFuncion = true;

    currentFunReturnType = f->tipo;

    globalTypes[f->nombre] = f->tipo;
    memoria.clear();
    offset = -8;

    for(size_t i = 0; i < f->Pnombres.size(); i++) {
        memoria[f->Pnombres[i]] = {offset, f->Ptipos[i], {}};
        offset -= 8;
    }

    f->cuerpo->accept(this);
    fun_memoria[f->nombre] = abs(offset) + 16;
    entornoFuncion = old_entorno;
    return 0;
}



int TypeCheckVisitor::visit(FcallExp* exp) {
    TRACE("TypeCheckVisitor::FcallExp");

    if (!globalTypes.count(exp->nombre)) {
        if (exp->nombre == "printf") {
             exp->m_type = CType::TYPE_INT;
             return 0;
        }
        throw runtime_error("Función no declarada: " + exp->nombre);
    }

    for(auto arg : exp->argumentos) {
        arg->accept(this);
    }

    exp->m_type = globalTypes[exp->nombre];
    exp->et = 1;
    return 0;
}
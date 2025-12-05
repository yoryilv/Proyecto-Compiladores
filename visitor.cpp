#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <unordered_map>
#include "ast.h"
#include "visitor.h"
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

int GenCodeVisitor::generar(Program* program) {
    this->type_checker.typeCheck(program); 
    program->accept(this);
    return 0;
}

int GenCodeVisitor::visit(Program* program) {
    out << ".data\n"
        << "print_fmt: .string \"%ld \\n\"\n"
        << "print_fmt_float: .string \"%f \\n\"" << endl;
    
    for (auto dec : program->vdlist){
        dec->accept(this);
    }

    out << ".text\n";
    
    for (auto dec : program->fdlist){
        dec->accept(this);
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

    if (stm->vars.empty()) return 0;

    CType type = stm->m_type;
    int init_idx = 0;

    int element_size = 8;
    if (type == CType::TYPE_INT || type == CType::TYPE_UNSIGNED) {
        element_size = 4;
    }

    for (const string& varName : stm->vars) {
        if (this->type_checker.memoriaGlobal.count(varName)) {
            out << ".globl " << varName << endl;
            
            out << ".align " << element_size << endl; 
            
            if (stm->isInitialized && init_idx < stm->initValues.size()) {
                out << varName << ":\n";
                Exp* initExp = stm->initValues[init_idx];
                
                if (type == CType::TYPE_FLOAT) {
                    if (FloatExp* f = dynamic_cast<FloatExp*>(initExp)) 
                        out << " .double " << f->value << endl;
                    else if (NumberExp* n = dynamic_cast<NumberExp*>(initExp)) 
                        out << " .double " << (double)n->value << endl;
                } else {
                    if (type == CType::TYPE_INT || type == CType::TYPE_UNSIGNED) {
                        if (NumberExp* n = dynamic_cast<NumberExp*>(initExp)) 
                            out << " .long " << n->value << endl;
                        else if (UnsignedExp* u = dynamic_cast<UnsignedExp*>(initExp)) 
                            out << " .long " << u->value << endl;
                        else
                            out << " .long " << 0 << endl; 
                    } else {
                        if (NumberExp* n = dynamic_cast<NumberExp*>(initExp)) 
                            out << " .quad " << n->value << endl;
                        else if (LongExp* l = dynamic_cast<LongExp*>(initExp)) 
                            out << " .quad " << l->value << endl;
                    }
                }
                init_idx++;
            } else {
                out << ".comm " << varName << ", " << element_size << ", " << element_size << endl;
            }
        } 

        else {
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
                
                int array_bytes = total_elements * element_size;
                
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
                        
                        int element_offset = array_start_offset + (i * element_size);
                        
                        if (type == CType::TYPE_FLOAT) {
                            if (initExp->m_type != CType::TYPE_FLOAT) {
                                out << " cvtsi2sdq %rax, %xmm0\n";
                            }
                            out << " movsd %xmm0, " << element_offset << "(%rbp)\n";
                        } 
                        else if (element_size == 4) {
                            out << " movl %eax, " << element_offset << "(%rbp)\n";
                        }
                        else {
                            out << " movq %rax, " << element_offset << "(%rbp)\n";
                        }
                    }
                }
            }
            else {
                gen_offset -= element_size;
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
                    } 
                    else if (element_size == 4) {
                        out << " movl %eax, " << current_offset << "(%rbp)\n";
                    } 
                    else {
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
    out << " movl $" << exp->value << ", %eax\n";
    if (exp->value < 0) out << " cltq\n";
    return 0; 
}

int GenCodeVisitor::visit(LongExp* exp) {
    out << " movq $" << exp->value << ", %rax\n"; 
    return 0; 
}

int GenCodeVisitor::visit(UnsignedExp* exp) {
    out << " movl $" << exp->value << ", %eax\n";    
    return 0;
}

int GenCodeVisitor::visit(FloatExp* exp) {
    double valor_real = exp->value;
    unsigned long long valor_en_bits;
    
    memcpy(&valor_en_bits, &valor_real, sizeof(double));

    out << " movq $" << valor_en_bits << ", %rax\n";
    out << " movq %rax, %xmm0\n";
    
    return 0;
}

int GenCodeVisitor::visit(StringExp* exp) {
    int id = labelcont++;
    string_literals.push_back({id, exp->value});
    out << " leaq .LC_STR_" << id << "(%rip), %rax\n";
    return 0;
}

int GenCodeVisitor::visit(IdExp* exp) {
    string name = exp->value;
    CType type;
    int offset = 0;
    bool is_global = false;

    if (this->type_checker.memoriaGlobal.count(name)) {
        type = this->type_checker.globalTypes[name];
        is_global = true;
    } 
    else if (local_vars.count(name)) {
        offset = local_vars[name].offset;
        type = local_vars[name].type;
    } else {
        VarInfo info = this->type_checker.memoria[name];
        offset = info.offset;
        type = info.type;
    }

    if (is_global) {
        if (type == CType::TYPE_FLOAT){
            out << " movsd " << name << "(%rip), %xmm0\n";
        } 
        else if (type == CType::TYPE_INT) {
            out << " movslq " << name << "(%rip), %rax\n";
        }
        else if (type == CType::TYPE_UNSIGNED) {
            out << " movl " << name << "(%rip), %eax\n";
        }
        else {
            out << " movq " << name << "(%rip), %rax\n";
        }
    } 
    else {
        if (type == CType::TYPE_FLOAT){
            out << " movsd " << offset << "(%rbp), %xmm0\n";
        } 
        else if (type == CType::TYPE_INT) {
            out << " movslq " << offset << "(%rbp), %rax\n";
        }
        else if (type == CType::TYPE_UNSIGNED) {
            out << " movl " << offset << "(%rbp), %eax\n";
        }
        else {
            out << " movq " << offset << "(%rbp), %rax\n";
        }
    }
    
    exp->m_type = type;
    return 0;
}

int GenCodeVisitor::getWeight(Exp* e) {
    if (!e) return 0;

    if (BinaryExp* bin = dynamic_cast<BinaryExp*>(e)) {
        int leftW = getWeight(bin->left);
        int rightW = getWeight(bin->right);

        if (leftW == rightW) {
            return leftW + 1;
        }
        return max(leftW, rightW);
    }
    return 1; 
}

int GenCodeVisitor::visit(BinaryExp* exp) {

    int wLeft = getWeight(exp->left);
    int wRight = getWeight(exp->right);
    bool swap_order = (wRight > wLeft);

    if (!swap_order) {
        exp->left->accept(this);
        
        if (exp->left->m_type == CType::TYPE_FLOAT) {
            out << " subq $8, %rsp\n";
            out << " movsd %xmm0, (%rsp)\n";
        } else {
            out << " pushq %rax\n";
        }

        exp->right->accept(this); 
    } else {
        exp->right->accept(this);

        if (exp->right->m_type == CType::TYPE_FLOAT) {
            out << " subq $8, %rsp\n";
            out << " movsd %xmm0, (%rsp)\n";
        } else {
            out << " pushq %rax\n";
        }

        exp->left->accept(this);
    }

    bool leftIsFloat = (exp->left->m_type == CType::TYPE_FLOAT);
    bool rightIsFloat = (exp->right->m_type == CType::TYPE_FLOAT);
    bool hasFloat = leftIsFloat || rightIsFloat;

    if (hasFloat) {
        if (!swap_order) {
            if (rightIsFloat) {
                out << " movsd %xmm0, %xmm1\n";
            } else {
                out << " cvtsi2sdq %rax, %xmm1\n";
            }

            if (leftIsFloat) {
                out << " movsd (%rsp), %xmm0\n";
                out << " addq $8, %rsp\n";
            } else {
                out << " popq %rax\n";
                out << " cvtsi2sdq %rax, %xmm0\n";
            }
        } else {
            if (!leftIsFloat) {
                out << " cvtsi2sdq %rax, %xmm0\n";
            }

            if (rightIsFloat) {
                out << " movsd (%rsp), %xmm1\n";
                out << " addq $8, %rsp\n";
            } else {
                out << " popq %rax\n"; 
                out << " cvtsi2sdq %rax, %xmm1\n";
            }
        }

        switch (exp->op) {
            case PLUS_OP:  out << " addsd %xmm1, %xmm0\n"; break;
            case MINUS_OP: out << " subsd %xmm1, %xmm0\n"; break;
            case MUL_OP:   out << " mulsd %xmm1, %xmm0\n"; break;
            case DIV_OP:   out << " divsd %xmm1, %xmm0\n"; break;
            
            case GT_OP:
            case LT_OP:
            case GEQ_OP:
            case LE_OP:
            case EQ_OP:
            case NEQ_OP:
                out << " ucomisd %xmm1, %xmm0\n";
                out << " movl $0, %eax\n";
                switch(exp->op) {
                    case GT_OP:  out << " seta %al\n"; break;
                    case LT_OP:  out << " setb %al\n"; break;
                    case GEQ_OP: out << " setae %al\n"; break;
                    case LE_OP:  out << " setbe %al\n"; break;
                    case EQ_OP:  out << " sete %al\n"; break;
                    case NEQ_OP: out << " setne %al\n"; break;
                    default: break; 
                }
                out << " movzbq %al, %rax\n";
                exp->m_type = CType::TYPE_INT;
                return 0;
            default: break;
        }
        exp->m_type = CType::TYPE_FLOAT;
        return 0;
    }
    
    if (!swap_order) {
        out << " movq %rax, %rcx\n";
        out << " popq %rax\n";
    } else {
        out << " movq %rax, %rcx\n";         
        out << " popq %rax\n";       
        out << " xchgq %rax, %rcx\n";
    }

    bool leftIsLong  = (exp->left->m_type == CType::TYPE_LONG);
    bool rightIsLong = (exp->right->m_type == CType::TYPE_LONG);
    bool leftIsInt   = (exp->left->m_type == CType::TYPE_INT);
    bool rightIsInt  = (exp->right->m_type == CType::TYPE_INT);

    if (leftIsInt && rightIsLong) {
        out << " cltq\n";
    }

    if (rightIsInt && leftIsLong) {
        out << " movslq %ecx, %rcx\n";
    }

    bool isUnsigned = (exp->left->m_type == CType::TYPE_UNSIGNED || 
                       exp->right->m_type == CType::TYPE_UNSIGNED);
    
    if (leftIsLong || rightIsLong) {
        exp->m_type = CType::TYPE_LONG;
    } else {
        exp->m_type = isUnsigned ? CType::TYPE_UNSIGNED : CType::TYPE_INT;
    }
    
    switch (exp->op) {
        case PLUS_OP:  out << " addq %rcx, %rax\n"; break;
        case MINUS_OP: out << " subq %rcx, %rax\n"; break;
        case MUL_OP:   out << " imulq %rcx, %rax\n"; break;
        
        case DIV_OP:
            if (isUnsigned) {
                out << " xorq %rdx, %rdx\n";
                out << " divq %rcx\n";
            } else {
                out << " cqo\n";
                out << " idivq %rcx\n";
            }
            break;

        case GT_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            if (isUnsigned) out << " seta %al\n"; else out << " setg %al\n";
            out << " movzbq %al, %rax\n";
            exp->m_type = CType::TYPE_INT;
            break;

        case LT_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            if (isUnsigned) out << " setb %al\n"; else out << " setl %al\n";
            out << " movzbq %al, %rax\n";
            exp->m_type = CType::TYPE_INT;
            break;

        case GEQ_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            if (isUnsigned) out << " setae %al\n"; else out << " setge %al\n";
            out << " movzbq %al, %rax\n";
            exp->m_type = CType::TYPE_INT;
            break;

        case LE_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            if (isUnsigned) out << " setbe %al\n"; else out << " setle %al\n";
            out << " movzbq %al, %rax\n";
            exp->m_type = CType::TYPE_INT;
            break;

        case EQ_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " sete %al\n";
            out << " movzbq %al, %rax\n";
            exp->m_type = CType::TYPE_INT;
            break;

        case NEQ_OP:
            out << " cmpq %rcx, %rax\n";
            out << " movl $0, %eax\n";
            out << " setne %al\n";
            out << " movzbq %al, %rax\n";
            exp->m_type = CType::TYPE_INT;
            break;
    }
    
    return 0;
}

int GenCodeVisitor::visit(AssignStm* stm) {
    stm->e->accept(this);
    CType value_type = stm->e->m_type;
    bool is_32bit = (value_type == CType::TYPE_INT || value_type == CType::TYPE_UNSIGNED);

    if (IdExp* lval_id = dynamic_cast<IdExp*>(stm->lval)) {
        string name = lval_id->value;

        if (this->type_checker.memoriaGlobal.count(name)) {
            if (value_type == CType::TYPE_FLOAT) {
                out << " movsd %xmm0, " << name << "(%rip)\n";
            } 
            else if (is_32bit) {
                out << " movl %eax, " << name << "(%rip)\n";
            }
            else {
                out << " movq %rax, " << name << "(%rip)\n";
            }
        }
        else {
            int pos = 0;
            if (local_vars.count(name)) {
                pos = local_vars[name].offset;
            } else if (this->type_checker.memoria.count(name)) {
                pos = this->type_checker.memoria[name].offset;
            }

            if (value_type == CType::TYPE_FLOAT) {
                out << " movsd %xmm0, " << pos << "(%rbp)\n";
            } 
            else if (is_32bit) {
                out << " movl %eax, " << pos << "(%rbp)\n";
            }
            else {
                out << " movq %rax, " << pos << "(%rbp)\n";
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

        VarInfo info;
        info = this->type_checker.memoria[lval_arr->id];

        vector<int> dims = info.dimensions;

        out << " movq $0, %r11\n";
        for (size_t k = 0; k < dims.size(); k++) {
            int D_k = dims[k];
            out << " imulq $" << D_k << ", %r11\n"; 
            lval_arr->indices[k]->accept(this);
            out << " addq %rax, %r11\n";
        }

        if (is_32bit) {
            out << " imulq $4, %r11\n";
        } else {
            out << " imulq $8, %r11\n";
        }

        if (this->type_checker.memoriaGlobal.count(lval_arr->id)) {
             out << " leaq " << lval_arr->id << "(%rip), %r10\n";
        } else {
            int base_offset = info.offset; 
            out << " leaq " << base_offset << "(%rbp), %r10\n";
        }
        
        out << " addq %r11, %r10\n";

        out << " popq %rax\n";

        if (value_type == CType::TYPE_FLOAT) {
            out << " movq %rax, %xmm0\n";
            out << " movsd %xmm0, (%r10)\n";
        } 
        else if (is_32bit) {
            out << " movl %eax, (%r10)\n";
        }
        else {
            out << " movq %rax, (%r10)\n";
        }
    }

    return 0;
}


int GenCodeVisitor::visit(ArrayAccessExp* exp) {

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

    if (type == CType::TYPE_INT || type == CType::TYPE_UNSIGNED) {
        out << " imulq $4, %r11\n";
    } else {
        out << " imulq $8, %r11\n";
    }

    if (isGlobal) {
        out << " leaq " << exp->id << "(%rip), %rax\n";
    } else {
        out << " leaq " << base_offset << "(%rbp), %rax\n";
    }
    
    out << " addq %r11, %rax\n"; 
    
    if (type == CType::TYPE_FLOAT) {
        out << " movsd (%rax), %xmm0\n";
    } 
    else if (type == CType::TYPE_INT) {
        out << " movslq (%rax), %rax\n"; 
    }
    else if (type == CType::TYPE_UNSIGNED) {
        out << " movl (%rax), %eax\n";
    }
    else {
        out << " movq (%rax), %rax\n";
    }

    exp->m_type = type;
    return 0;
}


int GenCodeVisitor::visit(ExpStm* stm) {
    stm->e->accept(this);
    return 0;
}


int GenCodeVisitor::visit(Body* b) {

    for (auto dec : b->declarations) {
        dec->accept(this);
    }

    for (auto s : b->StmList){
        s->accept(this);
    }
    return 0;
}

int GenCodeVisitor::visit(IfStm* stm) {

    int label = labelcont++;

    stm->condition->accept(this);

    out << " cmpq $0, %rax" << endl;
    out << " je else_" << label << endl;

    stm->then->accept(this);

    out << " jmp endif_" << label << endl;

    out << " else_" << label << ":" << endl;
    if (stm->els) {
        stm->els->accept(this);
    }

    out << "endif_" << label << ":" << endl;
    return 0;
}


int GenCodeVisitor::visit(WhileStm* stm) {

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

    int label = labelcont++;
    string start_label = "for_start_" + to_string(label);
    string end_label   = "for_end_" + to_string(label);

    stm->init->accept(this);

    out << start_label << ":" << endl;

    stm->condition->accept(this);

    out << " cmpq $0, %rax" << endl;
    out << " je " << end_label << endl;

    stm->body->accept(this);

    stm->increment->accept(this);

    out << " jmp " << start_label << endl;
    out << end_label << ":" << endl;

    return 0;
}


int GenCodeVisitor::visit(ReturnStm* stm) {

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
    gen_offset = 0; 
    vector<string> int_regs_64 = {"%rdi","%rsi","%rdx","%rcx","%r8","%r9"};
    vector<string> int_regs_32 = {"%edi","%esi","%edx","%ecx","%r8d","%r9d"};
    vector<string> float_regs  = {"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5","%xmm6","%xmm7"};

    int i_idx = 0;
    int f_idx = 0;

    for (size_t i = 0; i < f->Pnombres.size(); i++) {
        string name = f->Pnombres[i];
        CType type  = f->Ptipos[i];

        int arg_size = 8;
        if (type == CType::TYPE_INT || type == CType::TYPE_UNSIGNED) {
            arg_size = 4;
        }

        gen_offset -= arg_size;
        local_vars[name] = {gen_offset, type, {}};

        if (type == CType::TYPE_FLOAT) {
            if (f_idx < (int)float_regs.size()) {
                out << "    movsd " << float_regs[f_idx++] << ", " << gen_offset << "(%rbp)\n";
            }
        } 
        else if (type == CType::TYPE_INT || type == CType::TYPE_UNSIGNED) {
            if (i_idx < (int)int_regs_32.size()) {
                out << "    movl " << int_regs_32[i_idx++] << ", " << gen_offset << "(%rbp)\n";
            }
        } 
        else {
            if (i_idx < (int)int_regs_64.size()) {
                out << "    movq " << int_regs_64[i_idx++] << ", " << gen_offset << "(%rbp)\n";
            }
        }
    }

    f->cuerpo->accept(this);

    out << ".end_" << f->nombre << ":\n";
    out << "    leave\n";
    out << "    ret\n";
    return 0;
}

int GenCodeVisitor::visit(FcallExp* exp) {

    vector<string> int_regs   = {"%rdi","%rsi","%rdx","%rcx","%r8","%r9"};
    vector<string> float_regs = {"%xmm0","%xmm1","%xmm2","%xmm3","%xmm4","%xmm5"};

    int nargs = (int)exp->argumentos.size();

    if (exp->nombre == "printf") {
        
        if (nargs == 0) {
            out << " xorl %eax, %eax\n";
            out << " call printf@PLT\n";
            exp->m_type = CType::TYPE_INT;
            return 0;
        }

        int num_float_args = 0;
        for (int i = 1; i < nargs; ++i) {
            if (exp->argumentos[i]->m_type == CType::TYPE_FLOAT) {
                num_float_args++;
            }
        }

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

        if (num_float_args > 0) {
            out << " movl $1, %eax\n";
        } else {
            out << " movl $0, %eax\n";
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

    for (auto dec : p->vdlist){
        dec->accept(this);
    }
    for (auto dec : p->fdlist){
        dec->accept(this);
    }
    return 0;
}

int TypeCheckVisitor::visit(VarDec* stm) {

    CType type = stm->m_type; 
    vector<int> dims;
    int total_elements = 1;


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
        total_elements *= dim_val;
    }

    int element_size = 8;
    if (type == CType::TYPE_INT || type == CType::TYPE_UNSIGNED) {
        element_size = 4;
    }

    for (auto var : stm->vars) {
        if (!entornoFuncion) {
             if (globalTypes.count(var)) throw runtime_error("Redefinicion global: " + var);
             memoriaGlobal[var] = true;
             globalTypes[var] = type;
        } else {
            if (memoria.count(var)) throw runtime_error("Variable local redefinida: " + var);
            
            if (dims.empty()) {
                offset -= element_size;
                memoria[var] = {offset, type, {}};
            } else {
                int array_bytes = total_elements * element_size;
                offset -= array_bytes;
                memoria[var] = {offset, type, dims};
            }
        }
    }

    return 0;
}

int TypeCheckVisitor::visit(NumberExp* exp) {

    exp->m_type = CType::TYPE_INT;
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(LongExp* exp) {
    exp->m_type = CType::TYPE_LONG;
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(UnsignedExp* exp) {

    exp->m_type = CType::TYPE_UNSIGNED;
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(FloatExp* exp) {

    exp->m_type = CType::TYPE_FLOAT;
    exp->et = 1;
    return 0;
}

int TypeCheckVisitor::visit(StringExp* exp) {

    exp->m_type = CType::TYPE_STRING;

    return 0;
}


int TypeCheckVisitor::visit(IdExp* exp) {

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

    stm->e->accept(this);
    return 0;
}

int TypeCheckVisitor::visit(Body* b) {

    for (auto dec : b->declarations){
        dec->accept(this);
    }
    for (auto s : b->StmList){
        s->accept(this);
    }
    return 0;
}

int TypeCheckVisitor::visit(IfStm* stm) {
    stm->condition->accept(this);
    stm->then->accept(this);
    if (stm->els) stm->els->accept(this);
    return 0;
}

int TypeCheckVisitor::visit(WhileStm* stm) {

    stm->condition->accept(this);
    stm->b->accept(this);
    return 0;
}

int TypeCheckVisitor::visit(ForStm* stm) {

    vector<string> vars_declared_in_loop;

    stm->init->accept(this);
    stm->condition->accept(this);
    stm->increment->accept(this);
    stm->body->accept(this);

    return 0;
}

int TypeCheckVisitor::visit(ReturnStm* stm) {

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

    bool old_entorno = entornoFuncion;
    entornoFuncion = true;

    currentFunReturnType = f->tipo;

    globalTypes[f->nombre] = f->tipo;
    memoria.clear();
    offset = 0;

    for(size_t i = 0; i < f->Pnombres.size(); i++) {
        int arg_size = 8;
        if (f->Ptipos[i] == CType::TYPE_INT || f->Ptipos[i] == CType::TYPE_UNSIGNED) {
             arg_size = 4;
        }

        offset -= arg_size;
        memoria[f->Pnombres[i]] = {offset, f->Ptipos[i], {}};
    }

    f->cuerpo->accept(this);
    fun_memoria[f->nombre] = abs(offset);
    entornoFuncion = old_entorno;
    return 0;
}


int TypeCheckVisitor::visit(FcallExp* exp) {
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
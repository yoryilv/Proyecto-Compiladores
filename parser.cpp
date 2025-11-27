#include<iostream>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR)) {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

// =============================
// Reglas gramaticales de C
// =============================

Program* Parser::parseProgram() {
    Program* p = new Program();

    while (!isAtEnd()) {
        
        CType base_type = parseTypeSpecifier();

        if (!match(Token::ID)) {
            throw runtime_error("Se esperaba un nombre (ID) despues del tipo.");
        }
        string name = previous->text;

        if (check(Token::LPAREN)) {
            p->fdlist.push_back(parseFunDec(base_type, name));
        } 
        else {
            p->vdlist.push_back(parseVarDec(base_type, name));
            if (!match(Token::SEMICOL)) {
                 throw runtime_error("Se esperaba ';' despues de la declaracion de variable global.");
            }
        }
    }
    cout << "Parser de C exitoso" << endl;
    return p;
}

// Parsea el especificador de tipo (int, float, etc.)
CType Parser::parseTypeSpecifier() {
    if (match(Token::INT)) return CType::TYPE_INT;
    if (match(Token::LONG)) return CType::TYPE_LONG;
    if (match(Token::FLOAT)) return CType::TYPE_FLOAT;
    if (match(Token::UNSIGNED)) return CType::TYPE_UNSIGNED;
    if (match(Token::VOID)) return CType::TYPE_VOID;
    
    throw runtime_error("Se esperaba un especificador de tipo (int, float, void, etc.)");
}

VarDec* Parser::parseVarDec(CType type, string name) {
    VarDec* vd = new VarDec();
    vd->m_type = type;
    vd->vars.push_back(name);


    if (check(Token::LBRACKET)) {
        while (match(Token::LBRACKET)) {
            vd->dimensions.push_back(parseE());
            
            if (!match(Token::RBRACKET)) {
                throw runtime_error("Se esperaba ']' al final de la dimensión.");
            }
        }
    }

    if (match(Token::ASSIGN)) {
        vd->isInitialized = true;
        
        if (match(Token::LBRACE)) {
            if (!check(Token::RBRACE)) { 
                do {
                    vd->initValues.push_back(parseAssignment()); 
                } while (match(Token::COMA));
            }
            
            if (!match(Token::RBRACE)) throw runtime_error("Se esperaba '}'.");
        } 
        else {
             vd->initValues.push_back(parseAssignment());
        }
    }


    return vd;
}

FunDec* Parser::parseFunDec(CType type, string name) {
    FunDec* fd = new FunDec();
    fd->tipo = type;
    fd->nombre = name;

    if (!match(Token::LPAREN)) throw runtime_error("Se esperaba '('.");


    if (!check(Token::RPAREN)) {
        do {
            CType param_type = parseTypeSpecifier();
            if (!match(Token::ID)) {
                throw runtime_error("Se esperaba un nombre para el parametro.");
            }
            string param_name = previous->text;

            fd->Ptipos.push_back(param_type);
            fd->Pnombres.push_back(param_name);

        } while (match(Token::COMA));
    }

    if (!match(Token::RPAREN)) throw runtime_error("Se esperaba ')'.");
    
    fd->cuerpo = parseBlock(); 
    return fd;
}

Body* Parser::parseBlock() {
    if (!match(Token::LBRACE)) {
        throw runtime_error("Se esperaba '{' al inicio del bloque.");
    }
    
    Body* b = new Body();

    while (!check(Token::RBRACE) && !isAtEnd()) {
        
        if (check(Token::INT) || check(Token::LONG) || check(Token::FLOAT) || check(Token::UNSIGNED)) {
            CType var_type = parseTypeSpecifier();
            if (!match(Token::ID)) throw runtime_error("Se esperaba ID en la declaracion local.");
            string var_name = previous->text;
            
            b->declarations.push_back(parseVarDec(var_type, var_name));
            
            if (!match(Token::SEMICOL)) {
                throw runtime_error("Se esperaba ';' despues de la declaracion local.");
            }
        } 
        else {
            b->StmList.push_back(parseStm());
        }
    }

    if (!match(Token::RBRACE)) {
        throw runtime_error("Se esperaba '}' al final del bloque.");
    }
    return b;
}

// Parsea una sentencia
Stm* Parser::parseStm() {

    if (match(Token::SEMICOL)) {
        return new ExpStm(nullptr);
    }

    
    if (match(Token::IF)) {
        if (!match(Token::LPAREN)) throw runtime_error("Se esperaba '('.");
        Exp* e = parseAssignment();
        if (!match(Token::RPAREN)) throw runtime_error("Se esperaba ')'.");
        
        Body* tb = new Body();
        tb->StmList.push_back(parseStm());
        
        Body* fb = nullptr;
        if (match(Token::ELSE)) {
            fb = new Body();
            fb->StmList.push_back(parseStm());
        }
        return new IfStm(e, tb, fb);
    }
    
    if (match(Token::WHILE)) {
        if (!match(Token::LPAREN)) throw runtime_error("Se esperaba '('.");
        Exp* e = parseAssignment();
        if (!match(Token::RPAREN)) throw runtime_error("Se esperaba ')'.");
        
        Body* tb = new Body();
        tb->StmList.push_back(parseStm());
        return new WhileStm(e, tb);
    }

    if (check(Token::FOR)) {
        return parseForStm();
    }


    if (match(Token::RETURN)) {
        ReturnStm* r = new ReturnStm();
        
        if (check(Token::SEMICOL)) {
            r->e = nullptr;
            match(Token::SEMICOL);
            return r;
        }
        
        r->e = parseAssignment();
        if (!match(Token::SEMICOL)) {
            throw runtime_error("Se esperaba ';' despues de return.");
        }
        return r;
    }

    if (check(Token::LBRACE)) {
        return parseBlock();
    }

    Exp* e = parseAssignment();
    if (!match(Token::SEMICOL)) {
        throw runtime_error("Error sintáctico: Se esperaba ';' despues de la instruccion.");
    }

    if (dynamic_cast<AssignStm*>(e) || dynamic_cast<FcallExp*>(e)) {
        return new ExpStm(e);
    }
    
    throw runtime_error("Sentencia invalida. Se esperaba una asignacion o llamada a funcion.");
}

ForStm* Parser::parseForStm() {
    if (!match(Token::FOR)) {
        throw runtime_error("Se esperaba 'for'.");
    }

    if (!match(Token::LPAREN)) throw runtime_error("Se esperaba '('.");

    Stm* init = nullptr;
    
    if (check(Token::INT) || check(Token::LONG) || check(Token::FLOAT) || check(Token::UNSIGNED)) {
        CType type = parseTypeSpecifier();
        if (!match(Token::ID)) throw runtime_error("Se esperaba ID.");
        string name = previous->text;
        
        init = parseVarDec(type, name); 
    } 
    else {
        init = new ExpStm(parseAssignment());
    }

    if (!match(Token::SEMICOL)) throw runtime_error("Falta ';' despues del init del for.");

    Exp* cond = parseAssignment();
    if (!match(Token::SEMICOL)) throw runtime_error("Falta ';' despues de la condicion.");

    Exp* incrExp = parseAssignment();
    Stm* incr = new ExpStm(incrExp);

    if (!match(Token::RPAREN)) throw runtime_error("Se esperaba ')'.");

    Stm* bodyStm = parseStm();
    Body* body = dynamic_cast<Body*>(bodyStm);
    
    if (!body) {
        body = new Body();
        body->StmList.push_back(bodyStm);
    }

    return new ForStm(init, cond, incr, body);
}



// Nivel 1: Asignación
Exp* Parser::parseAssignment() {
    Exp* l = parseComparison(); // Nivel inferior

    if (match(Token::ASSIGN)) {
        Exp* r = parseAssignment();

        if (dynamic_cast<IdExp*>(l) != nullptr || 
            dynamic_cast<ArrayAccessExp*>(l) != nullptr) {
            
            return new AssignStm(l, r);
        } else {
            throw runtime_error("Lado izquierdo de asignacion invalido.");
        }
    }
    return l; // No era una asignación
}

// Nivel 2: Comparación
Exp* Parser::parseComparison() {
    Exp* l = parseE(); // Nivel inferior
    
    while(match(Token::EQ) || match(Token::NEQ) || match(Token::LT) || 
          match(Token::LE) || match(Token::GT) || match(Token::GEQ)) 
    {
        BinaryOp op;
        if (previous->type == Token::EQ) op = EQ_OP;
        else if (previous->type == Token::NEQ) op = NEQ_OP;
        else if (previous->type == Token::LT) op = LT_OP;
        else if (previous->type == Token::LE) op = LE_OP;
        else if (previous->type == Token::GT) op = GT_OP;
        else if (previous->type == Token::GEQ) op = GEQ_OP;
        
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

// Nivel 3: Suma/Resta
Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

// Nivel 4: Multiplicación/División
Exp* Parser::parseT() {
    Exp* l = parseF();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

// Nivel 5: Primitivos
Exp* Parser::parseF() {
    Exp* e;
    string nom;
    if (match(Token::MINUS)) {
        Exp* operand = parseF(); 
        
        return new BinaryExp(new NumberExp(0), operand, MINUS_OP);
    }
    if (match(Token::INT_CONST)) { 
        try {
            return new NumberExp(stoi(previous->text));
        } catch (const std::out_of_range& e) {
            return new LongExp(stoll(previous->text));
        }
    }
    else if (match(Token::FLOAT_CONST)) {
        return new FloatExp(stod(previous->text));
    }
    else if (match(Token::LIT_STRING)) {
        string val = previous->text.substr(1, previous->text.length() - 2);
        return new StringExp(val);
    }
    else if (match(Token::LPAREN))
    {
        e = parseAssignment();
        if (!match(Token::RPAREN)) throw runtime_error("Se esperaba ')'.");
        return e;
    }
    else if (match(Token::ID)) {
        nom = previous->text;
        
        if (check(Token::LBRACKET)) {
            ArrayAccessExp* arr_acc = new ArrayAccessExp(nom);
            while (match(Token::LBRACKET)) {
                arr_acc->indices.push_back(parseE());
                if (!match(Token::RBRACKET)) {
                    throw runtime_error("Se esperaba ']' en el acceso al arreglo.");
                }
            }
            return arr_acc;
        }

        if(check(Token::LPAREN)) {
            match(Token::LPAREN);
            FcallExp* fcall = new FcallExp();
            fcall->nombre = nom;
            
            if (!check(Token::RPAREN)) {
                do {
                    fcall->argumentos.push_back(parseAssignment());
                } while (match(Token::COMA));
            }
            
            if (!match(Token::RPAREN)) throw runtime_error("Se esperaba ')'.");
            return fcall;
        }
        else {
            return new IdExp(nom);
            }
    }
    else {
        throw runtime_error("Error sintáctico: Se esperaba una expresion.");
    }
}
/*
    <program> ::= <function>
    <function> ::= "int" IDENT "(" <param> ")" "{" <content> "}"
    <param> ::= "void"
    <content> ::= "return" <exp> ";"
    <exp> ::= <factor> | <exp> <binary_op> <exp>
    <factor> ::= NUM_INT | <unary_op> <factor> | "(" <exp> ")"
    <unary_op> ::= "~" | "-" | "!"
    <binary_op> ::= "+" | "-" | "*" | "/" | "%" | "&" | "|" | "^" | "<<" | ">>"
                    "&&" | "||" | "==" | "!=" | "<" | ">" | "<=" | ">="

    <program> ::= <function>
    <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    <param> ::= "KW_VOID"
    <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    <exp> ::= <factor> | <exp> <binary_op> <exp>
    <factor> ::= NUM_INT | <unary_op> <factor> | "OPEN_PAREN" <exp> "CLOSE_PAREN"
    <unary_op> ::= "OP_TILDE" | "OP_NEG" | "OP_LOGIC_NOT"
    <binary_op> ::= "OP_ADD" | "OP_SUB" | "OP_MUL" | "OP_DIV" | "OP_MOD" | "OP_AND" | "OP_OR" | "OP_XOR" | "OP_SHL" | "OP_LSR"
                    "OP_LOGIC_AND" | "OP_LOGIC_OR" | "OP_EQ" | "OP_NE" | "OP_LT" | "OP_GT" | "OP_LE" | "OP_GE"

    <program> ::= <function>
    <function> ::= 2 0 5 <param> 6 7 <content> 8
    <param> ::= 3
    <content> ::= 4 <exp> 9
    <exp> ::= <factor> | <exp> <binary_op> <exp>
    <factor> ::= 1 | <unary_op> <factor> | 5 <exp> 6
    <unary_op> ::= 10 | 12 | 24
    <binary_op> ::= 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23
                    25 | 26 | 27 | 28 | 29 | 30 | 31 | 32

    ---

    FIRST(<program>) = {2}
    FIRST(<function>) = {2}
    FIRST(<param>) = {3}
    FIRST(<content>) = {4}
    FIRST(<exp>) = {1, 5, 10, 12, 24}
    FIRST(<factor>) = {1, 5, 10, 12, 24}
    FIRST(<unary_op>) = {10, 12, 24}
    FIRST(<binary_op>) = {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32}

    FOLLOW(<program>) = {$}
    FOLLOW(<function>) = {$}
    FOLLOW(<param>) = {6}
    FOLLOW(<content>) = {8}
    FOLLOW(<exp>) = {6, 9, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32}
    FOLLOW(<factor>) = {6, 9, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32}
    FOLLOW(<unary_op>) = {1, 5, 10, 12, 24}
    FOLLOW(<binary_op>) = {1, 5, 10, 12, 24}

    ---

    *** removed token list ***
    OPEN_PAREN
    CLOSE_PAREN
    OPEN_BRACE
    CLOSE_BRACE
    PN_SEMI

    *** ast rules ***
    <program> ::= <function>
    <function> ::= "KW_INT" IDENT "KW_VOID" <content>
    <content> ::= "KW_RETURN" <exp>
    <exp> ::= NUM_INT | <unary_op> <exp> | <binary_op> <exp> <exp>
    <unary_op> ::= "OP_TILDE" | "OP_NEG"

*/



#include "gisa_compiler.h"



int symbolCount;
Lexeme nextSymbol;

int token_type[1024];          // 읽어온 lexeme들의 타입을 저장. 
int token_value[1024];          // 읽어온 lexeme들의 값을 저장. 
int cur_line = 0;


int get_priority(int opcode);
Node * p_nt_program(Lexer_result lex_input);
Node * p_nt_function(Lexer_result lex_input);
Node * p_nt_param(Lexer_result lex_input);
Node * p_nt_content(Lexer_result lex_input);
Node * p_nt_exp(Lexer_result lex_input, int min_priority);
Node * p_nt_factor(Lexer_result lex_input);
Node * p_nt_unary_op(Lexer_result lex_input);
Node * p_nt_binary_op(Lexer_result lex_input);



int get_priority(int opcode) {
    switch (opcode) {
        case OP_ADD: {
            return 90;
        }

        case OP_MINUS: {
            return 90;
        }

        case OP_MUL: {
            return 100;
        }

        case OP_DIV: {
            return 100;
        }

        case OP_MOD: {
            return 100;
        }

        case OP_AND: {
            return 50;
        }

        case OP_OR: {
            return 30;
        }

        case OP_XOR: {
            return 40;
        }

        case OP_SHL: {
            return 80;
        }

        case OP_LSR: {
            return 80;
        }

        case OP_LOGIC_AND: {
            return 20;
        }

        case OP_LOGIC_OR: {
            return 15;
        }

        case OP_EQ: {
            return 60;
        }

        case OP_NE: {
            return 60;
        }

        case OP_LT: {
            return 70;
        }

        case OP_GT: {
            return 70;
        }

        case OP_LE: {
            return 70;
        }

        case OP_GE: {
            return 70;
        }

        default: {
            return 0;
        }
    }
}


void get_nextSymbol(Lexer_result lex_input){
    nextSymbol = lex_input.lexeme_list[symbolCount];
    printf("next symbol: %d\n", nextSymbol.token_number);
    symbolCount++;
    //printf("SymbolCount = %d, token_count = %d\n", symbolCount, lex_input.lexeme_count);
    if (symbolCount > (lex_input.lexeme_count + 1)) {
        printf("Parse Error: 제공된 lexeme보다 많은 symbol을 호출하고 있습니다.\n");
        exit(1);
    }
}

void error(int predLex, Lexeme givenLex){
    printf("Parse Error: token number %d인 lexeme가 주어져야 할 차례지만, 제공된 lexeme는 <%d, %d>입니다.\n", predLex, givenLex.token_number, givenLex.token_value);//: %d번 lexeme <%d, %d>를 파싱하는 과정에서 에러 발생");
    exit(1);
}

/*Node * p_terminal(Lexer_result lex_input, int term_num){
    if (nextSymbol.token_number == term_num){
        printf("parsing: %d\n", term_num);

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(0, nextSymbol);
}*/

Node * p_t_IDENT(Lexer_result lex_input){
    if (nextSymbol.token_number == IDENT){
        printf("parsing: IDENT\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(0, nextSymbol);
}

Node * p_t_NUM_INT(Lexer_result lex_input){
    if (nextSymbol.token_number == NUM_INT){
        printf("parsing: NUM_INT\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(1,nextSymbol);
}

Node * p_t_KW_INT(Lexer_result lex_input){
    if (nextSymbol.token_number == KW_INT){
        printf("parsing: KW_INT\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(2, nextSymbol);
}

Node * p_t_KW_VOID(Lexer_result lex_input){
    if (nextSymbol.token_number == KW_VOID){
        printf("parsing: KW_VOID\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(3, nextSymbol);
}

Node * p_t_KW_RETURN(Lexer_result lex_input){
    if (nextSymbol.token_number == KW_RETURN){
        printf("parsing: KW_RETURN\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(4, nextSymbol);
}

Node * p_t_OPEN_PAREN(Lexer_result lex_input){
    if (nextSymbol.token_number == OPEN_PAREN){
        printf("parsing: OPEN_PAREN\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(5, nextSymbol);
}

Node * p_t_CLOSE_PAREN(Lexer_result lex_input){
    if (nextSymbol.token_number == CLOSE_PAREN){
        printf("parsing: CLOSE_PAREN\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(6, nextSymbol);
}

Node * p_t_OPEN_BRACE(Lexer_result lex_input){
    if (nextSymbol.token_number == OPEN_BRACE){
        printf("parsing: OPEN_BRACE\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(7, nextSymbol);
}

Node * p_t_CLOSE_BRACE(Lexer_result lex_input){
    if (nextSymbol.token_number == CLOSE_BRACE){
        printf("parsing: CLOSE_BRACE\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(8, nextSymbol);
}

Node * p_t_PN_SEMI(Lexer_result lex_input){
    if (nextSymbol.token_number == PN_SEMI){
        printf("parsing: PN_SEMI\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(9, nextSymbol);
}

Node * p_t_OP_TILDE(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_TILDE){
        printf("parsing: OP_TILDE\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_NEG(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_MINUS){
        printf("parsing: OP_NEG\n");

        Node * n = node_maker(NULL, NULL, OP_NEG, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_ADD(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_ADD){
        printf("parsing: OP_ADD\n");

        Node * n = node_maker(NULL, NULL, OP_ADD, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_SUB(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_MINUS){
        printf("parsing: OP_SUB\n");

        Node * n = node_maker(NULL, NULL, OP_SUB, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_MUL(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_MUL){
        printf("parsing: OP_MUL\n");

        Node * n = node_maker(NULL, NULL, OP_MUL, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_DIV(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_DIV){
        printf("parsing: OP_DIV\n");

        Node * n = node_maker(NULL, NULL, OP_DIV, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_MOD(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_MOD){
        printf("parsing: OP_MOD\n");

        Node * n = node_maker(NULL, NULL, OP_MOD, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_AND(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_AND){
        printf("parsing: OP_AND\n");

        Node * n = node_maker(NULL, NULL, OP_AND, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_OR(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_OR){
        printf("parsing: OP_OR\n");

        Node * n = node_maker(NULL, NULL, OP_OR, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_XOR(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_XOR){
        printf("parsing: OP_XOR\n");

        Node * n = node_maker(NULL, NULL, OP_XOR, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_SHL(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_SHL){
        printf("parsing: OP_SHL\n");

        Node * n = node_maker(NULL, NULL, OP_SHL, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_LSR(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_LSR){
        printf("parsing: OP_LSR\n");

        Node * n = node_maker(NULL, NULL, OP_LSR, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(11, nextSymbol);
}

Node * p_t_OP_LOGIC_NOT(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_LOGIC_NOT){
        printf("parsing: OP_LOGIC_NOT\n");

        Node * n = node_maker(NULL, NULL, OP_LOGIC_NOT, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_LOGIC_AND(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_LOGIC_AND){
        printf("parsing: OP_LOGIC_AND\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_LOGIC_OR(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_LOGIC_OR){
        printf("parsing: OP_LOGIC_OR\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_EQ(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_EQ){
        printf("parsing: OP_EQ\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_NE(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_NE){
        printf("parsing: OP_NE\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_LT(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_LT){
        printf("parsing: OP_LT\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_GT(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_GT){
        printf("parsing: OP_GT\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_LE(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_LE){
        printf("parsing: OP_LE\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}

Node * p_t_OP_GE(Lexer_result lex_input){
    if (nextSymbol.token_number == OP_GE){
        printf("parsing: OP_GE\n");

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(10, nextSymbol);
}




Node * p_nt_program(Lexer_result lex_input){    // <program> ::= <function>
    if (nextSymbol.token_number == KW_INT) {
        printf("parsing: nt_program\n");
        Node * x1 = p_nt_function(lex_input);
        
        Node * n = node_maker(x1, NULL, NT_PROGRAM, 0);

        return n;
    } else error(2, nextSymbol);
}

Node * p_nt_function(Lexer_result lex_input){   // <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    if (nextSymbol.token_number == KW_INT) {
        printf("parsing: nt_function\n");
        Node * x1 = p_t_KW_INT(lex_input);
        Node * x2 = p_t_IDENT(lex_input);
        Node * x3 = p_t_OPEN_PAREN(lex_input);
        Node * x4 = p_nt_param(lex_input);
        Node * x5 = p_t_CLOSE_PAREN(lex_input);
        Node * x6 = p_t_OPEN_BRACE(lex_input);
        Node * x7 = p_nt_content(lex_input);
        Node * x8 = p_t_CLOSE_BRACE(lex_input);

        x1->brother = x2;
        x2->brother = x4;
        x4->brother = x7;
        
        Node * n = node_maker(x1, NULL, NT_FUNCTION, 0);

        free(x3);
        free(x5);
        free(x6);
        free(x8);

        return n;
    } else error(2, nextSymbol);
}

Node * p_nt_param(Lexer_result lex_input){      // <param> ::= "KW_VOID"
    if (nextSymbol.token_number == KW_VOID) {
        printf("parsing: nt_param\n");
        Node * x1 = p_t_KW_VOID(lex_input);
        
        return x1;
    } else error(3, nextSymbol);
}

Node * p_nt_content(Lexer_result lex_input){    // <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    if (nextSymbol.token_number == KW_RETURN) {
        printf("parsing: nt_content\n");
        Node * x1 = p_t_KW_RETURN(lex_input);
        Node * x2 = p_nt_exp(lex_input, 0);
        Node * x3 = p_t_PN_SEMI(lex_input);

        x1->brother = x2;
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);

        free(x3);

        return n;
    } else error(4, nextSymbol);
}

Node * p_nt_exp(Lexer_result lex_input, int min_priority){        // <exp> ::= <factor> | <exp> <binary_op> <exp>
    if (nextSymbol.token_number == NUM_INT || nextSymbol.token_number == OPEN_PAREN || nextSymbol.token_number == OP_TILDE || nextSymbol.token_number == OP_MINUS || nextSymbol.token_number == OP_LOGIC_NOT) {
        printf("parsing: nt_exp\n");
        Node * left = p_nt_factor(lex_input);
        
        while ((nextSymbol.token_number == OP_ADD || nextSymbol.token_number == OP_MINUS || nextSymbol.token_number == OP_MUL || nextSymbol.token_number == OP_DIV || nextSymbol.token_number == OP_MOD || nextSymbol.token_number == OP_AND || nextSymbol.token_number == OP_OR || nextSymbol.token_number == OP_XOR || nextSymbol.token_number == OP_SHL || nextSymbol.token_number == OP_LSR || nextSymbol.token_number == OP_LOGIC_AND || nextSymbol.token_number == OP_LOGIC_OR || nextSymbol.token_number == OP_EQ || nextSymbol.token_number == OP_NE || nextSymbol.token_number == OP_LT || nextSymbol.token_number == OP_GT || nextSymbol.token_number == OP_LE || nextSymbol.token_number == OP_GE) && (get_priority(nextSymbol.token_number) >= min_priority)) {
            int op_priority = get_priority(nextSymbol.token_number);
            Node * operator = p_nt_binary_op(lex_input);
            Node * right = p_nt_exp(lex_input, (op_priority + 1));

            operator->brother = left;
            left->brother = right;

            Node * new_left = node_maker(operator, NULL, NT_EXP, 0);
            left = new_left;

        }
        
        return left;
    } else error(1, nextSymbol);
}

Node * p_nt_factor(Lexer_result lex_input){        // <factor> ::= NUM_INT | <unary_op> <factor> | "OPEN_PAREN" <exp> "CLOSE_PAREN"
    if (nextSymbol.token_number == NUM_INT) {
        printf("parsing: nt_exp\n");
        Node * x1 = p_t_NUM_INT(lex_input);
        
        return x1;
    } else if (nextSymbol.token_number == OP_TILDE || nextSymbol.token_number == OP_MINUS || nextSymbol.token_number == OP_LOGIC_NOT) {
        printf("parsing: nt_exp\n");
        Node * x1 = p_nt_unary_op(lex_input);
        Node * x2 = p_nt_factor(lex_input);

        x1->brother = x2;

        Node * n = node_maker(x1, NULL, NT_EXP, 0);

        return n;
    } else if (nextSymbol.token_number == OPEN_PAREN) {
        printf("parsing: nt_exp\n");
        Node * x1 = p_t_OPEN_PAREN(lex_input);
        Node * x2 = p_nt_exp(lex_input, 0);
        Node * x3 = p_t_CLOSE_PAREN(lex_input);
        
        free(x1);
        free(x3);

        return x2;
    } else error(1, nextSymbol);
}

Node * p_nt_unary_op(Lexer_result lex_input){      // <unary_op> ::= "OP_TILDE" | "OP_NEG"
    if (nextSymbol.token_number == OP_TILDE) {
        printf("parsing: nt_unary_op\n");
        Node * x1 = p_t_OP_TILDE(lex_input);

        return x1;
    } else if (nextSymbol.token_number == OP_MINUS) {
        printf("parsing: nt_unary_op\n");
        Node * x1 = p_t_OP_NEG(lex_input);

        return x1;
    } else if (nextSymbol.token_number == OP_LOGIC_NOT) {
        printf("parsing: nt_unary_op\n");
        Node * x1 = p_t_OP_LOGIC_NOT(lex_input);

        return x1;
    } else error(3, nextSymbol);
}

Node * p_nt_binary_op(Lexer_result lex_input){      // <binary_op> ::= "OP_ADD" | "OP_SUB" | "OP_MUL" | "OP_DIV" | "OP_MOD"
    printf("parsing: nt_binary_op\n");
    Node * x1;

    switch (nextSymbol.token_number) {
        case OP_ADD: {
            x1 = p_t_OP_ADD(lex_input);
            return x1;
        }
        
        case OP_MINUS: {
            x1 = p_t_OP_SUB(lex_input);
            return x1;
        }
        
        case OP_MUL: {
            x1 = p_t_OP_MUL(lex_input);
            return x1;
        }
        
        case OP_DIV: {
            x1 = p_t_OP_DIV(lex_input);
            return x1;
        }
        
        case OP_MOD: {
            x1 = p_t_OP_MOD(lex_input);
            return x1;
        }
        
        case OP_AND: {
            x1 = p_t_OP_AND(lex_input);
            return x1;
        }
        
        case OP_OR: {
            x1 = p_t_OP_OR(lex_input);
            return x1;
        }
        
        case OP_XOR: {
            x1 = p_t_OP_XOR(lex_input);
            return x1;
        }
        
        case OP_SHL: {
            x1 = p_t_OP_SHL(lex_input);
            return x1;
        }
        
        case OP_LSR: {
            x1 = p_t_OP_LSR(lex_input);
            return x1;
        }

        case OP_LOGIC_AND: {
            x1 = p_t_OP_LOGIC_AND(lex_input);
            return x1;
        }

        case OP_LOGIC_OR: {
            x1 = p_t_OP_LOGIC_OR(lex_input);
            return x1;
        }

        case OP_EQ: {
            x1 = p_t_OP_EQ(lex_input);
            return x1;
        }

        case OP_NE: {
            x1 = p_t_OP_NE(lex_input);
            return x1;
        }

        case OP_LT: {
            x1 = p_t_OP_LT(lex_input);
            return x1;
        }

        case OP_GT: {
            x1 = p_t_OP_GT(lex_input);
            return x1;
        }

        case OP_LE: {
            x1 = p_t_OP_LE(lex_input);
            return x1;
        }

        case OP_GE: {
            x1 = p_t_OP_GE(lex_input);
            return x1;
        }

        default: error(3, nextSymbol);
    }
}




/* **************************************
*****************************************
*****************************************
***********                   ***********
***********    MAIN PARSER    ***********
***********                   ***********
*****************************************
*****************************************
************************************** */

Node * parser(Lexer_result lex_input, char *parse_name)
{
    symbolCount = 0;
    Node * ast_top;
    /*
    char *lex_name;
    char *parse_name;

        // 인자 개수 확인해 파일이름 지정.
        if (argc == 3) {
            lex_name = argv[1];
            parse_name = argv[2];
        } else {
            lex_name = "lexeme.lex";
            parse_name = "ast.parse";
        }
    */


    FILE *parsefp = fopen(parse_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (parsefp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Parse 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", parse_name);
        exit(1);
    }

    
    //lexer_result_printer(lex_input);

    

    printf("\nLexeme 로딩 성공. Parsing 시작.\n\n");

    get_nextSymbol(lex_input);
    ast_top = p_nt_program(lex_input);

    if (nextSymbol.token_number != 999) {
        printf("Parse Error: parsing이 마무리되지 않은 lexeme가 남은 채 parsing이 종료되었습니다. 현재 처리되어야 할 토큰: <%d, %d>\n", nextSymbol.token_number, nextSymbol.token_value);
        exit(0);
    }


    bin_tree_file_printer(ast_top, parsefp);


    printf("Parsing Finished.\n");
    fclose(parsefp);

    bin_tree_printer(ast_top);

    return ast_top;
}
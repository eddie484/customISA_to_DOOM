/*
    <program> ::= <function>
    <function> ::= "int" IDENT "(" <param> ")" "{" <instr_list> "}"
    <param> ::= "void"
    <instr_list> ::= <instr> <instr_list> | ε
    <instr> ::= <content> | <declr>
    <content> ::= "return" <exp> ";" | <exp> ";" | ";"
    <declr> ::= "int" IDENT <assign> ";"
    <assign> ::= "=" <exp> | ε
    <exp> ::= <factor> | <exp> <binary_op> <exp>
    <factor> ::= NUM_INT | IDENT | <unary_op> <factor> | "(" <exp> ")"
    <unary_op> ::= "~" | "-" | "!"
    <binary_op> ::= "+" | "-" | "*" | "/" | "%" | "&" | "|" | "^" | "<<" | ">>"
                    "&&" | "||" | "==" | "!=" | "<" | ">" | "<=" | ">=" | "="

    <program> ::= <function>
    <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <instr_list> "CLOSE_BRACE"
    <param> ::= "KW_VOID"
    <instr_list> ::= <instr> <instr_list> | ε
    <instr> ::= <content> | <declr>
    <content> ::= "KW_RETURN" <exp> "PN_SEMI" | <exp> "PN_SEMI" | "PN_SEMI"
    <declr> ::= "KW_INT" IDENT <assign> "PN_SEMI"
    <assign> ::= "OP_ASSIGN" <exp> | ε
    <exp> ::= <factor> | <exp> <binary_op> <exp>
    <factor> ::= NUM_INT | IDENT | <unary_op> <factor> | "OPEN_PAREN" <exp> "CLOSE_PAREN"
    <unary_op> ::= "OP_TILDE" | "OP_NEG" | "OP_LOGIC_NOT"
    <binary_op> ::= "OP_ADD" | "OP_SUB" | "OP_MUL" | "OP_DIV" | "OP_MOD" | "OP_AND" | "OP_OR" | "OP_XOR" | "OP_SHL" | "OP_LSR"
                    "OP_LOGIC_AND" | "OP_LOGIC_OR" | "OP_EQ" | "OP_NE" | "OP_LT" | "OP_GT" | "OP_LE" | "OP_GE" | "OP_ASSIGN"

    <program> ::= <function>
    <function> ::= 2 0 5 <param> 6 7 <instr_list> 8
    <param> ::= 3
    <instr_list> ::= <instr> <instr_list> | ε
    <instr> ::= <content> | <declr>
    <content> ::= 4 <exp> 9 | <exp> 9 | 9
    <declr> ::= 2 0 <assign> 9
    <assign> ::= 33 <exp> | ε
    <exp> ::= <factor> | <exp> <binary_op> <exp>
    <factor> ::= 1 | 0 | <unary_op> <factor> | 5 <exp> 6
    <unary_op> ::= 10 | 11 | 24
    <binary_op> ::= 11 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23
                    25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 | 33

    ---
새로 배운것: first의 역할을 본인의 first는 여기서 들어올 수 있는 토큰의 경우의 수 역할이고, 어디로 갈진 직접 문법 보고 판단해야 하는것인 줄 알았는데, child의 first를 보고 함수 분기를 정하는 거였다니.. 충격!
    FIRST(<program>) = {2}
    FIRST(<function>) = {2}
    FIRST(<param>) = {3}
    FIRST(<instr_list>) = {0, 1, 2, 4, 5, 9, 10, 11, 24, ε}
    FIRST(<instr>) = {0, 1, 2, 4, 5, 9, 10, 11, 24}
    FIRST(<content>) = {0, 1, 4, 5, 9, 10, 11, 24}
    FIRST(<declr>) = {2}
    FIRST(<assign>) = {33, ε}
    FIRST(<exp>) = {0, 1, 5, 10, 11, 24}
    FIRST(<factor>) = {0, 1, 5, 10, 11, 24}
    FIRST(<unary_op>) = {10, 11, 24}
    FIRST(<binary_op>) = {11, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33}

    FOLLOW(<program>) = {$}
    FOLLOW(<function>) = {$}
    FOLLOW(<param>) = {6}
    FOLLOW(<instr_list>) = {8}
    FOLLOW(<instr>) = {0, 1, 2, 4, 5, 8, 9, 10, 11, 24}
    FOLLOW(<content>) = {0, 1, 2, 4, 5, 8, 9, 10, 11, 24}
    FOLLOW(<declr>) = {0, 1, 2, 4, 5, 8, 9, 10, 11, 24}
    FOLLOW(<assign>) = {9}
    FOLLOW(<exp>) = {6, 9, 11, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33}
    FOLLOW(<factor>) = {6, 9, 11, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33}
    FOLLOW(<unary_op>) = {0, 1, 5, 10, 11, 24}
    FOLLOW(<binary_op>) = {0, 1, 5, 10, 11, 24}

    ---

    *** removed token list ***

    *** ast rules ***
    <program> ::= <function>
    <function> ::= "KW_INT" IDENT "KW_VOID" <instr_list>
    <instr_list> ::= <content> <instr_list> | <declr> <instr_list> | ε
    <content> ::= "KW_RETURN" <exp> | <exp> | ε
    <declr> ::= "KW_INT" IDENT <assign>
    <assign> ::= "OP_ASSIGN" <exp> | ε
    <exp> ::= <factor> | <exp> <binary_op> <exp>
    <factor> ::= NUM_INT | IDENT | <unary_op> <factor> | "OPEN_PAREN" <exp> "CLOSE_PAREN"
    <unary_op> ::= "OP_TILDE" | "OP_NEG" | "OP_LOGIC_NOT"
    <binary_op> ::= "OP_ADD" | "OP_SUB" | "OP_MUL" | "OP_DIV" | "OP_MOD" | "OP_AND" | "OP_OR" | "OP_XOR" | "OP_SHL" | "OP_LSR"
                    "OP_LOGIC_AND" | "OP_LOGIC_OR" | "OP_EQ" | "OP_NE" | "OP_LT" | "OP_GT" | "OP_LE" | "OP_GE" | "OP_ASSIGN"


*/



#include "gisa_compiler.h"



int symbolCount;
Lexeme nextSymbol;

int token_type[1024];          // 읽어온 lexeme들의 타입을 저장. 
int token_value[1024];          // 읽어온 lexeme들의 값을 저장. 
int cur_line = 0;

static const int priority_table[34] = {
    [OP_ADD] = 90,
    [OP_MINUS] = 90,
    [OP_MUL] = 100,
    [OP_DIV] = 100,
    [OP_MOD] = 100,
    [OP_AND] = 50,
    [OP_OR] = 30,
    [OP_XOR] = 40,
    [OP_SHL] = 80,
    [OP_LSR] = 80,
    [OP_LOGIC_AND] = 20,
    [OP_LOGIC_OR] = 15,
    [OP_EQ] = 60,
    [OP_NE] = 60,
    [OP_LT] = 70,
    [OP_GT] = 70,
    [OP_LE] = 70,
    [OP_GE] = 70,
    [OP_ASSIGN] = 5
};


int first(int input_token, int nt_set);
int follow(int input_token, int nt_set);
Node * p_nt_program(Lexer_result lex_input);
Node * p_nt_function(Lexer_result lex_input);
Node * p_nt_param(Lexer_result lex_input);
Node * p_nt_instr_list(Lexer_result lex_input);
Node * p_nt_instr(Lexer_result lex_input);
Node * p_nt_content(Lexer_result lex_input);
Node * p_nt_declr(Lexer_result lex_input);
Node * p_nt_assign(Lexer_result lex_input);
Node * p_nt_exp(Lexer_result lex_input, int min_priority);
Node * p_nt_factor(Lexer_result lex_input);
Node * p_nt_unary_op(Lexer_result lex_input);
Node * p_nt_binary_op(Lexer_result lex_input);



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

int first(int input_token, int nt_set){
    switch(nt_set){
        case NT_PROGRAM:
            return (input_token == 2);
            
        case NT_FUNCTION:
            return (input_token == 2);
            
        case NT_PARAM:
            return (input_token == 3);
            
        case NT_INSTR_LIST:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_INSTR:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_CONTENT:
            return (input_token == 0 || input_token == 1 || input_token == 4 || input_token == 5 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_DECLR:
            return (input_token == 2);
            
        case NT_ASSIGN:
            return (input_token == 33);
            
        case NT_EXP:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_FACTOR:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_UNARY_OP:
            return (input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_BINARY_OP:
            return (input_token == 11 || input_token == 15 || input_token == 16 || input_token == 17 || input_token == 18 || input_token == 19 || input_token == 20 || input_token == 21 || input_token == 22 || input_token == 23 || input_token == 25 || input_token == 26 || input_token == 27 || input_token == 28 || input_token == 29 || input_token == 30 || input_token == 31 || input_token == 32 || input_token == 33);
            
    }
}

int follow(int input_token, int nt_set){
    switch(nt_set){
        case NT_PROGRAM:
            return (input_token == 999);
            
        case NT_FUNCTION:
            return (input_token == 999);
            
        case NT_PARAM:
            return (input_token == 6);
            
        case NT_INSTR_LIST:
            return (input_token == 8);
            
        case NT_INSTR:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_CONTENT:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_DECLR:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_ASSIGN:
            return (input_token == 9);
            
        case NT_EXP:
            return (input_token == 6 || input_token == 9 || input_token == 11 || input_token == 15 || input_token == 16 || input_token == 17 || input_token == 18 || input_token == 19 || input_token == 20 || input_token == 21 || input_token == 22 || input_token == 23 || input_token == 25 || input_token == 26 || input_token == 27 || input_token == 28 || input_token == 29 || input_token == 30 || input_token == 31 || input_token == 32 || input_token == 33);
            
        case NT_FACTOR:
            return (input_token == 6 || input_token == 9 || input_token == 11 || input_token == 15 || input_token == 16 || input_token == 17 || input_token == 18 || input_token == 19 || input_token == 20 || input_token == 21 || input_token == 22 || input_token == 23 || input_token == 25 || input_token == 26 || input_token == 27 || input_token == 28 || input_token == 29 || input_token == 30 || input_token == 31 || input_token == 32 || input_token == 33);
            
        case NT_UNARY_OP:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 24);
            
        case NT_BINARY_OP:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 24);
            
    }
}

Node * p_terminal(Lexer_result lex_input, int term_num){
    if (nextSymbol.token_number == term_num){
        printf("parsing: %d\n", term_num);

        Node * n = node_maker(NULL, NULL, nextSymbol.token_number, nextSymbol.token_value);

        get_nextSymbol(lex_input);

        return n;
    }
    else error(term_num, nextSymbol);
}

Node * p_t_OP_MINUS(Lexer_result lex_input, int term_num){
    Node * n;

    if (term_num == OP_NEG && nextSymbol.token_number == OP_MINUS){
        printf("parsing: OP_NEG\n");
        n = node_maker(NULL, NULL, OP_NEG, nextSymbol.token_value);

    } else if (term_num == OP_SUB && nextSymbol.token_number == OP_MINUS){
        printf("parsing: OP_SUB\n");
        n = node_maker(NULL, NULL, OP_SUB, nextSymbol.token_value);

    } else error(term_num, nextSymbol);

    get_nextSymbol(lex_input);

    return n;
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
        Node * x1 = p_terminal(lex_input, KW_INT);
        Node * x2 = p_terminal(lex_input, IDENT);
        Node * x3 = p_terminal(lex_input, OPEN_PAREN);
        Node * x4 = p_nt_param(lex_input);
        Node * x5 = p_terminal(lex_input, CLOSE_PAREN);
        Node * x6 = p_terminal(lex_input, OPEN_BRACE);
        Node * x7 = p_nt_instr_list(lex_input);
        Node * x8 = p_terminal(lex_input, CLOSE_BRACE);

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
        Node * x1 = p_terminal(lex_input, KW_VOID);
        
        return x1;
    } else error(3, nextSymbol);
}

Node * p_nt_instr_list(Lexer_result lex_input){      // <param> ::= "KW_VOID"
    if (first(nextSymbol.token_number, NT_INSTR)) {
        printf("parsing: nt_instr_list\n");
        Node * x1 = p_nt_instr(lex_input);
        Node * x2 = p_nt_instr_list(lex_input);

        x1->brother = x2;

        return x1;
    } else if (follow(nextSymbol.token_number, NT_INSTR_LIST)) {
        printf("parsing: nt_instr_list_null\n");
        Node * n = node_maker(NULL, NULL, NT_INSTR_LIST, 0);

        return n;
    } else error(3, nextSymbol);
}

Node * p_nt_instr(Lexer_result lex_input){      // <param> ::= "KW_VOID"
    if (first(nextSymbol.token_number, NT_CONTENT)) {
        printf("parsing: nt_instr\n");
        Node * x1 = p_nt_content(lex_input);

        return x1;
    } else if (first(nextSymbol.token_number, NT_DECLR)) {
        printf("parsing: nt_instr\n");
        Node * x1 = p_nt_declr(lex_input);

        return x1;
    } else error(3, nextSymbol);
}

Node * p_nt_content(Lexer_result lex_input){    // <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    if (nextSymbol.token_number == KW_RETURN) {
        printf("parsing: nt_content\n");
        Node * x1 = p_terminal(lex_input, KW_RETURN);
        Node * x2 = p_nt_exp(lex_input, 0);
        Node * x3 = p_terminal(lex_input, PN_SEMI);

        x1->brother = x2;
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);

        free(x3);

        return n;
    } else if (first(nextSymbol.token_number, NT_EXP)) {
        printf("parsing: nt_content\n");
        Node * x1 = p_nt_exp(lex_input, 0);
        Node * x2 = p_terminal(lex_input, PN_SEMI);
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);

        free(x2);

        return n;
    } else if (nextSymbol.token_number == PN_SEMI) {
        printf("parsing: nt_content\n");
        Node * x1 = p_terminal(lex_input, PN_SEMI);
        
        Node * n = node_maker(NULL, NULL, NT_CONTENT, 0);

        free(x1);

        return n;
    } else error(4, nextSymbol);
}

Node * p_nt_declr(Lexer_result lex_input){   // <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    if (nextSymbol.token_number == KW_INT) {
        printf("parsing: nt_declr\n");
        Node * x1 = p_terminal(lex_input, KW_INT);
        Node * x2 = p_terminal(lex_input, IDENT);
        Node * x3 = p_nt_assign(lex_input);
        Node * x4 = p_terminal(lex_input, PN_SEMI);

        x1->brother = x2;
        x2->brother = x3;
        
        Node * n = node_maker(x1, NULL, NT_DECLR, 0);

        free(x4);

        return n;
    } else error(2, nextSymbol);
}

Node * p_nt_assign(Lexer_result lex_input){   // <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    if (nextSymbol.token_number == OP_ASSIGN) {
        printf("parsing: nt_assign\n");
        Node * x1 = p_terminal(lex_input, OP_ASSIGN);
        Node * x2 = p_nt_exp(lex_input, 0);

        x1->brother = x2;
        
        Node * n = node_maker(x1, NULL, NT_ASSIGN, 0);

        return n;
    } else if (follow(nextSymbol.token_number, NT_ASSIGN)) {
        printf("parsing: nt_assign_null\n");
        Node * n = node_maker(NULL, NULL, NT_ASSIGN, 0);

        return n;
    } else error(2, nextSymbol);
}

Node * p_nt_exp(Lexer_result lex_input, int min_priority){        // <exp> ::= <factor> | <exp> <binary_op> <exp>
    if (nextSymbol.token_number == IDENT || nextSymbol.token_number == NUM_INT || nextSymbol.token_number == OPEN_PAREN || first(nextSymbol.token_number, NT_UNARY_OP)) {
        printf("parsing: nt_exp\n");
        Node * left = p_nt_factor(lex_input);
        
        while ((first(nextSymbol.token_number, NT_BINARY_OP)) && (priority_table[nextSymbol.token_number] >= min_priority)) {
            if (nextSymbol.token_number == OP_ASSIGN) {
                int op_priority = priority_table[nextSymbol.token_number];
                Node * operator = p_nt_binary_op(lex_input);
                Node * right = p_nt_exp(lex_input, op_priority);

                operator->brother = left;
                left->brother = right;

                Node * new_left = node_maker(operator, NULL, NT_EXP, 0);
                left = new_left;
            } else {
                int op_priority = priority_table[nextSymbol.token_number];
                Node * operator = p_nt_binary_op(lex_input);
                Node * right = p_nt_exp(lex_input, (op_priority + 1));

                operator->brother = left;
                left->brother = right;

                Node * new_left = node_maker(operator, NULL, NT_EXP, 0);
                left = new_left;
            }
        }
        
        return left;
    } else error(1, nextSymbol);
}

Node * p_nt_factor(Lexer_result lex_input){        // <factor> ::= NUM_INT | <unary_op> <factor> | "OPEN_PAREN" <exp> "CLOSE_PAREN"
    if (nextSymbol.token_number == NUM_INT) {
        printf("parsing: nt_factor\n");
        Node * x1 = p_terminal(lex_input, NUM_INT);

        return x1;
    } else if (nextSymbol.token_number == IDENT) {
        printf("parsing: nt_factor\n");
        Node * x1 = p_terminal(lex_input, IDENT);

        return x1;
    } else if (first(nextSymbol.token_number, NT_UNARY_OP)) {
        printf("parsing: nt_factor\n");
        Node * x1 = p_nt_unary_op(lex_input);
        Node * x2 = p_nt_factor(lex_input);

        x1->brother = x2;

        Node * n = node_maker(x1, NULL, NT_FACTOR, 0);

        return n;
    } else if (nextSymbol.token_number == OPEN_PAREN) {
        printf("parsing: nt_factor\n");
        Node * x1 = p_terminal(lex_input, OPEN_PAREN);
        Node * x2 = p_nt_exp(lex_input, 0);
        Node * x3 = p_terminal(lex_input, CLOSE_PAREN);
        

        free(x1);
        free(x3);

        return x2;
    } else error(1, nextSymbol);
}

Node * p_nt_unary_op(Lexer_result lex_input){      // <unary_op> ::= "OP_TILDE" | "OP_NEG"
    printf("parsing: nt_unary_op\n");
    Node * x1;

    switch (nextSymbol.token_number) {
        case OP_TILDE: case OP_LOGIC_NOT: 
            x1 = p_terminal(lex_input, nextSymbol.token_number);
            return x1;

        case OP_MINUS:
            x1 = p_t_OP_MINUS(lex_input, OP_NEG);
            return x1;

        default: error(3, nextSymbol);
    }
}

Node * p_nt_binary_op(Lexer_result lex_input){      // <binary_op> ::= "OP_ADD" | "OP_SUB" | "OP_MUL" | "OP_DIV" | "OP_MOD"
    printf("parsing: nt_binary_op\n");
    Node * x1;

    switch (nextSymbol.token_number) {
        case OP_ADD: case OP_MUL: case OP_DIV: case OP_MOD: case OP_AND: case OP_OR: case OP_XOR: case OP_SHL: case OP_LSR: case OP_LOGIC_AND: case OP_LOGIC_OR: case OP_EQ: case OP_NE: case OP_LT: case OP_GT: case OP_LE: case OP_GE: case OP_ASSIGN:
            x1 = p_terminal(lex_input, nextSymbol.token_number);
            return x1;
        
        case OP_MINUS:
            x1 = p_t_OP_MINUS(lex_input, OP_SUB);
            return x1;
            
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


    FILE *parsefp = fopen(parse_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (parsefp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Parse 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", parse_name);
        exit(1);
    }


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
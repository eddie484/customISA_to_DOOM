/*
    <program> ::= <function>
    <function> ::= "int" IDENT "(" <param> ")" <block>
    <param> ::= "void"
    <block> ::= "{" <instr_list> "}"
    <instr_list> ::= <instr> <instr_list> | ε
    <instr> ::= <content> | <declr> | <goto> | <label>
    <content> ::= "return" <exp> ";" | <exp> ";" | ";" | "if" "(" <exp> ")" <instr> <else> | <block> | "break" ";" | "continue" ";" | "while" "(" <exp> ")" <instr> | "do" <instr> "while" "(" <exp> ")" ";" | "for" "(" <for_init> <for_exp> ";" <for_exp> ")" <instr>
    <else> ::= "else" <instr> | ε
    <goto> ::= "goto" IDENT ";"
    <label> ::= IDENT ":"
    <declr> ::= "int" IDENT <assign> ";"
    <assign> ::= "=" <exp> | ε
    <for_init> ::= <declr> | <exp> ";" | ";"
    <for_exp> ::= <exp> | ε
    <exp> ::= <factor> | <exp> <binary_op> <exp> | <exp> "?" <exp> ":" <exp>
    <factor> ::= NUM_INT | IDENT <postfix> | <unary_op> <factor> | "(" <exp> ")"
    <postfix> ::= "++" | "--" | ε
    <unary_op> ::= "~" | "-" | "!" | "++" | "--"
    <binary_op> ::= "+" | "-" | "*" | "/" | "%" | "&" | "|" | "^" | "<<" | ">>"
                    "&&" | "||" | "==" | "!=" | "<" | ">" | "<=" | ">=" | "="
                    "+=" | "-=" | "*=" | "/=" | "%=" | "&=" | "|=" | "^=" | ">>=" | "<<="

    <program> ::= <function>
    <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" <block>
    <param> ::= "KW_VOID"
    <block> ::= "OPEN_BRACE" <instr_list> "CLOSE_BRACE"
    <instr_list> ::= <instr> <instr_list> | ε
    <instr> ::= <content> | <declr> | <goto> | <label>
    <content> ::= "KW_RETURN" <exp> "PN_SEMI" | <exp> "PN_SEMI" | "PN_SEMI" | "KW_IF" "OPEN_PAREN" <exp> "CLOSE_PAREN" <instr> <else> | <block> | "KW_BREAK" "PN_SEMI" | "KW_CONTINUE" "PN_SEMI" | "KW_WHILE" "OPEN_PAREN" <exp> "CLOSE_PAREN" <instr> | "KW_DO" <instr> "KW_WHILE" "OPEN_PAREN" <exp> "CLOSE_PAREN" "PN_SEMI" | "KW_FOR" "OPEN_PAREN" <for_init> <for_exp> "PN_SEMI" <for_exp> "CLOSE_PAREN" <instr>
    <else> ::= "KW_ELSE" <instr> | ε
    <goto> ::= "KW_GOTO" IDENT "PN_SEMI"
    <label> ::= IDENT "OP_COLON"
    <declr> ::= "KW_INT" IDENT <assign> "PN_SEMI"
    <assign> ::= "OP_ASSIGN" <exp> | ε
    <for_init> ::= <declr> | <exp> "PN_SEMI" | "PN_SEMI"
    <for_exp> ::= <exp> | ε
    <exp> ::= <factor> | <exp> <binary_op> <exp> | <exp> "OP_QUESTION" <exp> " OP_COLON" <exp>
    <factor> ::= NUM_INT | IDENT <postfix> | <unary_op> <factor> | "OPEN_PAREN" <exp> "CLOSE_PAREN"
    <postfix> ::= "OP_INCREMENT" | "OP_DECREMENT" | ε
    <unary_op> ::= "OP_TILDE" | "OP_MINUS" | "OP_LOGIC_NOT" | "OP_INCREMENT" | "OP_DECREMENT"
    <binary_op> ::= "OP_MINUS" | "OP_ADD" | "OP_MUL" | "OP_DIV" | "OP_MOD" | "OP_AND" | "OP_OR" | "OP_XOR" | "OP_SHL" | "OP_LSR"
                    "OP_LOGIC_AND" | "OP_LOGIC_OR" | "OP_EQ" | "OP_NE" | "OP_LT" | "OP_GT" | "OP_LE" | "OP_GE" | "OP_ASSIGN"
                    "OP_ADDEQ" | "OP_SUBEQ" | "OP_MULEQ" | "OP_DIVEQ" | "OP_MODEQ" | "OP_ANDEQ" | "OP_OREQ" | "OP_XOREQ" | "OP_SHLEQ" | "OP_LSREQ"

    <program> ::= <function>
    <function> ::= 2 0 5 <param> 6 <block>
    <param> ::= 3
    <block> ::= 7 <instr_list> 8
    <instr_list> ::= <instr> <instr_list> | ε
    <instr> ::= <content> | <declr> | <goto> | <label>
    <content> ::= 4 <exp> 9 | <exp> 9 | 9 | 49 5 <exp> 6 <instr> <else> | <block> | 57 9 | 58 9 | 55 5 <exp> 6 <instr> | 54 <instr> 55 5 <exp> 6 9 | 56 5 <for_init> <for_exp> 9 <for_exp> 6 <instr>
    <else> ::= 50 <instr> | ε
    <goto> ::= 53 0 9
    <label> ::= 0 52
    <declr> ::= 2 0 <assign> 9
    <assign> ::= 33 <exp> | ε
    <for_init> ::= <declr> | <exp> 9 | 9
    <for_exp> ::= <exp> | ε
    <exp> ::= <factor> | <exp> <binary_op> <exp> | <exp> 51 <exp> 52 <exp>
    <factor> ::= 1 | 0 <postfix> | <unary_op> <factor> | 5 <exp> 6
    <postfix> ::= 13 | 44 | ε
    <unary_op> ::= 10 | 11 | 13 | 24 | 44
    <binary_op> ::= 11 | 14 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23
                    25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 | 33
                    34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43

    ---

    FIRST(<program>) = {2}
    FIRST(<function>) = {2}
    FIRST(<param>) = {3}
    FIRST(<block>) = {7}
    FIRST(<instr_list>) = {0, 1, 2, 4, 5, 7, 9, 10, 11, 13, 24, 44, 49, 53, 54, 55, 56, 57, 58, ε}
    FIRST(<instr>) = {0, 1, 2, 4, 5, 7, 9, 10, 11, 13, 24, 44, 49, 53, 54, 55, 56, 57, 58}
    FIRST(<content>) = {0, 1, 4, 5, 7, 9, 10, 11, 13, 24, 44, 49, 54, 55, 56, 57, 58}
    FIRST(<else>) = {50, ε}
    FIRST(<goto>) = {53}
    FIRST(<label>) = {0}
    FIRST(<declr>) = {2}
    FIRST(<assign>) = {33, ε}
    FIRST(<for_init>) = {0, 1, 2, 5, 9, 10, 11, 13, 24, 44}
    FIRST(<for_exp>) = {0, 1, 5, 10, 11, 13, 24, 44, ε}
    FIRST(<exp>) = {0, 1, 5, 10, 11, 13, 24, 44}
    FIRST(<factor>) = {0, 1, 5, 10, 11, 13, 24, 44}
    FIRST(<postfix>) = {13, 44, ε}
    FIRST(<unary_op>) = {10, 11, 13, 24, 44}
    FIRST(<binary_op>) = {11, 14, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43}

    FOLLOW(<program>) = {$}
    FOLLOW(<function>) = {$}
    FOLLOW(<param>) = {6}
    FOLLOW(<block>) = {$, 0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 13, 24, 44, 49, 50, 53, 54, 55, 56, 57, 58}
    FOLLOW(<instr_list>) = {8}
    FOLLOW(<instr>) = {0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 13, 24, 44, 49, 50, 53, 54, 55, 56, 57, 58}
    FOLLOW(<content>) = {0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 13, 24, 44, 49, 50, 53, 54, 55, 56, 57, 58}
    FOLLOW(<else>) = {0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 13, 24, 44, 49, 50, 53, 54, 55, 56, 57, 58}
    FOLLOW(<declr>) = {0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 13, 24, 44, 49, 50, 53, 54, 55, 56, 57, 58}
    FOLLOW(<goto>) = {0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 13, 24, 44, 49, 50, 53, 54, 55, 56, 57, 58}
    FOLLOW(<label>) = {0, 1, 2, 4, 5, 7, 8, 9, 10, 11, 13, 24, 44, 49, 50, 53, 54, 55, 56, 57, 58}
    FOLLOW(<assign>) = {9}
    FOLLOW(<for_init>) = {0, 1, 5, 9, 10, 11, 13, 24, 44}
    FOLLOW(<for_exp>) = {6, 9}
    FOLLOW(<exp>) = {6, 9, 11, 14, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 51, 52}
    FOLLOW(<factor>) = {6, 9, 11, 14, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 51, 52}
    FOLLOW(<postfix>) = {6, 9, 11, 14, 16, 17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 51, 52}
    FOLLOW(<unary_op>) = {0, 1, 5, 10, 11, 13, 24, 44}
    FOLLOW(<binary_op>) = {0, 1, 5, 10, 11, 13, 24, 44}

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

int return_exist;

static const int priority_table[52] = {
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
    [OP_ASSIGN] = 5,
    [OP_ADDEQ] = 5,
    [OP_SUBEQ] = 5,
    [OP_MULEQ] = 5,
    [OP_DIVEQ] = 5,
    [OP_MODEQ] = 5,
    [OP_ANDEQ] = 5,
    [OP_OREQ] = 5,
    [OP_XOREQ] = 5,
    [OP_SHLEQ] = 5,
    [OP_LSREQ] = 5,
    [OP_QUESTION] = 10
};


int first(int input_token, int nt_set);
int follow(int input_token, int nt_set);
Node * p_nt_program(Lexer_result lex_input);
Node * p_nt_function(Lexer_result lex_input);
Node * p_nt_param(Lexer_result lex_input);
Node * p_nt_block(Lexer_result lex_input);
Node * p_nt_instr_list(Lexer_result lex_input);
Node * p_nt_instr(Lexer_result lex_input);
Node * p_nt_label(Lexer_result lex_input);
Node * p_nt_goto(Lexer_result lex_input);
Node * p_nt_content(Lexer_result lex_input);
Node * p_nt_else(Lexer_result lex_input);
Node * p_nt_declr(Lexer_result lex_input);
Node * p_nt_assign(Lexer_result lex_input);
Node * p_nt_for_init(Lexer_result lex_input);
Node * p_nt_for_exp(Lexer_result lex_input);
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

Lexeme peek(Lexer_result lex_input){
    Lexeme nnSymbol = lex_input.lexeme_list[symbolCount];
    printf("peek symbol: %d\n", nnSymbol.token_number);
    return nnSymbol;
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
            
        case NT_BLOCK:
            return (input_token == 7);
            
        case NT_INSTR_LIST:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_INSTR:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_CONTENT:
            return (input_token == 0 || input_token == 1 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_ELSE:
            return (input_token == 50);
            
        case NT_GOTO:
            return (input_token == 53);
            
        case NT_LABEL:
            return (input_token == 0);
            
        case NT_DECLR:
            return (input_token == 2);
            
        case NT_ASSIGN:
            return (input_token == 33);
            
        case NT_FOR_INIT:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 5 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44);
            
        case NT_FOR_EXP:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44);
            
        case NT_EXP:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 51 || input_token == 52);
            
        case NT_FACTOR:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 51 || input_token == 52);
            
        case NT_UNARY_OP:
            return (input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44);
            
        case NT_BINARY_OP:
            return (input_token == 11 || input_token == 14 || input_token == 16 || input_token == 17 || input_token == 18 || input_token == 19 || input_token == 20 || input_token == 21 || input_token == 22 || input_token == 23 || input_token == 25 || input_token == 26 || input_token == 27 || input_token == 28 || input_token == 29 || input_token == 30 || input_token == 31 || input_token == 32 || input_token == 33 || input_token == 34 || input_token == 35 || input_token == 36 || input_token == 37 || input_token == 38 || input_token == 39 || input_token == 40 || input_token == 41 || input_token == 42 || input_token == 43);
            
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
            
        case NT_BLOCK:
            return (input_token == 999 || input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 50 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_INSTR_LIST:
            return (input_token == 8);
            
        case NT_INSTR:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 50 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_CONTENT:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 50 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_ELSE:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 50 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_DECLR:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 50 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_GOTO:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 50 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_LABEL:
            return (input_token == 0 || input_token == 1 || input_token == 2 || input_token == 4 || input_token == 5 || input_token == 7 || input_token == 8 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44 || input_token == 49 || input_token == 50 || input_token == 53 || input_token == 54 || input_token == 55 || input_token == 56 || input_token == 57 || input_token == 58);
            
        case NT_ASSIGN:
            return (input_token == 9);
            
        case NT_FOR_INIT:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 9 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44);
            
        case NT_FOR_EXP:
            return (input_token == 6 || input_token == 9);
            
        case NT_EXP:
            return (input_token == 6 || input_token == 9 || input_token == 11 || input_token == 14 || input_token == 16 || input_token == 17 || input_token == 18 || input_token == 19 || input_token == 20 || input_token == 21 || input_token == 22 || input_token == 23 || input_token == 25 || input_token == 26 || input_token == 27 || input_token == 28 || input_token == 29 || input_token == 30 || input_token == 31 || input_token == 32 || input_token == 33 || input_token == 34 || input_token == 35 || input_token == 36 || input_token == 37 || input_token == 38 || input_token == 39 || input_token == 40 || input_token == 41 || input_token == 42 || input_token == 43);
            
        case NT_FACTOR:
            return (input_token == 6 || input_token == 9 || input_token == 11 || input_token == 14 || input_token == 16 || input_token == 17 || input_token == 18 || input_token == 19 || input_token == 20 || input_token == 21 || input_token == 22 || input_token == 23 || input_token == 25 || input_token == 26 || input_token == 27 || input_token == 28 || input_token == 29 || input_token == 30 || input_token == 31 || input_token == 32 || input_token == 33 || input_token == 34 || input_token == 35 || input_token == 36 || input_token == 37 || input_token == 38 || input_token == 39 || input_token == 40 || input_token == 41 || input_token == 42 || input_token == 43);
            
        case NT_UNARY_OP:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44);
            
        case NT_BINARY_OP:
            return (input_token == 0 || input_token == 1 || input_token == 5 || input_token == 10 || input_token == 11 || input_token == 13 || input_token == 24 || input_token == 44);
            
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

Node * p_t_OP_INCRE_DECRE(Lexer_result lex_input, int term_num){    // term_num 0: PRE, 1: POST
    Node * n;

    if (term_num == 0 && nextSymbol.token_number == OP_INCREMENT){
        printf("parsing: OP_PRE_INCRE\n");
        n = node_maker(NULL, NULL, OP_PRE_INCRE, nextSymbol.token_value);

    } else if (term_num == 1 && nextSymbol.token_number == OP_INCREMENT){
        printf("parsing: OP_POST_INCRE\n");
        n = node_maker(NULL, NULL, OP_POST_INCRE, nextSymbol.token_value);

    } else if (term_num == 0 && nextSymbol.token_number == OP_DECREMENT){
        printf("parsing: OP_PRE_DECRE\n");
        n = node_maker(NULL, NULL, OP_PRE_DECRE, nextSymbol.token_value);

    } else if (term_num == 1 && nextSymbol.token_number == OP_DECREMENT){
        printf("parsing: OP_POST_DECRE\n");
        n = node_maker(NULL, NULL, OP_POST_DECRE, nextSymbol.token_value);

    } else printf("Parse Error: token number Increment/Decrement OP인 lexeme가 주어져야 할 차례지만, 제공된 lexeme는 <%d, %d>입니다.\n", nextSymbol.token_number, nextSymbol.token_value);
    

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
        Node * x6 = p_nt_block(lex_input);

        x1->brother = x2;
        x2->brother = x4;
        x4->brother = x6;


        // 수정해야 함. 함수 종료 시점에 직전 토큰이 return이 아니면 return 0을 삽입하도록 만들어야 함.
        if (return_exist == 0) {    // return이 없는 함수일 경우, return 0;을 삽입한다.
            printf("Parsing 도중 return이 발견되지 않았습니다. return 0;을 삽입합니다.\n");
            Node * content = node_maker(NULL, NULL, NT_CONTENT, 0);
            Node * ret = node_maker(NULL, NULL, KW_RETURN, 0);
            Node * zero = node_maker(NULL, NULL, NUM_INT, lexval_manager ("0"));

            Node * brother = x6->son->brother;
            while ((brother->brother != NULL) && (brother->brother->token.token_number != NT_INSTR_LIST)) {
                brother = brother->brother;
            }

            content->son = ret;
            ret->brother = zero;

            if (brother->brother == NULL) {     // 빈 block일 때. brother가 instr_list를 가리키고 있음. 따라서 return이 brother 앞에 와야함.
                brother->son = content->son;
                brother->brother = content;
                brother->token = content->token;

                content->son = NULL;
                content->brother = NULL;
                content->token.token_number = NT_INSTR_LIST;
                content->token.token_value = 0;

            } else {    // 비지 않은 block일 때
                content->brother = brother->brother;
                brother->brother = content;
            }
        }
        
        Node * n = node_maker(x1, NULL, NT_FUNCTION, 0);

        free(x3);
        free(x5);

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

Node * p_nt_block(Lexer_result lex_input){      // <param> ::= "KW_VOID"
    if (first(nextSymbol.token_number, NT_BLOCK)) {
        printf("parsing: nt_block\n");
        Node * x1 = p_terminal(lex_input, OPEN_BRACE);
        Node * x2 = p_nt_instr_list(lex_input);
        Node * x3 = p_terminal(lex_input, CLOSE_BRACE);
        
        free(x1);
        free(x3);
        
        Node * n = node_maker(x2, NULL, NT_BLOCK, 0);

        return n;
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
    if (first(nextSymbol.token_number, NT_LABEL) && peek(lex_input).token_number == OP_COLON) {
        printf("parsing: nt_label\n");
        Node * x1 = p_nt_label(lex_input);
        
        return x1;
    } else if (first(nextSymbol.token_number, NT_GOTO)) {
        printf("parsing: nt_goto\n");
        Node * x1 = p_nt_goto(lex_input);
        
        return x1;
    } else if (first(nextSymbol.token_number, NT_CONTENT)) {
        printf("parsing: nt_instr\n");
        Node * x1 = p_nt_content(lex_input);

        return x1;
    } else if (first(nextSymbol.token_number, NT_DECLR)) {
        printf("parsing: nt_instr\n");
        Node * x1 = p_nt_declr(lex_input);

        return x1;
    } else error(3, nextSymbol);
}

Node * p_nt_label(Lexer_result lex_input){    // <label> ::= IDENT "OP_COLON"
    if (nextSymbol.token_number == IDENT) {
        printf("parsing: nt_label\n");
        Node * x1 = p_terminal(lex_input, IDENT);
        Node * x2 = p_terminal(lex_input, OP_COLON);

        free(x2);
        
        Node * n = node_maker(x1, NULL, NT_LABEL, 0);

        return n;
    }
}

Node * p_nt_goto(Lexer_result lex_input){    // <goto> ::= "KW_GOTO" IDENT "PN_SEMI"
    if (nextSymbol.token_number == KW_GOTO) {
        printf("parsing: nt_goto\n");
        Node * x1 = p_terminal(lex_input, KW_GOTO);
        Node * x2 = p_terminal(lex_input, IDENT);
        Node * x3 = p_terminal(lex_input, PN_SEMI);

        x1->brother = x2;

        free(x3);
    
        Node * n = node_maker(x1, NULL, NT_GOTO, 0);

        return n;
    }
}

Node * p_nt_content(Lexer_result lex_input){    // <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    if (nextSymbol.token_number == KW_RETURN) {
        printf("parsing: nt_content: KW_RETURN\n");
        return_exist = 1;
        Node * x1 = p_terminal(lex_input, KW_RETURN);
        Node * x2 = p_nt_exp(lex_input, 0);
        Node * x3 = p_terminal(lex_input, PN_SEMI);

        x1->brother = x2;
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);

        free(x3);

        return n;
    } else if (first(nextSymbol.token_number, NT_EXP)) {
        printf("parsing: nt_content: NT_EXP\n");
        Node * x1 = p_nt_exp(lex_input, 0);
        Node * x2 = p_terminal(lex_input, PN_SEMI);
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);

        free(x2);

        return n;
    } else if (nextSymbol.token_number == PN_SEMI) {
        printf("parsing: nt_content: PN_SEMI\n");
        Node * x1 = p_terminal(lex_input, PN_SEMI);
        
        Node * n = node_maker(NULL, NULL, NT_CONTENT, 0);

        free(x1);

        return n;
    } else if (nextSymbol.token_number == KW_IF) {
        printf("parsing: nt_content: KW_IF\n");
        Node * x1 = p_terminal(lex_input, KW_IF);
        Node * x2 = p_terminal(lex_input, OPEN_PAREN);
        Node * x3 = p_nt_exp(lex_input, 0);
        Node * x4 = p_terminal(lex_input, CLOSE_PAREN);
        Node * x5 = p_nt_instr(lex_input);
        Node * x6 = p_nt_else(lex_input);

        x1->brother = x3;
        x3->brother = x5;
        x5->brother = x6;
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);
        
        free(x2);
        free(x4);

        return n;
    } else if (first(nextSymbol.token_number, NT_BLOCK)) {
        printf("parsing: nt_content: NT_BLOCK\n");
        Node * x1 = p_nt_block(lex_input);
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);
        
        return n;
    } else if (nextSymbol.token_number == KW_BREAK) {
        printf("parsing: nt_content: KW_BREAK\n");
        Node * x1 = p_terminal(lex_input, KW_BREAK);
        Node * x2 = p_terminal(lex_input, PN_SEMI);

        free(x2);

        return x1;
    } else if (nextSymbol.token_number == KW_CONTINUE) {
        printf("parsing: nt_content: KW_CONTINUE\n");
        Node * x1 = p_terminal(lex_input, KW_CONTINUE);
        Node * x2 = p_terminal(lex_input, PN_SEMI);

        free(x2);

        return x1;
    } else if (nextSymbol.token_number == KW_WHILE) {
        printf("parsing: nt_content: KW_WHILE\n");
        Node * x1 = p_terminal(lex_input, KW_WHILE);
        Node * x2 = p_terminal(lex_input, OPEN_PAREN);
        Node * x3 = p_nt_exp(lex_input, 0);
        Node * x4 = p_terminal(lex_input, CLOSE_PAREN);
        Node * x5 = p_nt_instr(lex_input);

        x1->son = x3;
        x3->brother = x5;
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);

        free(x2);
        free(x4);

        return n;
    } else if (nextSymbol.token_number == KW_DO) {
        printf("parsing: nt_content: KW_DO\n");
        Node * x1 = p_terminal(lex_input, KW_DO);
        Node * x2 = p_nt_instr(lex_input);
        Node * x3 = p_terminal(lex_input, KW_WHILE);
        Node * x4 = p_terminal(lex_input, OPEN_PAREN);
        Node * x5 = p_nt_exp(lex_input, 0);
        Node * x6 = p_terminal(lex_input, CLOSE_PAREN);
        Node * x7 = p_terminal(lex_input, PN_SEMI);

        x1->son = x2;
        x2->brother = x5;
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);

        free(x3);
        free(x4);
        free(x6);
        free(x7);

        return n;
    } else if (nextSymbol.token_number == KW_FOR) {
        printf("parsing: nt_content: KW_FOR\n");
        Node * x1 = p_terminal(lex_input, KW_FOR);
        Node * x2 = p_terminal(lex_input, OPEN_PAREN);
        Node * x3 = p_nt_for_init(lex_input);
        Node * x4 = p_nt_for_exp(lex_input);
        Node * x5 = p_terminal(lex_input, PN_SEMI);
        Node * x6 = p_nt_for_exp(lex_input);
        Node * x7 = p_terminal(lex_input, CLOSE_PAREN);
        Node * x8 = p_nt_instr(lex_input);
        
        Node * for_init_block = node_maker(x3, NULL, NT_BLOCK, 0);

        x1->son = for_init_block;
        x3->brother = x4;
        x4->brother = x6;
        x6->brother = x8;
        
        Node * n = node_maker(x1, NULL, NT_CONTENT, 0);

        free(x2);
        free(x5);
        free(x7);

        return n;
    } else error(4, nextSymbol);
}

Node * p_nt_else(Lexer_result lex_input){   // <else> ::= "KW_ELSE" <instr> | ε
    if (nextSymbol.token_number == KW_ELSE) {
        printf("parsing: nt_else\n");
        Node * x1 = p_terminal(lex_input, KW_ELSE);
        Node * x2 = p_nt_instr(lex_input);

        Node * n = node_maker(x2, NULL, NT_ELSE, 0);

        free(x1);

        return n;
    } else if (follow(nextSymbol.token_number, NT_ELSE)) {
        return NULL;
    } else error(2, nextSymbol);
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

Node * p_nt_for_init(Lexer_result lex_input){   // <for_init> ::= <declr> | <exp> ";" | ";"
    if (first(nextSymbol.token_number, NT_DECLR)) {
        printf("parsing: nt_for_init: declr\n");
        Node * x1 = p_nt_declr(lex_input);

        Node * n = node_maker(x1, NULL, NT_FOR_INIT, 0);
        
        return n;
    } else if (first(nextSymbol.token_number, NT_EXP)) {
        printf("parsing: nt_for_init: exp\n");
        Node * x1 = p_nt_exp(lex_input, 0);
        Node * x2 = p_terminal(lex_input, PN_SEMI);

        x1->brother = x2;

        Node * n = node_maker(x1, NULL, NT_FOR_INIT, 0);
        
        return n;
    } else if (nextSymbol.token_number == PN_SEMI) {
        printf("parsing: nt_for_init: semicolon\n");
        Node * x1 = p_terminal(lex_input, PN_SEMI);

        Node * n = node_maker(x1, NULL, NT_FOR_INIT, 0);

        return n;
    } else error(2, nextSymbol);
}

Node * p_nt_for_exp(Lexer_result lex_input){   // <for_exp> ::= <exp> | ε
    if (first(nextSymbol.token_number, NT_EXP)) {
        printf("parsing: nt_for_exp: exp\n");
        Node * x1 = p_nt_exp(lex_input, 0);
        
        return x1;
    } else if (follow(nextSymbol.token_number, NT_EXP)) {
        printf("parsing: nt_for_exp: null\n");
        Node * n = node_maker(NULL, NULL, NT_FOR_EXP, 0);

        return n;
    } else error(2, nextSymbol);
}

Node * p_nt_exp(Lexer_result lex_input, int min_priority){        // <exp> ::= <factor> | <exp> <binary_op> <exp>
    if (nextSymbol.token_number == IDENT || nextSymbol.token_number == NUM_INT || nextSymbol.token_number == OPEN_PAREN || first(nextSymbol.token_number, NT_UNARY_OP)) {
        printf("parsing: nt_exp\n");
        Node * left = p_nt_factor(lex_input);
        
        while (((first(nextSymbol.token_number, NT_BINARY_OP)) || nextSymbol.token_number == OP_QUESTION) && (priority_table[nextSymbol.token_number] >= min_priority)) {
            if (nextSymbol.token_number >= OP_ASSIGN && nextSymbol.token_number <= OP_LSREQ) {
                int op_priority = priority_table[nextSymbol.token_number];
                Node * operator = p_nt_binary_op(lex_input);
                Node * right = p_nt_exp(lex_input, op_priority);

                operator->brother = left;
                left->brother = right;

                Node * new_left = node_maker(operator, NULL, NT_EXP, 0);
                left = new_left;
            } else if (nextSymbol.token_number == OP_QUESTION) {
                int op_priority = priority_table[nextSymbol.token_number];
                Node * op_question = p_nt_binary_op(lex_input);
                Node * middle = p_nt_exp(lex_input, 0);
                Node * op_colon = p_nt_binary_op(lex_input);
                Node * right = p_nt_exp(lex_input, op_priority);

                op_question->brother = left;
                left->brother = middle;
                middle->brother = right;

                free(op_colon);

                Node * new_left = node_maker(op_question, NULL, NT_EXP, 0);
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
        
        if (nextSymbol.token_number == OP_INCREMENT || nextSymbol.token_number == OP_DECREMENT) {
            Node * x2 = p_t_OP_INCRE_DECRE(lex_input, 1);    // 0: pre, 1: post

            x1->brother = x2;

            Node * n = node_maker(x1, NULL, NT_EXP, 0);

            return n;
        }        


        return x1;
    } else if (first(nextSymbol.token_number, NT_UNARY_OP)) {
        printf("parsing: nt_factor\n");
        Node * x1 = p_nt_unary_op(lex_input);
        Node * x2 = p_nt_factor(lex_input);

        x1->brother = x2;

        Node * n = node_maker(x1, NULL, NT_EXP, 0);

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

        case OP_INCREMENT:
        case OP_DECREMENT:
            x1 = p_t_OP_INCRE_DECRE(lex_input, 0);    // 0: pre
            return x1;

        default: error(3, nextSymbol);
    }
}

Node * p_nt_binary_op(Lexer_result lex_input){      // <binary_op> ::= "OP_ADD" | "OP_SUB" | "OP_MUL" | "OP_DIV" | "OP_MOD"
    printf("parsing: nt_binary_op\n");
    Node * x1;

    switch (nextSymbol.token_number) {
        case OP_ADD: case OP_MUL: case OP_DIV: case OP_MOD: case OP_AND: case OP_OR: case OP_XOR: case OP_SHL: case OP_LSR: case OP_LOGIC_AND: case OP_LOGIC_OR: case OP_EQ: case OP_NE: case OP_LT: case OP_GT: case OP_LE: case OP_GE: case OP_ASSIGN: case OP_ADDEQ: case OP_SUBEQ: case OP_MULEQ: case OP_DIVEQ: case OP_MODEQ: case OP_ANDEQ: case OP_OREQ: case OP_XOREQ: case OP_SHLEQ: case OP_LSREQ: case OP_QUESTION: case OP_COLON:
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
    return_exist = 0;


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
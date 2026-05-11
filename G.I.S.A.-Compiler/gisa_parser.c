/*
    <program> ::= <function>
    <function> ::= "int" IDENT "(" <param> ")" "{" <content> "}"
    <param> ::= "void"
    <content> ::= "return" <exp> ";"
    <exp> ::= NUM_INT

    <program> ::= <function>
    <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    <param> ::= "KW_VOID"
    <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    <exp> ::= NUM_INT

    <program> ::= <function>
    <function> ::= 2 0 5 <param> 6 7 <content> 8
    <param> ::= 3
    <content> ::= 4 <exp> 9
    <exp> ::= 1

    ---

    FIRST(<program>) = {2}
    FIRST(<function>) = {2}
    FIRST(<param>) = {3}
    FIRST(<content>) = {4}
    FIRST(<exp>) = {1}

    FOLLOW(<program>) = {$}
    FOLLOW(<function>) = {$}
    FOLLOW(<param>) = {6}
    FOLLOW(<content>) = {8}
    FOLLOW(<exp>) = {9}
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gisa_compiler.h"



int symbolCount = 0;
int nextSymbol = 0;

int token_type[1024];          // 읽어온 lexeme들의 타입을 저장. 
int token_value[1024];          // 읽어온 lexeme들의 값을 저장. 
int cur_line = 0;


void p_nt_program();
void p_nt_function();
void p_nt_param();
void p_nt_content();
void p_nt_exp();



void get_nextSymbol(Lexer_result lex_input){
    nextSymbol = lex_input.lexeme_list[symbolCount].token_number;
    printf("next symbol: %d\n", nextSymbol);
    symbolCount++;
    //printf("SymbolCount = %d, token_count = %d\n", symbolCount, lex_input.lexeme_count);
    if (symbolCount > (lex_input.lexeme_count + 1)) {
        printf("Parse Error: 제공된 lexeme보다 많은 symbol을 호출하고 있습니다.\n");
        exit(1);
    }
}

void error(int predLex, int givenLex){
    printf("Parse Error: lexeme %d가 주어져야 할 차례지만, 제공된 lexeme는 %d입니다.\n", predLex, givenLex);//: %d번 lexeme <%d, %d>를 파싱하는 과정에서 에러 발생");
    exit(1);
}

void p_t_IDENT(Lexer_result lex_input){
    if (nextSymbol == 0){
        printf("parsing: IDENT\n");
        get_nextSymbol(lex_input);
    }
    else error(0, nextSymbol);
}

void p_t_NUM_INT(Lexer_result lex_input){
    if (nextSymbol == 1){
        printf("parsing: NUM_INT\n");
        get_nextSymbol(lex_input);
    }
    else error(1,nextSymbol);
}

void p_t_KW_INT(Lexer_result lex_input){
    if (nextSymbol == 2){
        printf("parsing: KW_INT\n");
        get_nextSymbol(lex_input);
    }
    else error(2, nextSymbol);
}

void p_t_KW_VOID(Lexer_result lex_input){
    if (nextSymbol == 3){
        printf("parsing: KW_VOID\n");
        get_nextSymbol(lex_input);
    }
    else error(3, nextSymbol);
}

void p_t_KW_RETURN(Lexer_result lex_input){
    if (nextSymbol == 4){
        printf("parsing: KW_RETURN\n");
        get_nextSymbol(lex_input);
    }
    else error(4, nextSymbol);
}

void p_t_OPEN_PAREN(Lexer_result lex_input){
    if (nextSymbol == 5){
        printf("parsing: OPEN_PAREN\n");
        get_nextSymbol(lex_input);
    }
    else error(5, nextSymbol);
}

void p_t_CLOSE_PAREN(Lexer_result lex_input){
    if (nextSymbol == 6){
        printf("parsing: CLOSE_PAREN\n");
        get_nextSymbol(lex_input);
    }
    else error(6, nextSymbol);
}

void p_t_OPEN_BRACE(Lexer_result lex_input){
    if (nextSymbol == 7){
        printf("parsing: OPEN_BRACE\n");
        get_nextSymbol(lex_input);
    }
    else error(7, nextSymbol);
}

void p_t_CLOSE_BRACE(Lexer_result lex_input){
    if (nextSymbol == 8){
        printf("parsing: CLOSE_BRACE\n");
        get_nextSymbol(lex_input);
    }
    else error(8, nextSymbol);
}

void p_t_PN_SEMI(Lexer_result lex_input){
    if (nextSymbol == 9){
        printf("parsing: PN_SEMI\n");
        get_nextSymbol(lex_input);
    }
    else error(9, nextSymbol);
}


void p_nt_program(Lexer_result lex_input){    // <program> ::= <function>
    if (nextSymbol == 2) {
        printf("parsing: nt_program\n");
        p_nt_function(lex_input);
    }
}

void p_nt_function(Lexer_result lex_input){   // <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    if (nextSymbol == 2) {
        printf("parsing: nt_function\n");
        p_t_KW_INT(lex_input);
        p_t_IDENT(lex_input);
        p_t_OPEN_PAREN(lex_input);
        p_nt_param(lex_input);
        p_t_CLOSE_PAREN(lex_input);
        p_t_OPEN_BRACE(lex_input);
        p_nt_content(lex_input);
        p_t_CLOSE_BRACE(lex_input);
    }
}

void p_nt_param(Lexer_result lex_input){      // <param> ::= "KW_VOID"
    if (nextSymbol == 3) {
        printf("parsing: nt_param\n");
        p_t_KW_VOID(lex_input);
    }
}

void p_nt_content(Lexer_result lex_input){    // <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    if (nextSymbol == 4) {
        printf("parsing: nt_content\n");
        p_t_KW_RETURN(lex_input);
        p_nt_exp(lex_input);
        p_t_PN_SEMI(lex_input);
    }
}

void p_nt_exp(Lexer_result lex_input){        // <exp> ::= NUM_INT
    if (nextSymbol == 1) {
        printf("parsing: nt_exp\n");
        p_t_NUM_INT(lex_input);
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

Node parser(Lexer_result lex_input, char *parse_name)
{
    Node ast_top;
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

    /*
    printf("Lexing loaded result test:\n");

        for (int i = 0; i < lex_input.lexeme_count; i++) {
            printf("lexeme count %d: <%d, %d>\n", lex_input.lexeme_count, lex_input.lexeme_list[i].token_number, lex_input.lexeme_list[i].token_value);
        }

        for (int i = 0; i < lex_input.value_count; i++) {
            printf("value table count %d: <%s, %d>\n", lex_input.value_count, lex_input.value_table[i].lexeme_name, lex_input.value_table[i].name_number);
        }
    */

    printf("\nLexeme 로딩 성공. Parsing 시작.\n\n");

    get_nextSymbol(lex_input);
    p_nt_program(lex_input);


    printf("Parsing Finished.\n");
    fclose(parsefp);

    return ast_top;
}
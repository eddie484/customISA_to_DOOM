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



void get_nextSymbol(){
    nextSymbol = token_type[symbolCount];
    printf("next symbol: %d\n", nextSymbol);
    symbolCount++;
    //printf("SymbolCount = %d, cur_line = %d\n", symbolCount, cur_line);
    if (symbolCount > (cur_line + 1)) {
        printf("Parse Error: 제공된 lexeme보다 많은 symbol을 호출하고 있습니다.\n");
        exit(1);
    }
}

void error(int predLex, int givenLex){
    printf("Parse Error: lexeme %d가 주어져야 할 차례지만, 제공된 lexeme는 %d입니다.\n", predLex, givenLex);//: %d번 lexeme <%d, %d>를 파싱하는 과정에서 에러 발생");
    exit(1);
}

void p_t_IDENT(){
    if (nextSymbol == 0){
        printf("parsing: IDENT\n");
        get_nextSymbol();
    }
    else error(0, nextSymbol);
}

void p_t_NUM_INT(){
    if (nextSymbol == 1){
        printf("parsing: NUM_INT\n");
        get_nextSymbol();
    }
    else error(1,nextSymbol);
}

void p_t_KW_INT(){
    if (nextSymbol == 2){
        printf("parsing: KW_INT\n");
        get_nextSymbol();
    }
    else error(2, nextSymbol);
}

void p_t_KW_VOID(){
    if (nextSymbol == 3){
        printf("parsing: KW_VOID\n");
        get_nextSymbol();
    }
    else error(3, nextSymbol);
}

void p_t_KW_RETURN(){
    if (nextSymbol == 4){
        printf("parsing: KW_RETURN\n");
        get_nextSymbol();
    }
    else error(4, nextSymbol);
}

void p_t_OPEN_PAREN(){
    if (nextSymbol == 5){
        printf("parsing: OPEN_PAREN\n");
        get_nextSymbol();
    }
    else error(5, nextSymbol);
}

void p_t_CLOSE_PAREN(){
    if (nextSymbol == 6){
        printf("parsing: CLOSE_PAREN\n");
        get_nextSymbol();
    }
    else error(6, nextSymbol);
}

void p_t_OPEN_BRACE(){
    if (nextSymbol == 7){
        printf("parsing: OPEN_BRACE\n");
        get_nextSymbol();
    }
    else error(7, nextSymbol);
}

void p_t_CLOSE_BRACE(){
    if (nextSymbol == 8){
        printf("parsing: CLOSE_BRACE\n");
        get_nextSymbol();
    }
    else error(8, nextSymbol);
}

void p_t_PN_SEMI(){
    if (nextSymbol == 9){
        printf("parsing: PN_SEMI\n");
        get_nextSymbol();
    }
    else error(9, nextSymbol);
}


void p_nt_program(){    // <program> ::= <function>
    if (nextSymbol == 2) {
        printf("parsing: nt_program\n");
        p_nt_function();
    }
}

void p_nt_function(){   // <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    if (nextSymbol == 2) {
        printf("parsing: nt_function\n");
        p_t_KW_INT();
        p_t_IDENT();
        p_t_OPEN_PAREN();
        p_nt_param();
        p_t_CLOSE_PAREN();
        p_t_OPEN_BRACE();
        p_nt_content();
        p_t_CLOSE_BRACE();
    }
}

void p_nt_param(){      // <param> ::= "KW_VOID"
    if (nextSymbol == 3) {
        printf("parsing: nt_param\n");
        p_t_KW_VOID();
    }
}

void p_nt_content(){    // <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    if (nextSymbol == 4) {
        printf("parsing: nt_content\n");
        p_t_KW_RETURN();
        p_nt_exp();
        p_t_PN_SEMI();
    }
}

void p_nt_exp(){        // <exp> ::= NUM_INT
    if (nextSymbol == 1) {
        printf("parsing: nt_exp\n");
        p_t_NUM_INT();
    }
}




/* *******************************
**********************************
**********************************
***********            ***********
***********    MAIN    ***********
***********            ***********
**********************************
**********************************
******************************* */

int main(int argc, char *argv[])
{
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

    FILE *lexfp = fopen(lex_name, "r");       // 처리할 lexeme가 적힌 파일 오픈
    FILE *parsefp = fopen(parse_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (lexfp == NULL || parsefp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("파일 읽기 실패\n");
        return 1;
    }

    char buf[1024];          // 읽어온 lexeme 라인을 저장. 




    while (fgets(buf, sizeof(buf), lexfp)) {        // 입력으로 받은 lexeme 파일을 한 줄씩 읽어온다. 끝까지 읽어올 때까지 루프한다.
        if (strcmp(buf, "@@@") == 0) break;
        
        //lexeme 구조체에 각각 기록할 것. 파일 생성할 때 세컨드도 필요하니까.
        sscanf(buf, "<%d, %d>", &token_type[cur_line], &token_value[cur_line]);
        printf("<%d, %d>, line %d\n", token_type[cur_line], token_value[cur_line], cur_line);
        cur_line++;

    }

    printf("\nLexeme 로딩 성공. Parsing 시작.\n\n");

    get_nextSymbol();
    p_nt_program();


    printf("Parsing Finished.\n");
    fclose(lexfp);
    fclose(parsefp);
}
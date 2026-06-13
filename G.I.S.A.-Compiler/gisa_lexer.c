/*
    IDENT       0   // 식별자. [a-zA-Z_][a-zA-Z0-9_]*\b
    NUM_INT     1   // 상수 정수. [0-9]+\b

    KW_INT      2   // 키워드 int
    KW_VOID     3   // 키워드 void
    KW_RETURN   4   // 키워드 return

    OPEN_PAREN  5   // (
    CLOSE_PAREN 6   // )
    OPEN_BRACE  7   // {
    CLOSE_BRACE 8   // }

    PN_SEMI     9   // ;

    OP_TILDE        10  // ~
    OP_MINUS        11  // -
    OP_DECREMENT    12  // --

    OP_ADD          14  // +
    OP_MUL          16  // *
    OP_DIV          17  // /
    OP_MOD          18  // %

    OP_AND          14  // &
    OP_OR           15  // |
    OP_XOR          16  // ^
    OP_SHL          17  // <<
    OP_LSR          18  // >>

    OP_LOGIC_NOT    24  // !
    OP_LOGIC_AND    25  // &&
    OP_LOGIC_OR     26  // ||
    OP_EQ           27  // ==
    OP_NE           28  // !=
    OP_LT           29  // <
    OP_GT           30  // >
    OP_LE           31  // <=
    OP_GE           32  // >=

    OP_ASSIGN       33  // =

*/



#include <ctype.h>

#include "gisa_compiler.h"


int lexeme_limit = 100;
int lex_val_table_limit = 10;

Lexeme * lexeme;
int lexeme_count = 0;
Lexeme_value * lex_val_table;
int lex_val_table_count = 0;

typedef struct {
    int macro_number;
    char char_icon;
} Char_macro_mix;


static const int lexing_state_table[128] = {
    ['('] = 5,
    [')'] = 6,
    ['{'] = 7,
    ['}'] = 8,
    [';'] = 9,
    ['~'] = 10,
    ['-'] = 11,
    ['+'] = 12,
    ['*'] = 13,
    ['/'] = 14,
    ['%'] = 15,
    ['&'] = 16,
    ['|'] = 17,
    ['^'] = 18,
    ['<'] = 19,
    ['>'] = 20,
    ['!'] = 21,
    ['='] = 22,
    ['?'] = 23,
    [':'] = 24
};

static const Char_macro_mix lexing_result_table[128] = {
    [5].macro_number = OPEN_PAREN,
    [5].char_icon = '(',
    
    [6].macro_number = CLOSE_PAREN,
    [6].char_icon = ')',
    
    [7].macro_number = OPEN_BRACE,
    [7].char_icon = '{',
    
    [8].macro_number = CLOSE_BRACE,
    [8].char_icon = '}',
    
    [9].macro_number = PN_SEMI,
    [9].char_icon = ';',
    
    [10].macro_number = OP_TILDE,
    [10].char_icon = '~',
    
    [23].macro_number = OP_QUESTION,
    [23].char_icon = '?',
    
    [24].macro_number = OP_COLON,
    [24].char_icon = ':'
};


/* ****************************************
******** LEXER VALUE MANAGER START ********
**************************************** */

int lexval_manager (char *name) {
    int count = 0;

    while(count < lex_val_table_count){   // 이미 저장된 value인지 확인
        if (strcmp(name, lex_val_table[count].lexeme_name) == 0){
            return lex_val_table[count].name_number;  // 발견하면 해당 value의 number를 return 한다.
        } else {
            count++;
        }
    }
    
    // 확인 결과 저장되지 않은 value일 경우, 저장한다.
    strcpy(lex_val_table[lex_val_table_count].lexeme_name, name);
    lex_val_table[lex_val_table_count].name_number = lex_val_table_count;

    printf("새로운 Lexeme Value 저장. Lexeme Value: %s, Value Number: %d.\n", lex_val_table[lex_val_table_count].lexeme_name, lex_val_table[lex_val_table_count].name_number);
    lex_val_table_count++;

    if (lex_val_table_count >= lex_val_table_limit) {
        lex_val_table_limit = lex_val_table_limit * 2;
        lex_val_table = realloc(lex_val_table, sizeof(Lexeme_value) * lex_val_table_limit);
        //printf("REALLOC! addr is: %p\n", (void *)lex_val_table);
    }

    return lex_val_table[lex_val_table_count - 1].name_number;  // 저장 후 저장된 number를 return 한다.
    
}

/* **************************************
******** LEXER VALUE MANAGER END ********
************************************** */


/*
int check_word_boundary(int i, char left, char right) {     // ret 1이 \b, ret0은 \b 아님
    if (i == 0) {
        if (('0' <= right <= '9') || ('A' <= right <= 'Z') || ('a' <= right <= 'z')) return 0;
        else return 1;
    } else if (i == 1023) {
        if (('0' <= left <= '9') || ('A' <= left <= 'Z') || ('a' <= left <= 'z')) return 0;
        else return 1;
    } else {
        if ((left ==  && right != ) || (('0' <= left <= '9') || ('A' <= left <= 'Z') || ('a' <= left <= 'z')  && ('0' <= right <= '9') || ('A' <= right <= 'Z') || ('a' <= right <= 'z'))) return 0;
    }
}
*/
int check_right_word_boundary(char right) {     // ret 1이 \b, ret0은 \b 아님
    if (('0' <= right && right <= '9') || ('A' <= right && right <= 'Z') || ('a' <= right && right <= 'z') || right == '_') return 0;
    else return 1;
}




/* *************************************
****************************************
****************************************
***********                  ***********
***********    MAIN LEXER    ***********
***********                  ***********
****************************************
****************************************
************************************* */

Lexer_result lexer(char *prep_name, char *lex_name)
{
    Lexer_result lexer_result;

    lexeme = malloc(sizeof(Lexeme) * lexeme_limit);
    lex_val_table = malloc(sizeof(Lexeme_value) * lex_val_table_limit);


    /*
    char *prep_name;
    char *lex_name;

        // 인자 개수 확인해 파일이름 지정.
        if (argc == 3) {
            prep_name = argv[1];
            lex_name = argv[2];
        } else {
            prep_name = "preprocess.prep";
            lex_name = "lexeme.lex";
        }
    */


    FILE *prepfp = fopen(prep_name, "r");       // 처리할 전처리 코드가 적힌 파일 오픈
    FILE *lexfp = fopen(lex_name, "w");         // 처리 결과 lexeme를 저장할 파일 오픈

    if (prepfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("처리할 전처리 코드가 적힌 파일 읽기 실패. 시도한 파일명: %s\n", prep_name);
        exit(1);
    } else if (lexfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Lex 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", lex_name);
        exit(1);
    }


    char buf[1024];          // 읽어온 전처리 코드 라인을 저장. 
    int cur_state = 0;
    int cur_index = 0;



    while (fgets(buf, sizeof(buf), prepfp)) {        // 입력으로 받은 전처리 파일을 한 줄씩 읽어온다. 끝까지 읽어올 때까지 루프한다.
        cur_index = 0;

        char *p = buf;
        while (isspace((unsigned char)*p)) p++;

        if (p != buf) memmove(buf, p, strlen(p) + 1);

        while (cur_index < 1024) {
            
            char c = buf[cur_index];
            //printf("이번 문자: %c, 다음 문자: %c, 다다음 문자: %c, state: %d\n", buf[cur_index], buf[cur_index + 1], buf[cur_index + 2], cur_state);

            // state = 0. Idle.
            if (cur_state == 0) {  
                if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_') {
                    if (check_right_word_boundary(buf[cur_index + 1])) cur_state = 2;   // 한 글자 식별자의 경우, 바로 state 2로 이동.
                    else {                                                              // 두 글자 이상 식별자의 경우, 인덱스 증가와 state 1로 이동.
                        cur_state = 1;
                    }
                }
                else if ('0' <= c && c <= '9') {
                    cur_state = 3;
                    if (check_right_word_boundary(buf[cur_index + 1]))  cur_state = 4;   // 한 글자 상수의 경우, 바로 state 4로 이동.
                    else {                                                              // 두 글자 이상 상수의 경우, 인덱스 증가와 state 3로 이동.
                        cur_state = 3;
                    }
                }
                else if (((int)c >= 0 && (int)c <= 127) && lexing_state_table[(int)c]) {
                    cur_state = lexing_state_table[(int)c];
                }
                else if (c == ' ') {
                    memmove(buf, buf + 1, 1024 - 1);
                    cur_index--;
                } //
                else if (c != '\r' && c != '\n' && c != '\0') {
                    printf("Lexical error. Program exit. current state: %d, error: -%c-\n", cur_state, c);
                    exit(1);
                }

                cur_index++;
                
            } 
            
            
            // state = 1. 식별자 탐지 중.
            else if (cur_state == 1) {
                if (check_right_word_boundary(buf[cur_index + 1])) cur_state = 2;
                else if (('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_') {
                    cur_state = 1;
                }
                else {
                    printf("Lexical error. Program exit. current state: %d, current index: %d, error: -%c-\n", cur_state, cur_index, c);
                    exit(1);
                }
                    cur_index++;


            // state = 3. 상수 탐지 중.
            } else if (cur_state == 3) {
                if (check_right_word_boundary(buf[cur_index + 1])) cur_state = 4;
                else if ('0' <= c && c <= '9') {
                    cur_state = 3;
                }
                else {
                    printf("Lexical error. Program exit. current state: %d, error: -%c-\n", cur_state, c);
                    exit(1);
                }
                    cur_index++;


            // state = 2. 식별자 탐지 완료, 키워드 구분 및 lexeme 리스트 확인.
            } else if (cur_state == 2) {
                char get_str[cur_index + 1];
                snprintf(get_str, cur_index + 1, "%s", buf);
                if (!strcmp(get_str, "int")) {
                    lexeme[lexeme_count].token_number = KW_INT;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("INT");
                } else if (!strcmp(get_str, "void")) {
                    lexeme[lexeme_count].token_number = KW_VOID;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("VOID");
                } else if (!strcmp(get_str, "return")) {
                    lexeme[lexeme_count].token_number = KW_RETURN;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("RETURN");
                } else if (!strcmp(get_str, "if")) {
                    lexeme[lexeme_count].token_number = KW_IF;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("IF");
                } else if (!strcmp(get_str, "else")) {
                    lexeme[lexeme_count].token_number = KW_ELSE;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("ELSE");
                } else if (!strcmp(get_str, "goto")) {
                    lexeme[lexeme_count].token_number = KW_GOTO;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("GOTO");
                } else if (!strcmp(get_str, "do")) {
                    lexeme[lexeme_count].token_number = KW_DO;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("DO");
                } else if (!strcmp(get_str, "while")) {
                    lexeme[lexeme_count].token_number = KW_WHILE;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("WHILE");
                } else if (!strcmp(get_str, "for")) {
                    lexeme[lexeme_count].token_number = KW_FOR;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("FOR");
                } else if (!strcmp(get_str, "break")) {
                    lexeme[lexeme_count].token_number = KW_BREAK;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("BREAK");
                } else if (!strcmp(get_str, "continue")) {
                    lexeme[lexeme_count].token_number = KW_CONTINUE;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("CONTINUE");
                } else if (!strcmp(get_str, "switch")) {
                    lexeme[lexeme_count].token_number = KW_SWITCH;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("SWITCH");
                } else if (!strcmp(get_str, "case")) {
                    lexeme[lexeme_count].token_number = KW_CASE;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("CASE");
                } else if (!strcmp(get_str, "default")) {
                    lexeme[lexeme_count].token_number = KW_DEFAULT;
                    lexeme[lexeme_count].token_value = 0;
                    //printf("DEFAULT");
                } else {
                    lexeme[lexeme_count].token_number = IDENT;
                    lexeme[lexeme_count].token_value = lexval_manager (get_str);
                }
                //printf("%s", get_str);

                printf("<%d, %d>, %s\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value, get_str);
                fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                lexeme_count++;
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;


            // state = 4. 상수 탐지 완료, lexeme 리스트 확인.
            } else if (cur_state == 4) {
                char get_num[cur_index + 1];
                snprintf(get_num, cur_index + 1, "%s", buf);

                lexeme[lexeme_count].token_number = NUM_INT;
                lexeme[lexeme_count].token_value = lexval_manager(get_num);

                printf("<%d, %d>, %s\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value, get_num);
                fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                lexeme_count++;
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;

                
            
            
            // state = 11. -로 시작하는 경우.
            } else if (cur_state == 11) {
                if (buf[cur_index] == '-') {    // --인 경우
                    lexeme[lexeme_count].token_number = OP_DECREMENT;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, --\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                    
                } else if (buf[cur_index] == '=') {    // -=인 경우
                    lexeme[lexeme_count].token_number = OP_SUBEQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, --\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                    
                } else {                            // -인 경우
                    lexeme[lexeme_count].token_number = OP_MINUS;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, -\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 12. +로 시작하는 경우.
            } else if (cur_state == 12) {
                if (buf[cur_index] == '+') {    // ++인 경우
                    lexeme[lexeme_count].token_number = OP_INCREMENT;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, ++\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else if (buf[cur_index] == '=') {    // +=인 경우
                    lexeme[lexeme_count].token_number = OP_ADDEQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, +=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // +인 경우
                    lexeme[lexeme_count].token_number = OP_ADD;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, +\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 13. *로 시작하는 경우.
            } else if (cur_state == 13) {
                if (buf[cur_index] == '=') {    // *=인 경우
                    lexeme[lexeme_count].token_number = OP_MULEQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, *=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // *인 경우
                    lexeme[lexeme_count].token_number = OP_MUL;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, *\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 14. /로 시작하는 경우.
            } else if (cur_state == 14) {
                if (buf[cur_index] == '=') {    // /=인 경우
                    lexeme[lexeme_count].token_number = OP_DIVEQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, /=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // /인 경우
                    lexeme[lexeme_count].token_number = OP_DIV;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, /\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 15. %로 시작하는 경우.
            } else if (cur_state == 15) {
                if (buf[cur_index] == '=') {    // %=인 경우
                    lexeme[lexeme_count].token_number = OP_MODEQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, %%=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // %인 경우
                    lexeme[lexeme_count].token_number = OP_MOD;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, %%\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 16. &로 시작하는 경우.
            } else if (cur_state == 16) {
                if (buf[cur_index] == '&') {    // &&인 경우
                    lexeme[lexeme_count].token_number = OP_LOGIC_AND;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, &&\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else if (buf[cur_index] == '=') {    // &=인 경우
                    lexeme[lexeme_count].token_number = OP_ANDEQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, &=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // &인 경우
                    lexeme[lexeme_count].token_number = OP_AND;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, &\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 17. |로 시작하는 경우.
            } else if (cur_state == 17) {
                if (buf[cur_index] == '|') {    // ||인 경우
                    lexeme[lexeme_count].token_number = OP_LOGIC_OR;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, ||\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else if (buf[cur_index] == '=') {    // |=인 경우
                    lexeme[lexeme_count].token_number = OP_OREQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, |=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // |인 경우
                    lexeme[lexeme_count].token_number = OP_OR;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, |\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 18. ^로 시작하는 경우.
            } else if (cur_state == 18) {
                if (buf[cur_index] == '=') {    // ^=인 경우
                    lexeme[lexeme_count].token_number = OP_XOREQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, ^=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // ^인 경우
                    lexeme[lexeme_count].token_number = OP_XOR;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, ^\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 19. <로 시작하는 경우.
            } else if (cur_state == 19) {
                if (buf[cur_index] == '<') {    // <<=인 경우
                    if (buf[cur_index + 1] == '=') {
                        lexeme[lexeme_count].token_number = OP_SHLEQ;
                        lexeme[lexeme_count].token_value = 0;

                        printf("<%d, %d>, <<=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                        fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                        lexeme_count++;
                        cur_index += 2;    // 세 글자 토큰이므로 cur_index = 3이다.
                        cur_state = 0;
                        memmove(buf, buf + cur_index, 1024 - cur_index);
                        cur_index = 0;

                    } else {                    // <<인 경우
                        lexeme[lexeme_count].token_number = OP_SHL;
                        lexeme[lexeme_count].token_value = 0;

                        printf("<%d, %d>, <<\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                        fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                        lexeme_count++;
                        cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                        cur_state = 0;
                        memmove(buf, buf + cur_index, 1024 - cur_index);
                        cur_index = 0;
                    }

                } else if (buf[cur_index] == '=') {    // <=인 경우
                    lexeme[lexeme_count].token_number = OP_LE;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, <=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // <인 경우
                    lexeme[lexeme_count].token_number = OP_LT;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, <\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 20. >로 시작하는 경우.
            } else if (cur_state == 20) {
                if (buf[cur_index] == '>') {    // >>=인 경우
                    if (buf[cur_index + 1] == '=') {
                        lexeme[lexeme_count].token_number = OP_LSREQ;
                        lexeme[lexeme_count].token_value = 0;

                        printf("<%d, %d>, >>=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                        fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                        lexeme_count++;
                        cur_index += 2;    // 세 글자 토큰이므로 cur_index = 3이다.
                        cur_state = 0;
                        memmove(buf, buf + cur_index, 1024 - cur_index);
                        cur_index = 0;

                    } else {
                        lexeme[lexeme_count].token_number = OP_LSR;
                        lexeme[lexeme_count].token_value = 0;

                        printf("<%d, %d>, >>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                        fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                        lexeme_count++;
                        cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                        cur_state = 0;
                        memmove(buf, buf + cur_index, 1024 - cur_index);
                        cur_index = 0;
                    }

                } else if (buf[cur_index] == '=') {    // >=인 경우
                    lexeme[lexeme_count].token_number = OP_GE;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, >=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // >인 경우
                    lexeme[lexeme_count].token_number = OP_GT;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, >\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 21. !로 시작하는 경우.
            } else if (cur_state == 21) {
                if (buf[cur_index] == '=') {    // !=인 경우
                    lexeme[lexeme_count].token_number = OP_NE;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, !=\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // !인 경우
                    lexeme[lexeme_count].token_number = OP_LOGIC_NOT;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, !\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }


            // state = 22. =로 시작하는 경우.
            } else if (cur_state == 22) {
                if (buf[cur_index] == '=') {    // ==인 경우
                    lexeme[lexeme_count].token_number = OP_EQ;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, ==\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_index++;    // 두 글자 토큰이므로 cur_index = 2이다.
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;

                } else {                            // =인 경우
                    lexeme[lexeme_count].token_number = OP_ASSIGN;
                    lexeme[lexeme_count].token_value = 0;

                    printf("<%d, %d>, =\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                    lexeme_count++;
                    cur_state = 0;
                    memmove(buf, buf + cur_index, 1024 - cur_index);
                    cur_index = 0;
                }




            // 한 글자 짜리 토큰들 처리
            } else if ((cur_state >= 5 && cur_state <= 10) || (cur_state >= 12 && cur_state <= 15) || (cur_state == 18) || (cur_state == 23) || (cur_state == 24)) {
                lexeme[lexeme_count].token_number = lexing_result_table[cur_state].macro_number;
                lexeme[lexeme_count].token_value = 0;

                printf("<%d, %d>, %c\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value, lexing_result_table[cur_state].char_icon);
                fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
                lexeme_count++;
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;
            } 

            if (lexeme_count >= lexeme_limit) {
                lexeme_limit = lexeme_limit * 2;
                lexeme = realloc(lexeme, sizeof(Lexeme) * lexeme_limit);
            }

            
            if (c == '\r' || c == '\n') {
                //printf("엔터 만남! %d\n", cur_state);
                break;
            } else if (c == ' ') {
                //printf("스페이스바 만남!\n");
                continue;
            } else if (c == '\0') break;
        }



        printf("라인 패스\n");
      
    }
    printf("Lexing Finished.\n");

    

    if (lexeme_count >= lexeme_limit) {
        lexeme_limit = lexeme_limit + 1;
        lexeme = realloc(lexeme, sizeof(Lexeme) * lexeme_limit);
    }

    lexeme[lexeme_count].token_number = LEXING_EOF;
    lexeme[lexeme_count].token_value = 0;

    printf("<%d, %d>, END OF FILE.\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
    fprintf(lexfp, "<%d, %d>\n", lexeme[lexeme_count].token_number, lexeme[lexeme_count].token_value);
    
    lexeme_count++;
    //fprintf(lexfp, "@@@\n");    // @@@ 라인을 추가해 렉싱 완료 표시.

    fclose(prepfp);
    fclose(lexfp);


    lexer_result.lexeme_list = lexeme;
    lexer_result.lexeme_count = lexeme_count;
    lexer_result.value_table = lex_val_table;
    lexer_result.value_count = lex_val_table_count;

    return lexer_result;
}
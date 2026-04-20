#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define IDENT       0   // 식별자. [a-zA-Z_][a-zA-Z0-9_]*\b
#define NUM_INT     1   // 상수 정수. [0-9]+\b

#define KW_INT      2   // 키워드 int
#define KW_VOID     3   // 키워드 void
#define KW_RETURN   4   // 키워드 return

#define OPEN_PAREN  5   // (
#define CLOSE_PAREN 6   // )
#define OPEN_BRACE  7   // {
#define CLOSE_BRACE 8   // }

#define PN_SEMI     9   // ;


typedef struct {
    int token_number;
    int token_value;
} Lexeme;

typedef struct {
    char lexeme_name[50];
    int name_number;
} Lexeme_value;


Lexeme_value lex_val [100];
int lex_val_count = 0;


int lexval_manager (char *name) {
    int count = 0;
    while(count < lex_val_count){
        if (strcmp(name, lex_val[count].lexeme_name) == 0){
            return lex_val[count].name_number;
        } else {
            count++;
        }
    }
    
    strcpy(lex_val[lex_val_count].lexeme_name, name);
    lex_val[lex_val_count].name_number = lex_val_count;

    printf("새로운 Lexeme Value 저장. Lexeme Value: %s, Value Number: %d.\n", lex_val[lex_val_count].lexeme_name, lex_val[lex_val_count].name_number);
    lex_val_count++;
    return lex_val[lex_val_count - 1].name_number;
    
}
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

    FILE *prepfp = fopen(prep_name, "r");       // 처리할 전처리 코드가 적힌 파일 오픈
    FILE *lexfp = fopen(lex_name, "w");         // 처리 결과 lexeme를 저장할 파일 오픈

    if (prepfp == NULL || lexfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("파일 읽기 실패\n");
        return 1;
    }


    char buf[1024];          // 읽어온 전처리 코드 라인을 저장. 
    int cur_state = 0;
    int cur_index = 0;



    while (fgets(buf, sizeof(buf), prepfp)) {        // 입력으로 받은 어셈블리 파일을 한 줄씩 읽어온다. 끝까지 읽어올 때까지 루프한다.
        cur_index = 0;

        char *p = buf;
        while (isspace((unsigned char)*p)) p++;

        if (p != buf) memmove(buf, p, strlen(p) + 1);

        while (cur_index < 1024) {
            Lexeme lexeme;
            char c = buf[cur_index];
            //printf("이번 문자: %c, state: %d\n", c, cur_state);

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
                else if (c == '(') {
                    cur_state = 5;
                }
                else if (c == ')') {
                    cur_state = 6;
                }
                else if (c == '{') {
                    cur_state = 7;
                }
                else if (c == '}') {
                    cur_state = 8;
                }
                else if (c == ';') {
                    cur_state = 9;
                }
                else if (c == ' ') {
                    memmove(buf, buf + 1, 1024 - 1);
                    cur_index--;
                } //
                else if (c != '\r' && c != '\0') {
                    printf("Lexical error. Program exit. current state: %d, error: -%c-\n", cur_state, c);
                    return 1;
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
                    return 1;
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
                    return 1;
                }
                    cur_index++;
            }

            
            // state = 2. 식별자 탐지 완료, 키워드 구분 및 lexeme 리스트 확인.
            if (cur_state == 2) {
                char get_str[cur_index + 1];
                snprintf(get_str, cur_index + 1, "%s", buf);
                if (!strcmp(get_str, "int")) {
                    lexeme.token_number = KW_INT;
                    lexeme.token_value = 0;
                    //printf("INT");
                } else if (!strcmp(get_str, "void")) {
                    lexeme.token_number = KW_VOID;
                    lexeme.token_value = 0;
                    //printf("VOID");
                } else if (!strcmp(get_str, "return")) {
                    lexeme.token_number = KW_RETURN;
                    lexeme.token_value = 0;
                    //printf("RETURN");
                } else {
                    lexeme.token_number = IDENT;
                    lexeme.token_value = lexval_manager (get_str);
                }
                //printf("%s", get_str);

                printf("<%d, %d>, %s\n", lexeme.token_number, lexeme.token_value, get_str);
                fprintf(lexfp, "<%d, %d>\n", lexeme.token_number, lexeme.token_value);
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;
            }
            
            
            // state = 4. 상수 탐지 완료, lexeme 리스트 확인.
            if (cur_state == 4) {
                char get_num[cur_index + 1];
                snprintf(get_num, cur_index + 1, "%s", buf);

                lexeme.token_number = NUM_INT;
                lexeme.token_value = lexval_manager(get_num);

                printf("<%d, %d>, %s\n", lexeme.token_number, lexeme.token_value, get_num);
                fprintf(lexfp, "<%d, %d>\n", lexeme.token_number, lexeme.token_value);
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;
            }
            
            
            // state = 5. ( 인 경우.
            if (cur_state == 5) {
                lexeme.token_number = OPEN_PAREN;
                lexeme.token_value = 0;

                printf("<%d, %d>, (\n", lexeme.token_number, lexeme.token_value);
                fprintf(lexfp, "<%d, %d>\n", lexeme.token_number, lexeme.token_value);
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;
            } 
            
            
            // state = 6. ) 인 경우.
            if (cur_state == 6) {
                lexeme.token_number = CLOSE_PAREN;
                lexeme.token_value = 0;

                printf("<%d, %d>, )\n", lexeme.token_number, lexeme.token_value);
                fprintf(lexfp, "<%d, %d>\n", lexeme.token_number, lexeme.token_value);
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;
            } 
            
            
            // state = 7. { 인 경우.
            if (cur_state == 7) {
                lexeme.token_number = OPEN_BRACE;
                lexeme.token_value = 0;

                printf("<%d, %d>, {\n", lexeme.token_number, lexeme.token_value);
                fprintf(lexfp, "<%d, %d>\n", lexeme.token_number, lexeme.token_value);
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;
            } 
            
            
            // state = 8. } 인 경우.
            if (cur_state == 8) {
                lexeme.token_number = CLOSE_BRACE;
                lexeme.token_value = 0;

                printf("<%d, %d>, }\n", lexeme.token_number, lexeme.token_value);
                fprintf(lexfp, "<%d, %d>\n", lexeme.token_number, lexeme.token_value);
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;
            } 
            
            
            // state = 9. ; 인 경우.
            if (cur_state == 9) {
                lexeme.token_number = PN_SEMI;
                lexeme.token_value = 0;

                printf("<%d, %d>, ;\n", lexeme.token_number, lexeme.token_value);
                fprintf(lexfp, "<%d, %d>\n", lexeme.token_number, lexeme.token_value);
                cur_state = 0;
                memmove(buf, buf + cur_index, 1024 - cur_index);
                cur_index = 0;
            }

            
            if (c == '\r') {
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
    fclose(lexfp);
}
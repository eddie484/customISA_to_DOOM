#ifndef COMMON_H
#define COMMON_H


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


#define NT_PROGRAM  100
#define NT_FUNCTION 101
#define NT_PARAM    102
#define NT_CONTENT  103
#define NT_EXP      104


#define ASM_PROGRAM  200
#define ASM_FUNCTION 201
#define ASM_INSTR    202
#define ASM_MOV      203
#define ASM_RET      204
#define ASM_REGISTER 205


#define LEXING_EOF   999


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    int token_number;
    int token_value;
} Lexeme;

typedef struct {
    char lexeme_name[50];
    int name_number;
} Lexeme_value;

typedef struct {
    Lexeme *lexeme_list;
    int lexeme_count;
    Lexeme_value *value_table;
    int value_count;
} Lexer_result;

typedef struct Node {
    Lexeme token;
    struct Node * son;
    struct Node * brother;
} Node;


int lexval_manager (char *name);
int check_right_word_boundary(char right);
Lexer_result lexer(char *prep_name, char *lex_name);
Node * parser(Lexer_result lex_input, char *parse_name);
Node * code_generator(Node * parse_input, char * codegentree_name);
void code_emitter(Node * codegen_result, Lexer_result lexer_result, char * codegentree_name);

void lexer_result_printer(Lexer_result lexer_result);
void bin_tree_printer(Node * tree_top);
void bin_tree_file_printer(Node * tree_top, FILE *parsefp);

#endif
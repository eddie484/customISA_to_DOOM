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


extern Lexeme lexeme [100];
extern Lexeme_value lex_val [100];


int lexval_manager (char *name);
int check_right_word_boundary(char right);
Lexer_result lexer(char *prep_name, char *lex_name);

#endif
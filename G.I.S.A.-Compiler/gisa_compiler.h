#ifndef COMMON_H
#define COMMON_H


#define IDENT           0   // 식별자. [a-zA-Z_][a-zA-Z0-9_]*\b
#define NUM_INT         1   // 상수 정수. [0-9]+\b

#define KW_INT          2   // 키워드 int
#define KW_VOID         3   // 키워드 void
#define KW_RETURN       4   // 키워드 return

#define OPEN_PAREN      5   // (
#define CLOSE_PAREN     6   // )
#define OPEN_BRACE      7   // {
#define CLOSE_BRACE     8   // }

#define PN_SEMI         9   // ;

#define OP_TILDE        10  // ~
#define OP_MINUS        11  // -
#define OP_NEG          12  // -
#define OP_DECREMENT    13  // --

#define OP_ADD          14  // +
#define OP_SUB          15  // -
#define OP_MUL          16  // *
#define OP_DIV          17  // /
#define OP_MOD          18  // %

#define OP_AND          19  // &
#define OP_OR           20  // |
#define OP_XOR          21  // ^
#define OP_SHL          22  // <<
#define OP_LSR          23  // >>, 추후 signed/unsigned 타입 추가시 LSR, ASR 구분할 것.

#define OP_LOGIC_NOT    24  // !
#define OP_LOGIC_AND    25  // &&
#define OP_LOGIC_OR     26  // ||
#define OP_EQ           27  // ==
#define OP_NE           28  // !=
#define OP_LT           29  // <
#define OP_GT           30  // >
#define OP_LE           31  // <=
#define OP_GE           32  // >=

#define OP_ASSIGN       33  // =

#define OP_ADDEQ        34
#define OP_SUBEQ        35
#define OP_MULEQ        36
#define OP_DIVEQ        37
#define OP_MODEQ        38
#define OP_ANDEQ        39
#define OP_OREQ         40
#define OP_XOREQ        41
#define OP_SHLEQ        42
#define OP_LSREQ        43

#define OP_INCREMENT    44
#define OP_PRE_INCRE    45
#define OP_POST_INCRE   46
#define OP_PRE_DECRE    47
#define OP_POST_DECRE   48

#define KW_IF           49 
#define KW_ELSE         50
#define OP_QUESTION     51
#define OP_COLON        52

#define KW_GOTO         53

#define KW_DO           54
#define KW_WHILE        55
#define KW_FOR          56
#define KW_BREAK        57
#define KW_CONTINUE     58

#define KW_SWITCH       59
#define KW_CASE         60
#define KW_DEFAULT      61

#define PN_COMMA        62


#define NT_PROGRAM      100
#define NT_FUNC_DECLR   101
#define NT_PARAM_LIST   102
#define NT_PARAM_MULTI  103
#define NT_PARAM        104
#define NT_FUNC_CONTENT 105
#define NT_BLOCK        106
#define NT_INSTR_LIST   107
#define NT_INSTR        108
#define NT_CONTENT      109
#define NT_ELSE         110
#define NT_LABEL        111
#define NT_DECLR        112
#define NT_VAR_DECLR    113
#define NT_ASSIGN       114
#define NT_FOR_INIT     115
#define NT_FOR_EXP      116
#define NT_EXP          117
#define NT_FACTOR       118
#define NT_POSTFIX      119
#define NT_ARG_LIST     120
#define NT_ARG          121
#define NT_UNARY_OP     122
#define NT_BINARY_OP    123

#define NT_FUNCTION     130


#define SEM_SYMBOL      150


#define TAG_PROGRAM     300
#define TAG_FUNCTION    301
#define TAG_INSTR       302
#define TAG_LINE        303
#define TAG_LINE_SET    304
#define TAG_TEMP        305
#define TAG_MOV         306
#define TAG_BRANCH      307
#define TAG_LABEL       308
#define TAG_LABEL_MAKE  309
#define TAG_COND        310
#define TAG_CMP         311 //
#define TAG_BLOCK       312
#define TAG_NOP         313

#define COND_EQ         385
#define COND_NE         386
#define COND_CS         387
#define COND_HS         387
#define COND_CC         388
#define COND_LO         388
#define COND_MI         389
#define COND_PL         390
#define COND_VS         391
#define COND_VC         392
#define COND_HI         393
#define COND_LS         394
#define COND_GE         395
#define COND_LT         396
#define COND_GT         397
#define COND_LE         398
#define COND_AL         399


#define ASM_PROGRAM     200
#define ASM_FUNCTION    201
#define ASM_INSTR       202
#define ASM_RET         203
#define ASM_REGISTER    204
#define ASM_LINE        205
#define ASM_LINE_SET    206
#define ASM_STACK       207
#define ASM_PROLOGUE    208
#define ASM_EPILOGUE    209
#define ASM_BLOCK       210

#define ASM_MOV         400
#define ASM_ADD         401
#define ASM_SUB         402
#define ASM_CMP         404
#define ASM_MUL         405
#define ASM_DIV         409
#define ASM_MOD         411
#define ASM_NOT         436
#define ASM_LDR         440
#define ASM_STR         445
#define ASM_B           450

#define ASM_MOVH        403
#define ASM_MULH        406
#define ASM_MULHU       407
#define ASM_MULFX       408
#define ASM_DIVU        410
#define ASM_MODU        412
#define ASM_SHL         424
#define ASM_ASR         425
#define ASM_LSR         426
#define ASM_ROL         427
#define ASM_ROR         428
#define ASM_AND         433
#define ASM_OR          434
#define ASM_XOR         435
#define ASM_BCHK        437
#define ASM_LDRB        441
#define ASM_LDRSB       442
#define ASM_LDRH        443
#define ASM_LDRSH       444
#define ASM_STRB        446
#define ASM_STRH        447
#define ASM_JMP         451

#define ASM_LABEL       490


#define LEXING_EOF      999
/*
reg 0=ret_val
reg 1=rD
reg 2=rA
reg 3=rB
reg 13=stack_start_addr (더 큰 주소, 위쪽)
reg 14=stack_end_addr
reg 15=ret_pc
*/
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

typedef struct Symbol_location {
    int type;
    int location;
} Symbol_location;

typedef struct Symbol_info {
    int name;   // IDENT의 value
    int id;     // 모든 테이블에서 겹치지 않는 고유번호. 스코프 위치 함유.
    Node * type_tree;
    int size;
    Symbol_location location;
    int is_func;    // 0: 변수, 1: 함수
    int having_body; // (함수일 경우) 0: 선언만 완료된 상태, 1: 정의까지 완료. (변수일 경우 항상 0)
} Symbol_info;

typedef struct Label_info {
    int name;   // IDENT의 value
    int id;     // 모든 테이블에서 겹치지 않는 고유번호. 스코프 위치 함유.
} Label_info;

typedef struct Switch_info {
    int is_default;   // 0: case, 1: default
    int name;   // IDENT의 value
    int id;     // 모든 테이블에서 겹치지 않는 고유번호. 스코프 위치 함유.
} Switch_info;


int lexval_manager (char *name);
int check_right_word_boundary(char right);
Lexer_result lexer(char *prep_name, char *lex_name);
Node * parser(Lexer_result lex_input, char *parse_name);
Node * semantic_analyzer(Node * parse_input, char * symbast_name);
Node * tag_generator(Node * parse_input, char * tagtree_name);
Node * code_generator(Node * parse_input, char * codegentree_name);
void code_emitter(Node * codegen_result, char * codegentree_name);

void lexer_result_printer(Lexer_result lexer_result);
void bin_tree_printer(Node * tree_top);
void bin_tree_file_printer(Node * tree_top, FILE *parsefp);
void tree_malloc_cleaner(Node * node);
Node * node_maker(Node *  n_son, Node *  n_brother, int n_num, int n_val);
Node * line_maker(int line_type, int p1_num, int p1_val, int p2_num, int p2_val, int p3_num, int p3_val);
Node * get_son(Node * node);
Node * get_brother(Node * node);


extern int temp_count;
extern Lexeme_value * lex_val_table;
extern int symbol_id_count, label_id_count;
extern Switch_info *** case_table_list;
extern int * case_table_count;


#endif
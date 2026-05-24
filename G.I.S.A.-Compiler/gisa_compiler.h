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


#define NT_PROGRAM      100
#define NT_FUNCTION     101
#define NT_PARAM        102
#define NT_CONTENT      103
#define NT_EXP          104
#define NT_UNARY_OP     105
#define NT_BINARY_OP    106


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

#define ASM_MOV         400
#define ASM_ADD         401
#define ASM_SUB         402
#define ASM_MUL         405
#define ASM_DIV         409
#define ASM_MOD         411
#define ASM_NOT         436
#define ASM_LDR         440
#define ASM_STR         445
#define ASM_B           450

#define ASM_MOVH        403
#define ASM_CMP         404
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


int lexval_manager (char *name);
int check_right_word_boundary(char right);
Lexer_result lexer(char *prep_name, char *lex_name);
Node * parser(Lexer_result lex_input, char *parse_name);
Node * tag_generator(Node * parse_input, char * tagtree_name);
Node * code_generator(Node * parse_input, char * codegentree_name);
void code_emitter(Node * codegen_result, char * codegentree_name);

void lexer_result_printer(Lexer_result lexer_result);
void bin_tree_printer(Node * tree_top);
void bin_tree_file_printer(Node * tree_top, FILE *parsefp);
void tree_malloc_cleaner(Node * node);
Node * node_maker(Node *  n_son, Node *  n_brother, int n_num, int n_val);
Node * line_maker(int line_type, int p1_num, int p1_val, int p2_num, int p2_val, int p3_num, int p3_val);


extern int temp_count;
extern Lexeme_value * lex_val_table;

#endif
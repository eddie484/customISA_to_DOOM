#include "gisa_compiler.h"


char * lexval_finder (int table_num, Lexer_result lexer_result) {
    return lexer_result.value_table[table_num].lexeme_name;
}


void asm_printer(Node * node, Lexer_result lexer_result, FILE * codeemitfp){
    //printf("<%d, %d>\n", node->token.token_number, node->token.token_value);
    
    switch (node->token.token_number) {
        case ASM_PROGRAM:
            break;
            
        case ASM_FUNCTION: {
            char * label_name = lexval_finder(node->son->brother->token.token_value, lexer_result);
            printf("%s:\n", label_name);
            fprintf(codeemitfp, "%s:\n", label_name);
            break;
        }
            
        case ASM_INSTR:
            break;
            
        case ASM_MOV: {
            char * mov_value = lexval_finder(node->son->brother->token.token_value, lexer_result);
            printf("\tMOVI R0 #%s\n", mov_value);
            fprintf(codeemitfp, "\tMOVI R0 #%s\n", mov_value);
            break;
        }
            
        case ASM_RET: {
            printf("\tJMP R15\n");
            fprintf(codeemitfp, "\tJMP R15\n");
            break;
        }
            
        case ASM_REGISTER:
            break;
            
    }


    if (node->son != NULL) {
        asm_printer(node->son, lexer_result, codeemitfp);
    }
    
    if (node->brother != NULL) {
        asm_printer(node->brother, lexer_result, codeemitfp);
    }
}



void code_emitter(Node * codegen_result, Lexer_result lexer_result, char * asmoutput_name)
{
    FILE *codeemitfp = fopen(asmoutput_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (codeemitfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("어셈블리 코드를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", asmoutput_name);
        exit(1);
    }


    asm_printer(codegen_result, lexer_result, codeemitfp);

    
    printf("Code Emitting Finished.\n");
    fclose(codeemitfp);
}
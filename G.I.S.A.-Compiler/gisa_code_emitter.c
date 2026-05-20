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
            
        case ASM_LINE: {
            switch(node->son->token.token_number) {
                case ASM_MOV: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tMOV R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tMOV R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value, lexer_result);
                        printf("\tMOVI R%d #%s\n", node->son->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tMOVI R%d #%s\n", node->son->brother->token.token_value, int_value);

                    } else {
                        printf("MOV의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_ADD: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tADD R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tADD R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value, lexer_result);
                        printf("\tADDI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tADDI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("ADD의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_SUB: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tSUB R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tSUB R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value, lexer_result);
                        printf("\tSUBI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tSUBI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("SUB의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_NOT: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tNOT R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tNOT R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value, lexer_result);
                        printf("\tNOTI R%d #%s\n", node->son->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tNOTI R%d #%s\n", node->son->brother->token.token_value, int_value);

                    } else {
                        printf("NOT의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_LDR: {
                    if (1) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value, lexer_result);
                        printf("\tLDR R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tLDR R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("LDR의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_STR: {
                    if (1) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value, lexer_result);
                        printf("\tSTR R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tSTR R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("STR의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_RET: {
                    if (1) {
                        printf("\tMOV R0 R%d\n", node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tMOV R0 R%d\n", node->son->brother->brother->brother->token.token_value);

                        printf("\tMOV R14 R13\n");
                        fprintf(codeemitfp, "\tMOV R14 R13\n");
                        
                        printf("\tLDR R13 R14 #%d\n", -((temp_count + 1) * 4));
                        fprintf(codeemitfp, "\tLDR R13 R14 #%d\n", -((temp_count + 1) * 4));

                        printf("\tJMP R15\n");
                        fprintf(codeemitfp, "\tJMP R15\n");

                    } else {
                        printf("STR의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }
            }
        }
            
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
#include "gisa_compiler.h"


char * lexval_finder (int table_num) {
    //printf("주소는: %p\n", (void *)lex_val_table);
    return lex_val_table[table_num].lexeme_name;
}


void asm_printer(Node * node, FILE * codeemitfp){
    //printf("<%d, %d>\n", node->token.token_number, node->token.token_value);
    
    switch (node->token.token_number) {
        case ASM_PROGRAM:
            break;
            
        case ASM_FUNCTION: {
            char * label_name = lexval_finder(node->son->brother->token.token_value);
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
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
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
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
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
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tSUBI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tSUBI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("SUB의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_MUL: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tMUL R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tMUL R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tMULI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tMULI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("MUL의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_DIV: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tDIV R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tDIV R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tDIVI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tDIVI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("DIV의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_MOD: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tMOD R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tMOD R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tMODI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tMODI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("MOD의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }


                case ASM_AND: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tAND R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tAND R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tANDI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tANDI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("AND의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_OR: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tOR R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tOR R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tORI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tORI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("OR의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }


                case ASM_XOR: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tXOR R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tXOR R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tXORI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tXORI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("XOR의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }


                case ASM_SHL: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tSHL R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tSHL R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tSHLI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tSHLI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("SHL의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }


                case ASM_LSR: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tLSR R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tLSR R%d R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tLSRI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tLSRI R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("LSR의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }


                case ASM_NOT: {
                    if (node->son->brother->brother->brother->token.token_number == ASM_REGISTER) {
                        printf("\tNOT R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->brother->token.token_value);
                        fprintf(codeemitfp, "\tNOT R%d R%d\n", node->son->brother->token.token_value, node->son->brother->brother->brother->token.token_value);

                    } else if (node->son->brother->brother->brother->token.token_number == NUM_INT) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tNOTI R%d #%s\n", node->son->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tNOTI R%d #%s\n", node->son->brother->token.token_value, int_value);

                    } else {
                        printf("NOT의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_LDR: {
                    if (1) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
                        printf("\tLDR R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);
                        fprintf(codeemitfp, "\tLDR R%d R%d #%s\n", node->son->brother->token.token_value, node->son->brother->brother->token.token_value, int_value);

                    } else {
                        printf("LDR의 인자로 잘못된 형식이 입력되었습니다: <%d, %d>", node->son->brother->brother->brother->token.token_number, node->son->brother->brother->brother->token.token_value);
                    }
                    break;
                }

                case ASM_STR: {
                    if (1) {
                        char * int_value = lexval_finder(node->son->brother->brother->brother->token.token_value);
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
        asm_printer(node->son, codeemitfp);
    }
    
    if (node->brother != NULL) {
        asm_printer(node->brother, codeemitfp);
    }
}



void code_emitter(Node * codegen_result, char * asmoutput_name)
{
    FILE *codeemitfp = fopen(asmoutput_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (codeemitfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("어셈블리 코드를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", asmoutput_name);
        exit(1);
    }


    asm_printer(codegen_result, codeemitfp);

    
    printf("Code Emitting Finished.\n");
    fclose(codeemitfp);
}
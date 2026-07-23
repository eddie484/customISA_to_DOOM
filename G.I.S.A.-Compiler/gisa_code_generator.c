#include "gisa_compiler.h"


Node * asm_pass1_terminal(Node * tag);
Node * asm_pass1_nt_program(Node * tag);
Node * asm_pass1_nt_function(Node * tag);
Node * asm_pass1_nt_param_receive(Node * tag);
Node * asm_pass1_nt_block(Node * tag);
Node * asm_pass1_nt_instr(Node * tag);
Node * asm_pass1_nt_instr_loop(Node * tag);
Node * asm_pass1_nt_return(Node * tag);

void asm_pass2_temp_to_stack(Node * node);
void asm_pass3_prologue_maker(Node * node);




/* ***********************************
**************************************
**************************************
***********                ***********
***********    PASS ONE    ***********
***********                ***********
**************************************
**************************************
*********************************** */

Node * asm_pass1_terminal(Node * tag){
    if (tag->token.token_number >= IDENT && tag->token.token_number <= PN_SEMI) {
        printf("Processing: terminal <%d, %d>\n", tag->token.token_number, tag->token.token_value);
        Node * n = node_maker(NULL, NULL, tag->token.token_number, tag->token.token_value);

        return n;
    } else {
        printf("TAG to asm tree pass1 과정에서 오류 발생: terminal node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", tag->token.token_number);
        exit(1);
    }
}

Node * asm_pass1_nt_program(Node * tag){
    if (tag->token.token_number == TAG_PROGRAM) {
        printf("Processing: asm_pass1_nt_program\n");
        Node * x1 = asm_pass1_nt_function(tag->son);
        Node * n = node_maker(x1, NULL, ASM_PROGRAM, 0);

        return n;
    } else {
        printf("TAG to asm tree pass1 과정에서 오류 발생: TAG_PROGRAM node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", tag->token.token_number);
        exit(1);
    }
}

Node * asm_pass1_nt_function(Node * tag){
    if (tag != NULL && tag->token.token_number == TAG_FUNCTION) {
        Node * brother_func = NULL;
        if (tag->brother != NULL && tag->brother->token.token_number == TAG_FUNCTION) {
            printf("Processing: brother function을 처리합니다.\n");
            brother_func = asm_pass1_nt_function(tag->brother);
        }
        
        printf("Processing: asm_pass1_nt_function\n");
        Node * x1 = asm_pass1_nt_instr_loop(tag->son);
        Node * x2 = asm_pass1_nt_param_receive(tag->son->brother);
        Node * x3 = asm_pass1_nt_block(tag->son->brother->brother);

        x1->brother = x2;
        x2->brother = x3;

        Node * n = node_maker(x1, brother_func, ASM_FUNCTION, 0);

        return n;
    } else if (tag == NULL) {
        printf("TAG to asm tree pass1: 모든 함수 처리가 완료되었습니다.\n");
        return NULL;
    } else {
        printf("TAG to asm tree pass1 과정에서 오류 발생: NT_FUNCTION node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", tag->token.token_number);
        exit(1);
    }
}

Node * asm_pass1_nt_param_receive(Node * tag){
    // 인자들 값 가져오기 작업 할 곳!
    int param_count = 0;

    if (tag->son != NULL) {
        param_count++;
        Node * couning_current_param = tag->son->brother;
        while (couning_current_param != NULL) {
            param_count++;
            couning_current_param = couning_current_param->brother;
        }
    }


    Node * param_get_set = node_maker(NULL, NULL, ASM_LINE_SET, 0);
    if (param_count >= 1) {
        Node * param_1 = line_maker(ASM_MOV, TAG_TEMP, tag->son->token.token_value, TAG_TEMP, 0, ASM_REGISTER, 4);
        param_get_set->son = param_1;

        if (param_count >= 2) {
            Node * param_2 = line_maker(ASM_MOV, TAG_TEMP, tag->son->brother->token.token_value, TAG_TEMP, 0, ASM_REGISTER, 5);
            param_1->brother = param_2;

            if (param_count >= 3) {
                Node * param_3 = line_maker(ASM_MOV, TAG_TEMP, tag->son->brother->brother->token.token_value, TAG_TEMP, 0, ASM_REGISTER, 6);
                param_2->brother = param_3;

                if (param_count >= 4) {
                    // 4개 이상 인자들 스택에서 뽑아오기 구현해야 함.
                }
            }
        }
    }

    return param_get_set;
}

Node * asm_pass1_nt_func_call(Node * tag){
    // 인자들 값 저장하고 함수로 점프하는 작업 할 곳!
    Node * param_set = node_maker(NULL, NULL, ASM_LINE_SET, 0);

    if (tag->son != NULL) {
        Node * param_instr_1 = asm_pass1_nt_instr_loop(tag->son);
        Node * param_save_1 = line_maker(ASM_MOV, ASM_REGISTER, 4, TAG_TEMP, 0, TAG_TEMP, param_instr_1->token.token_value);
        param_set->son = param_instr_1;
        param_instr_1->brother = param_save_1;

        if (tag->son->brother != NULL) {
            Node * param_instr_2 = asm_pass1_nt_instr_loop(tag->son->brother);
            Node * param_save_2 = line_maker(ASM_MOV, ASM_REGISTER, 5, TAG_TEMP, 0, TAG_TEMP, param_instr_2->token.token_value);
            param_save_1->brother = param_instr_2;
            param_instr_2->brother = param_save_2;

            if (tag->son->brother->brother != NULL) {
                Node * param_instr_3 = asm_pass1_nt_instr_loop(tag->son->brother->brother);
                Node * param_save_3 = line_maker(ASM_MOV, ASM_REGISTER, 6, TAG_TEMP, 0, TAG_TEMP, param_instr_3->token.token_value);
                param_save_2->brother = param_instr_3;
                param_instr_3->brother = param_save_3;

                // 4개 이상 인자들 스택에 넣기 구현해야 함.
            }
        }
    }

    
    Node * func_calling = line_maker(ASM_FUNC_CALL, ASM_REGISTER, 15, TAG_TEMP, 0, TAG_LABEL, tag->token.token_value);
    Node * ret_val_saving = line_maker(ASM_MOV, TAG_TEMP, tag->brother->token.token_value, TAG_TEMP, 0, ASM_REGISTER, 0);

    param_set->brother = func_calling;
    func_calling->brother = ret_val_saving;

    Node * n = node_maker(param_set, NULL, ASM_LINE_SET, 0);

    return n;
}

Node * asm_pass1_nt_block(Node * tag){
    if (tag->token.token_number == TAG_BLOCK) {
        printf("Processing: asm_pass1_nt_block\n");
        Node * x1 = asm_pass1_nt_instr(tag->son);
        Node * n = node_maker(x1, NULL, ASM_BLOCK, 0);    // Node로 감싸지 않으면 brother로 묶여있던 ASM_INSTR이 끊기는 문제 발생.

        return n;
    } else {
        printf("TAG to asm tree pass1 과정에서 오류 발생: TAG_BLOCK node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", tag->token.token_number);
        exit(1);
    }
}

Node * asm_pass1_nt_instr(Node * tag){
    if (tag->token.token_number == TAG_INSTR) {
        printf("Processing: asm_pass1_nt_instr\n");
        Node * x1 = asm_pass1_nt_instr_loop(tag->son);
        Node * n = node_maker(x1, NULL, ASM_INSTR, 0);

        if (tag->brother != NULL) {
            Node * n1 = asm_pass1_nt_instr(tag->brother);
            n->brother = n1;
        }

        return n;
    } else {
        printf("TAG to asm tree pass1 과정에서 오류 발생: NT_CONTENT node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", tag->token.token_number);
        exit(1);
    }
}

Node * asm_pass1_nt_instr_loop(Node * tag){
    if (tag->token.token_number == TAG_LINE_SET) {
        Node * x1 = asm_pass1_nt_instr_loop(tag->son);
        Node * n = node_maker(x1, NULL, ASM_LINE_SET, tag->token.token_value);

        if (tag->brother != NULL) {
            Node * n1 = asm_pass1_nt_instr_loop(tag->brother);
            n->brother = n1;
        } else {
            n->brother = NULL;
        }
        
        return n;
    } else if (tag->token.token_number == TAG_LINE){
        if (tag->son->token.token_number == KW_RETURN){
            Node * n = line_maker(ASM_RET, TAG_TEMP, 0, TAG_TEMP, 0, TAG_TEMP, tag->son->brother->brother->brother->token.token_value);

            if (tag->brother != NULL) {
                Node * n1 = asm_pass1_nt_instr_loop(tag->brother);
                n->brother = n1;
            } else {
                n->brother = NULL;
            }

            return n;
        } else if (tag->son->token.token_number == OP_NEG && tag->son->brother->brother->token.token_number == TAG_TEMP && tag->son->brother->brother->token.token_value == 0) {
            Node * n1 = line_maker(ASM_MOV, ASM_REGISTER, 2, TAG_TEMP, 0, NUM_INT, lexval_manager ("0"));
            Node * n2 = line_maker(ASM_SUB, tag->son->brother->token.token_number, tag->son->brother->token.token_value, ASM_REGISTER, 2, tag->son->brother->brother->brother->token.token_number, tag->son->brother->brother->brother->token.token_value);

            n1->brother = n2;

            Node * n = node_maker(n1, NULL, ASM_LINE_SET, tag->token.token_value);

            if (tag->brother != NULL) {
                Node * n1 = asm_pass1_nt_instr_loop(tag->brother);
                n->brother = n1;
            } else {
                n->brother = NULL;
            }

            return n;
        } else {
            Node * x4 = node_maker(NULL, NULL, tag->son->brother->brother->brother->token.token_number, tag->son->brother->brother->brother->token.token_value);
            Node * x3 = node_maker(NULL, x4, tag->son->brother->brother->token.token_number, tag->son->brother->brother->token.token_value);
            Node * x2 = node_maker(NULL, x3, tag->son->brother->token.token_number, tag->son->brother->token.token_value);
            Node * x1 = malloc(sizeof(Node));
            Node * n = node_maker(x1, NULL, ASM_LINE, tag->token.token_value);
            
            x1->son = NULL;
            x1->brother = x2;

            switch (tag->son->token.token_number) {
                case TAG_MOV:
                    x1->token.token_number = ASM_MOV;
                    break;
                    
                case OP_TILDE:
                    x1->token.token_number = ASM_NOT;
                    break;
                    
                case OP_ADD:
                    x1->token.token_number = ASM_ADD;
                    break;
                    
                case OP_SUB:
                    x1->token.token_number = ASM_SUB;
                    break;
                    
                case OP_MUL:
                    x1->token.token_number = ASM_MUL;
                    break;
                    
                case OP_DIV:
                    x1->token.token_number = ASM_DIV;
                    break;
                    
                case OP_MOD:
                    x1->token.token_number = ASM_MOD;
                    break;
                    
                case OP_AND:
                    x1->token.token_number = ASM_AND;
                    break;
                    
                case OP_OR:
                    x1->token.token_number = ASM_OR;
                    break;
                    
                case OP_XOR:
                    x1->token.token_number = ASM_XOR;
                    break;
                    
                case OP_SHL:
                    x1->token.token_number = ASM_SHL;
                    break;
                    
                case OP_LSR:
                    x1->token.token_number = ASM_LSR;
                    break;
                    
                case TAG_CMP:
                    x1->token.token_number = ASM_CMP;
                    break;
                    
                case TAG_LABEL_MAKE:
                    x1->token.token_number = ASM_LABEL;
                    break;
                    
                case TAG_BRANCH:
                    x1->token.token_number = ASM_B;
                    break;
                    
                default:
                    x1->token = tag->son->token;
                    break;
            }
            x1->token.token_value = 0;

            if (tag->brother != NULL) {
                Node * n1 = asm_pass1_nt_instr_loop(tag->brother);
                n->brother = n1;
            } else {
                n->brother = NULL;
            }

            return n;
        }
    } else if (tag->token.token_number == TAG_BLOCK) {
        Node * n = asm_pass1_nt_block(tag);

        if (tag->brother != NULL) {
            Node * n1 = asm_pass1_nt_instr_loop(tag->brother);
            n->brother = n1;
        } else {
            n->brother = NULL;
        }

        return n;
    } else if (tag->token.token_number == TAG_NOP) {
        if (tag->brother != NULL) {
            Node * n = asm_pass1_nt_instr_loop(tag->brother);
            return n;
        } else {
            return NULL;
        }
        
    } else if (tag->token.token_number == TAG_FUNC_CALL) {
        Node * n = asm_pass1_nt_func_call(tag);
        return n;
    } else if (tag->token.token_number == TAG_PARAM_LIST) {
        return NULL;    // 재귀 방식 문제로 인해 잘못 호출되는 것임. 원래는 시작라벨의 brother로, instr_loop에서 호출되면 안됨. 중간5의 문제와 같은 원인.
    } else {
        printf("instr loop 중 잘못된 태그 토큰을 받았습니다: <%d, %d>\n", tag->token.token_number, tag->token.token_value);
        exit(1);
    }
}




/* ***********************************
**************************************
**************************************
***********                ***********
***********    PASS TWO    ***********
***********                ***********
**************************************
**************************************
*********************************** */

void asm_pass2_temp_to_stack(Node * node) {
    if (node->son != NULL) {
        asm_pass2_temp_to_stack(node->son);
    }
    
    if (node->brother != NULL) {
        asm_pass2_temp_to_stack(node->brother);
    }

    if (node->token.token_number == ASM_LINE) {
        // rD가 임시 레지스터일 경우
        if (node->son->brother->token.token_number == TAG_TEMP && node->son->brother->token.token_value != 0) { // rD

            int n = -(4 * node->son->brother->token.token_value);
            char str[12];
            snprintf(str, sizeof(str), "%d", n);

            Node * store_rD = line_maker(ASM_STR, ASM_REGISTER, 1, ASM_REGISTER, 13, NUM_INT, lexval_manager (str));
            store_rD->brother = node->brother;

            node->son->brother->token.token_number = ASM_REGISTER;
            node->son->brother->token.token_value = 1;
            node->brother = store_rD;
        }

        // rA가 임시변수일 경우
        if (node->son->brother->brother->token.token_number == TAG_TEMP && node->son->brother->brother->token.token_value != 0) { // rA

            int n = -(4 * node->son->brother->brother->token.token_value);
            char str[12];
            snprintf(str, sizeof(str), "%d", n);

            Node * original_line_node = line_maker(ASM_LDR, ASM_REGISTER, 2, ASM_REGISTER, 13, NUM_INT, lexval_manager (str));
            Node * son = original_line_node->son;

            original_line_node->son = node->son;
            original_line_node->brother = node->brother;
            original_line_node->token.token_number = node->token.token_number;
            original_line_node->token.token_value = node->token.token_value;

            node->son = son;
            node->brother = original_line_node;
            node->token.token_number = ASM_LINE;
            node->token.token_value = 0;

            original_line_node->son->brother->brother->token.token_number = ASM_REGISTER;
            original_line_node->son->brother->brother->token.token_value = 2;

            // rA가 임시변수이면서, 동시에 rB가 임시변수일 경우
            if (original_line_node->son->brother->brother->brother->token.token_number == TAG_TEMP && original_line_node->son->brother->brother->brother->token.token_value != 0) { // rB

                int n = -(4 * original_line_node->son->brother->brother->brother->token.token_value);
                char str[12];
                snprintf(str, sizeof(str), "%d", n);

                Node * line_ldr_rb = line_maker(ASM_LDR, ASM_REGISTER, 3, ASM_REGISTER, 13, NUM_INT, lexval_manager (str));

                node->brother = line_ldr_rb;
                line_ldr_rb->brother = original_line_node;

                original_line_node->son->brother->brother->brother->token.token_number = ASM_REGISTER;
                original_line_node->son->brother->brother->brother->token.token_value = 3;
            }

        // rA가 임시변수가 아니고, rB가 임시변수일 경우
        } else if (node->son->brother->brother->brother->token.token_number == TAG_TEMP && node->son->brother->brother->brother->token.token_value != 0) { // rB

            int n = -(4 * node->son->brother->brother->brother->token.token_value);
            char str[12];
            snprintf(str, sizeof(str), "%d", n);

            Node * original_line_node = line_maker(ASM_LDR, ASM_REGISTER, 3, ASM_REGISTER, 13, NUM_INT, lexval_manager (str));
            Node * son = original_line_node->son;

            original_line_node->son = node->son;
            original_line_node->brother = node->brother;
            original_line_node->token.token_number = node->token.token_number;
            original_line_node->token.token_value = node->token.token_value;

            node->son = son;
            node->brother = original_line_node;
            node->token.token_number = ASM_LINE;
            node->token.token_value = 0;

            original_line_node->son->brother->brother->brother->token.token_number = ASM_REGISTER;
            original_line_node->son->brother->brother->brother->token.token_value = 3;
        }
    }
}




/* *************************************
****************************************
****************************************
***********                  ***********
***********    PASS THREE    ***********
***********                  ***********
****************************************
****************************************
************************************* */

void asm_pass3_prologue_maker(Node * node) {
    if (node->son != NULL) {
        asm_pass3_prologue_maker(node->son);
    }
    
    if (node->brother != NULL) {
        asm_pass3_prologue_maker(node->brother);
    }

    if (node->token.token_number == ASM_FUNCTION) {
        // STR R13 R14 -(temp_count * 4 + 1)
        int n = -((temp_count + 1) * 4);
        char str[12];
        snprintf(str, sizeof(str), "%d", n);
        Node * p1 = line_maker(ASM_STR, ASM_REGISTER, 13, ASM_REGISTER, 14, NUM_INT, lexval_manager (str));

        // MOV R13 R14
        Node * p2 = line_maker(ASM_MOV, ASM_REGISTER, 13, TAG_TEMP, 0, ASM_REGISTER, 14);

        // ADD R14 R13 -(temp_count * 4 + 1)
        n = -((temp_count + 1) * 4);
        snprintf(str, sizeof(str), "%d", n);
        Node * p3 = line_maker(ASM_ADD, ASM_REGISTER, 14, ASM_REGISTER, 13, NUM_INT, lexval_manager (str));
        

        p1->brother = p2;
        p2->brother = p3;
        
        Node * prologue = node_maker(p1, node->son->brother, ASM_PROLOGUE, 0);
        node->son->brother = prologue;
    }

}




Node * code_generator(Node * parse_input, char * codegentree_name)
{
    Node * asm_pass1_top;
    Node * asm_pass2_top;
    Node * asm_pass3_top;


    FILE *codegenfp = fopen(codegentree_name, "w");         // 처리 결과 tag를 저장할 파일 오픈

    if (codegenfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Parse 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", codegentree_name);
        exit(1);
    }


    asm_pass1_top = asm_pass1_nt_program(parse_input);
    bin_tree_printer(asm_pass1_top);

    printf("Code Generating Pass 1 Finished.\n");

    asm_pass2_temp_to_stack(asm_pass1_top);
    bin_tree_printer(asm_pass1_top);

    printf("Code Generating Pass 2 Finished.\n");

    asm_pass3_prologue_maker(asm_pass1_top);
    bin_tree_printer(asm_pass1_top);

    printf("Code Generating Pass 3 Finished.\n");
    

    bin_tree_file_printer(asm_pass1_top, codegenfp);

    printf("Code Generating Finished.\n");
    fclose(codegenfp);
    return asm_pass1_top;
}
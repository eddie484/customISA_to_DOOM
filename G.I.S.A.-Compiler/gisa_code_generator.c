#include "gisa_compiler.h"


Node * asm_pass1_terminal(Node * tag);
Node * asm_pass1_nt_program(Node * tag);
Node * asm_pass1_nt_function(Node * tag);
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
    if (tag->token.token_number >= 0 && tag->token.token_number <= 9) {
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
    if (tag->token.token_number == TAG_FUNCTION) {
        printf("Processing: asm_pass1_nt_function\n");
        Node * x1 = asm_pass1_terminal(tag->son);
        Node * x2 = asm_pass1_terminal(tag->son->brother);
        Node * x3 = asm_pass1_terminal(tag->son->brother->brother);
        Node * x4 = asm_pass1_nt_instr(tag->son->brother->brother->brother);

        x1->brother = x2;
        x2->brother = x3;
        x3->brother = x4;

        Node * n = node_maker(x1, NULL, ASM_FUNCTION, 0);

        return n;
    } else {
        printf("TAG to asm tree pass1 과정에서 오류 발생: NT_FUNCTION node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", tag->token.token_number);
        exit(1);
    }
}

Node * asm_pass1_nt_instr(Node * tag){
    if (tag->token.token_number == TAG_INSTR) {
        printf("Processing: asm_pass1_nt_instr\n");
        Node * x1 = asm_pass1_nt_instr_loop(tag->son);
        Node * n = node_maker(x1, NULL, ASM_INSTR, 0);

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
        } else if (tag->son->token.token_number == OP_MINUS && tag->son->brother->brother->token.token_number == TAG_TEMP && tag->son->brother->brother->token.token_value == 0) {
            char str[12] = "0";
            Node * n1 = line_maker(ASM_MOV, ASM_REGISTER, 2, TAG_TEMP, 0, NUM_INT, lexval_manager (str));
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
                    x1->token.token_value = 0;
                    break;
                    
                case OP_TILDE:
                    x1->token.token_number = ASM_NOT;
                    x1->token.token_value = 0;
                    break;
                    
                case OP_MINUS:
                    x1->token.token_number = ASM_SUB;
                    x1->token.token_value = 0;
                    break;
                    
                default:
                    x1->token = tag->son->token;
                    break;
            }

            if (tag->brother != NULL) {
                Node * n1 = asm_pass1_nt_instr_loop(tag->brother);
                n->brother = n1;
            } else {
                n->brother = NULL;
            }

            return n;
        }
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
        }

        // rB가 임시변수일 경우
        if (node->son->brother->brother->brother->token.token_number == TAG_TEMP && node->son->brother->brother->brother->token.token_value != 0) { // rB

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
        str[12];
        snprintf(str, sizeof(str), "%d", n);
        Node * p3 = line_maker(ASM_ADD, ASM_REGISTER, 14, ASM_REGISTER, 13, NUM_INT, lexval_manager (str));
        

        p1->brother = p2;
        p2->brother = p3;
        
        
        Node * prologue = node_maker(p1, node->son->brother->brother->brother, ASM_PROLOGUE, 0);
        node->son->brother->brother->brother = prologue;
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

    asm_pass2_temp_to_stack(asm_pass1_top);
    bin_tree_printer(asm_pass1_top);

    asm_pass3_prologue_maker(asm_pass1_top);
    bin_tree_printer(asm_pass1_top);
    

    bin_tree_file_printer(asm_pass1_top, codegenfp);

    printf("Code Generating Finished.\n");
    fclose(codegenfp);
    return asm_pass1_top;
}
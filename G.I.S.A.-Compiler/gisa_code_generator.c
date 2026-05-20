#include "gisa_compiler.h"


Node * asm_pass1_terminal(Node * tag);
Node * asm_pass1_nt_program(Node * tag);
Node * asm_pass1_nt_function(Node * tag);
Node * asm_pass1_nt_instr(Node * tag);
Node * asm_pass1_nt_instr_loop(Node * tag);
Node * asm_pass1_nt_return(Node * tag);

void asm_pass2_testing1(Node * node);




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
        Node * n = malloc(sizeof(Node));
        n->token = tag->token;

        n->son = NULL;
        n->brother = NULL;

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

        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = ASM_PROGRAM;

        n->brother = NULL;
        n->token.token_value = 0;

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

        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = ASM_FUNCTION;

        n->brother = NULL;
        n->token.token_value = 0;

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

        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = ASM_INSTR;

        n->brother = NULL;
        n->token.token_value = 0;

        return n;
    } else {
        printf("TAG to asm tree pass1 과정에서 오류 발생: NT_CONTENT node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", tag->token.token_number);
        exit(1);
    }
}

Node * asm_pass1_nt_instr_loop(Node * tag){
    if (tag->token.token_number == TAG_LINE_SET) {
        Node * n = malloc(sizeof(Node));
        Node * x1 = asm_pass1_nt_instr_loop(tag->son);

        n->son = x1;
        n->token.token_number = ASM_LINE_SET;
        n->token.token_value = tag->token.token_value;

        if (tag->brother != NULL) {
            Node * n1 = asm_pass1_nt_instr_loop(tag->brother);
            n->brother = n1;
        } else {
            n->brother = NULL;
        }
        
        return n;
    } else if (tag->token.token_number == TAG_LINE){
        if (tag->son->token.token_number == KW_RETURN){
            Node * n1 = malloc(sizeof(Node));
            Node * n2 = malloc(sizeof(Node));

            Node * n1_x1 = malloc(sizeof(Node));
            Node * n1_x2 = malloc(sizeof(Node));
            Node * n1_x3 = malloc(sizeof(Node));
            Node * n1_x4 = malloc(sizeof(Node));
            
            Node * n2_x1 = malloc(sizeof(Node));
            Node * n2_x2 = malloc(sizeof(Node));
            Node * n2_x3 = malloc(sizeof(Node));
            Node * n2_x4 = malloc(sizeof(Node));

            n1->son = n1_x1;
            n1->brother = n2;
            n1->token.token_number = ASM_LINE;
            n1->token.token_value = 0;

            n2->son = n2_x1;
            n2->brother = NULL;
            n2->token.token_number = ASM_LINE;
            n2->token.token_value = 0;

            n1_x1->son = NULL;
            n1_x1->brother = n1_x2;
            n1_x1->token.token_number = ASM_MOV;
            n1_x1->token.token_value = 0;

            n1_x2->son = NULL;
            n1_x2->brother = n1_x3;
            n1_x2->token.token_number = ASM_REGISTER;
            n1_x2->token.token_value = 0;

            n1_x3->son = NULL;
            n1_x3->brother = n1_x4;
            n1_x3->token.token_number = TAG_TEMP;
            n1_x3->token.token_value = 0;

            n1_x4->son = NULL;
            n1_x4->brother = NULL;
            n1_x4->token.token_number = TAG_TEMP;
            n1_x4->token.token_value = tag->son->brother->brother->brother->token.token_value;

            n2_x1->son = NULL;
            n2_x1->brother = n2_x2;
            n2_x1->token.token_number = ASM_JUMP;
            n2_x1->token.token_value = 0;

            n2_x2->son = NULL;
            n2_x2->brother = n2_x3;
            n2_x2->token.token_number = TAG_TEMP;
            n2_x2->token.token_value = 0;

            n2_x3->son = NULL;
            n2_x3->brother = n2_x4;
            n2_x3->token.token_number = TAG_TEMP;
            n2_x3->token.token_value = 0;

            n2_x4->son = NULL;
            n2_x4->brother = NULL;
            n2_x4->token.token_number = ASM_REGISTER;
            n2_x4->token.token_value = 15;

            Node * n = malloc(sizeof(Node));
            n->son = n1;
            n->token.token_number = ASM_LINE_SET;
            n->token.token_value = tag->token.token_value;

            if (tag->brother != NULL) {
                Node * n1 = asm_pass1_nt_instr_loop(tag->brother);
                n->brother = n1;
            } else {
                n->brother = NULL;
            }

            return n;
        } else {
            Node * n = malloc(sizeof(Node));
            Node * x1 = malloc(sizeof(Node));
            Node * x2 = malloc(sizeof(Node));
            Node * x3 = malloc(sizeof(Node));
            Node * x4 = malloc(sizeof(Node));
            
            x1->son = NULL;
            x1->brother = x2;
            x1->token = tag->son->token;
            
            x2->son = NULL;
            x2->brother = x3;
            x2->token = tag->son->brother->token;
            
            x3->son = NULL;
            x3->brother = x4;
            x3->token = tag->son->brother->brother->token;
            
            x4->son = NULL;
            x4->brother = NULL;
            x4->token = tag->son->brother->brother->brother->token;

            n->son = x1;
            n->token.token_number = ASM_LINE;
            n->token.token_value = tag->token.token_value;

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

void asm_pass2_testing1(Node * node) {
    if (node->son != NULL) {
        asm_pass2_testing1(node->son);
    }
    
    if (node->brother != NULL) {
        asm_pass2_testing1(node->brother);
    }

    if (node->token.token_number == ASM_LINE) {
        // rD가 임시 레지스터일 경우
        if (node->son->brother->token.token_number == TAG_TEMP && node->son->brother->token.token_value != 0) { // rD
            Node * store_rD = malloc(sizeof(Node));
            Node * x1 = malloc(sizeof(Node));
            Node * x2 = malloc(sizeof(Node));
            Node * x3 = malloc(sizeof(Node));
            Node * x4 = malloc(sizeof(Node));

            x1->son = NULL;
            x1->brother = x2;
            x1->token.token_number = ASM_STR;
            x1->token.token_value = 0;

            x2->son = NULL;
            x2->brother = x3;
            x2->token.token_number = ASM_REGISTER;
            x2->token.token_value = 1;

            x3->son = NULL;
            x3->brother = x4;
            x3->token.token_number = ASM_REGISTER;
            x3->token.token_value = 13;

            x4->son = NULL;
            x4->brother = NULL;
            x4->token.token_number = NUM_INT;
            x4->token.token_value = -(4 * node->son->brother->token.token_value);

            store_rD->son = x1;
            store_rD->brother = node->brother;
            store_rD->token.token_number = ASM_LINE;
            store_rD->token.token_value = 0;

            node->son->brother->token.token_number = ASM_REGISTER;
            node->son->brother->token.token_value = 1;
            node->brother = store_rD;
        }

        // rA가 임시변수일 경우
        if (node->son->brother->brother->token.token_number == TAG_TEMP && node->son->brother->brother->token.token_value != 0) { // rA
            Node * original_line_node = malloc(sizeof(Node));
            Node * x1 = malloc(sizeof(Node));
            Node * x2 = malloc(sizeof(Node));
            Node * x3 = malloc(sizeof(Node));
            Node * x4 = malloc(sizeof(Node));

            x1->son = NULL;
            x1->brother = x2;
            x1->token.token_number = ASM_LDR;
            x1->token.token_value = 0;

            x2->son = NULL;
            x2->brother = x3;
            x2->token.token_number = ASM_REGISTER;
            x2->token.token_value = 2;

            x3->son = NULL;
            x3->brother = x4;
            x3->token.token_number = ASM_REGISTER;
            x3->token.token_value = 13;

            x4->son = NULL;
            x4->brother = NULL;
            x4->token.token_number = NUM_INT;
            x4->token.token_value = -(4 * node->son->brother->brother->token.token_value);

            original_line_node->son = node->son;
            original_line_node->brother = node->brother;
            original_line_node->token.token_number = node->token.token_number;
            original_line_node->token.token_value = node->token.token_value;

            node->son = x1;
            node->brother = original_line_node;
            node->token.token_number = ASM_LINE;
            node->token.token_value = 0;

            original_line_node->son->brother->brother->token.token_number = ASM_REGISTER;
            original_line_node->son->brother->brother->token.token_value = 2;
        }

        // rB가 임시변수일 경우
        if (node->son->brother->brother->brother->token.token_number == TAG_TEMP && node->son->brother->brother->brother->token.token_value != 0) { // rB
            Node * original_line_node = malloc(sizeof(Node));
            Node * x1 = malloc(sizeof(Node));
            Node * x2 = malloc(sizeof(Node));
            Node * x3 = malloc(sizeof(Node));
            Node * x4 = malloc(sizeof(Node));

            x1->son = NULL;
            x1->brother = x2;
            x1->token.token_number = ASM_LDR;
            x1->token.token_value = 0;

            x2->son = NULL;
            x2->brother = x3;
            x2->token.token_number = ASM_REGISTER;
            x2->token.token_value = 3;

            x3->son = NULL;
            x3->brother = x4;
            x3->token.token_number = ASM_REGISTER;
            x3->token.token_value = 13;

            x4->son = NULL;
            x4->brother = NULL;
            x4->token.token_number = NUM_INT;
            x4->token.token_value = -(4 * node->son->brother->brother->brother->token.token_value);

            original_line_node->son = node->son;
            original_line_node->brother = node->brother;
            original_line_node->token.token_number = node->token.token_number;
            original_line_node->token.token_value = node->token.token_value;

            node->son = x1;
            node->brother = original_line_node;
            node->token.token_number = ASM_LINE;
            node->token.token_value = 0;

            original_line_node->son->brother->brother->brother->token.token_number = ASM_REGISTER;
            original_line_node->son->brother->brother->brother->token.token_value = 3;
        }
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

    asm_pass2_testing1(asm_pass1_top);
    bin_tree_printer(asm_pass1_top);

    //asm_pass2_top = asm_pass2_nt_program(asm_pass1_top);
    //bin_tree_printer(asm_pass2_top);

    //asm_pass3_top = asm_pass3_nt_program(asm_pass2_top);
    //bin_tree_printer(asm_pass3_top);
    bin_tree_file_printer(asm_pass1_top, codegenfp);

    printf("Code Generating Finished.\n");
    fclose(codegenfp);
    return asm_pass1_top;
}
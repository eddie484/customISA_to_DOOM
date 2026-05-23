/* ****************************************************
*******************************************************
*******************************************************
***********                                 ***********
***********    TAG: Three-Addr GISA Code    ***********
***********                                 ***********
*******************************************************
*******************************************************
*******************************************************

    AST
    [[NUM_INT]]

    TAG
    NUM_INT


    AST
    [[<unary_op> (exp)]]

    TAG
    t1 = [[exp]]
    t = <unary_op> t1


    AST
    [[<binary_op> (exp)]]

    TAG
    t1 = [[exp]]
    t2 = [[exp]]
    t = <unary_op> t1 t2


    AST
    [[KW_RETURN (exp)]]

    TAG
    t1 = [[exp]]
    KW_RETURN t1

*/

#include "gisa_compiler.h"


int temp_count = 1;     // 0은 처음 입력값이므로 1부터 임시변수 시작.
int label_count = 1;

Node * tag_terminal(Node * ast);
Node * tag_nt_program(Node * ast);
Node * tag_nt_function(Node * ast);
Node * tag_nt_instr(Node * ast);
Node * tag_nt_instr_interpreting(Node * ast, int temp_in_rA, int temp_in_rB);


Node * tag_terminal(Node * ast){
    if (ast->token.token_number >= 0 && ast->token.token_number <= 9) {
        printf("Processing: terminal <%d, %d>\n", ast->token.token_number, ast->token.token_value);

        Node * n = node_maker(NULL, NULL, ast->token.token_number, ast->token.token_value);

        return n;
    } else {
        printf("AST to TAG 과정에서 오류 발생: terminal node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}

Node * tag_nt_program(Node * ast){
    if (ast->token.token_number == NT_PROGRAM) {
        printf("Processing: tag_nt_program\n");
        Node * x1 = tag_nt_function(ast->son);

        Node * n = node_maker(x1, NULL, TAG_PROGRAM, 0);

        return n;
    } else {
        printf("AST to TAG 과정에서 오류 발생: NT_PROGRAM node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}

Node * tag_nt_function(Node * ast){
    if (ast->token.token_number == NT_FUNCTION) {
        printf("Processing: tag_nt_function\n");
        Node * x1 = tag_terminal(ast->son);
        Node * x2 = tag_terminal(ast->son->brother);
        Node * x3 = tag_terminal(ast->son->brother->brother);
        Node * x4 = tag_nt_instr(ast->son->brother->brother->brother);

        x1->brother = x2;
        x2->brother = x3;
        x3->brother = x4;

        Node * n = node_maker(x1, NULL, TAG_FUNCTION, 0);

        return n;
    } else {
        printf("AST to TAG 과정에서 오류 발생: NT_FUNCTION node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}

Node * tag_nt_instr(Node * ast){
    if (ast->token.token_number == NT_CONTENT) {
        printf("Processing: tag_nt_instr\n");
        Node * x1 = tag_nt_instr_interpreting(ast->son, 0, 0);

        Node * n = node_maker(x1, NULL, TAG_INSTR, 0);

        return n;
    } else {
        printf("AST to TAG 과정에서 오류 발생: NT_CONTENT node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}


Node * tag_nt_instr_interpreting(Node * ast, int temp_in_rA, int temp_in_rB){
    if (ast->token.token_number == KW_RETURN) {
        printf("enter KW_RETURN\n");
        Node * n1 = tag_nt_instr_interpreting(ast->brother, temp_in_rA, temp_in_rB);
        Node * n2 = line_maker(KW_RETURN, TAG_TEMP, 0, TAG_TEMP, 0, TAG_TEMP, n1->token.token_value);

        n2->token.token_value = n2->son->brother->token.token_value;
        n1->brother = n2;

        Node * n = node_maker(n1, NULL, TAG_LINE_SET, n2->token.token_value);

        return n;

    } else if (ast->token.token_number == NT_EXP) {
        printf("enter NT_EXP\n");
        if (ast->son->token.token_number == OP_TILDE || ast->son->token.token_number == OP_NEG || ast->son->token.token_number == OP_LOGIC_NOT) {
            Node * n1 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);
            Node * n2 = tag_nt_instr_interpreting(ast->son, 0, n1->token.token_value);

            n1->brother = n2;

            Node * n = node_maker(n1, NULL, TAG_LINE_SET, n2->token.token_value);

            return n;
        } else if (ast->son->token.token_number == OP_ADD || ast->son->token.token_number == OP_SUB || ast->son->token.token_number == OP_MUL || ast->son->token.token_number == OP_DIV || ast->son->token.token_number == OP_MOD || ast->son->token.token_number == OP_AND || ast->son->token.token_number == OP_OR || ast->son->token.token_number == OP_XOR || ast->son->token.token_number == OP_SHL || ast->son->token.token_number == OP_LSR || ast->son->token.token_number == OP_EQ || ast->son->token.token_number == OP_NE || ast->son->token.token_number == OP_LT || ast->son->token.token_number == OP_GT || ast->son->token.token_number == OP_LE || ast->son->token.token_number == OP_GE) {
            Node * n1 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);
            Node * n2 = tag_nt_instr_interpreting(ast->son->brother->brother, temp_in_rA, temp_in_rB);
            Node * n3 = tag_nt_instr_interpreting(ast->son, n1->token.token_value, n2->token.token_value);

            n1->brother = n2;
            n2->brother = n3;

            Node * n = node_maker(n1, NULL, TAG_LINE_SET, n3->token.token_value);

            return n;
        } else if (ast->son->token.token_number == NUM_INT) {
            Node * n1 = tag_nt_instr_interpreting(ast->son, temp_in_rA, temp_in_rB);

            return n1;
        } else if (ast->son->token.token_number == OP_LOGIC_AND) {
            int return_val_temp;

            Node * n1 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);           // e1 calc
            Node * n3 = tag_nt_instr_interpreting(ast->son->brother->brother, temp_in_rA, temp_in_rB);  // e2 calc

            return_val_temp = temp_count++;
            Node * n5 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("1"));   // true, return 1
            n5->token.token_value = n5->son->brother->token.token_value;
            Node * n8 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("0"));   // false, return 0
            n8->token.token_value = n8->son->brother->token.token_value;

            Node * n7 = line_maker(TAG_LABEL_MAKE, TAG_LABEL, label_count++, TAG_TEMP, 0, TAG_TEMP, 0);   // label making_false
            n7->token.token_value = n7->son->brother->token.token_value;
            Node * n9 = line_maker(TAG_LABEL_MAKE, TAG_LABEL, label_count++, TAG_TEMP, 0, TAG_TEMP, 0);   // label making_end
            n9->token.token_value = n9->son->brother->token.token_value;

            Node * n2 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_EQ, TAG_LABEL, n7->token.token_value);    // e1 branch, to false
            Node * n4 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_EQ, TAG_LABEL, n7->token.token_value);    // e2 branch, to false
            Node * n6 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_AL, TAG_LABEL, n9->token.token_value);    // true branch, to end

            n1->brother = n2;
            n2->brother = n3;
            n3->brother = n4;
            n4->brother = n5;
            n5->brother = n6;
            n6->brother = n7;
            n7->brother = n8;
            n8->brother = n9;

            Node * n = node_maker(n1, NULL, TAG_LINE_SET, return_val_temp);

            return n;
        } else if (ast->son->token.token_number == OP_LOGIC_OR) {
            int return_val_temp;

            Node * n1 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);           // e1 calc
            Node * n3 = tag_nt_instr_interpreting(ast->son->brother->brother, temp_in_rA, temp_in_rB);  // e2 calc

            return_val_temp = temp_count++;
            Node * n5 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("0"));   // false, return 0
            Node * n8 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("1"));   // true, return 1

            Node * n7 = line_maker(TAG_LABEL_MAKE, TAG_LABEL, label_count++, TAG_TEMP, 0, TAG_TEMP, 0);   // label making_true
            n7->token.token_value = n7->son->brother->token.token_value;
            Node * n9 = line_maker(TAG_LABEL_MAKE, TAG_LABEL, label_count++, TAG_TEMP, 0, TAG_TEMP, 0);   // label making_end
            n9->token.token_value = n9->son->brother->token.token_value;

            Node * n2 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_NE, TAG_LABEL, n7->token.token_value);    // e1 branch, to false
            Node * n4 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_NE, TAG_LABEL, n7->token.token_value);    // e2 branch, to false
            Node * n6 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_AL, TAG_LABEL, n9->token.token_value);    // true branch, to end

            n1->brother = n2;
            n2->brother = n3;
            n3->brother = n4;
            n4->brother = n5;
            n5->brother = n6;
            n6->brother = n7;
            n7->brother = n8;
            n8->brother = n9;

            Node * n = node_maker(n1, NULL, TAG_LINE_SET, return_val_temp);

            return n;
        } else {
            printf("EXP로 허용되지 않는 토큰 입력: <%d, %d>\n", ast->son->token.token_number, ast->son->token.token_value);
            return NULL;
        }//에러처리

    } else if (ast->token.token_number == NUM_INT) {
        printf("enter NUM_INT\n");
        Node * n = line_maker(TAG_MOV, TAG_TEMP, temp_count++, TAG_TEMP, 0, NUM_INT, ast->token.token_value);
                
        n->token.token_value = n->son->brother->token.token_value;

        printf("\tINSTR: %d temp.%d %d\n", n->son->token.token_number, n->son->brother->token.token_value, n->son->brother->brother->brother->token.token_value);

        return n;

    // 단항 연산자일 경우 (OP_TILDE, OP_NEG, OP_LOGIC_NOT)
    } else if (ast->token.token_number == OP_TILDE || ast->token.token_number == OP_NEG || ast->token.token_number == OP_LOGIC_NOT) {
        printf("enter OP_%d\n", ast->token.token_number);
        Node * n = line_maker(ast->token.token_number, TAG_TEMP, temp_count++, TAG_TEMP, temp_in_rA, TAG_TEMP, temp_in_rB);
        
        n->token.token_value = n->son->brother->token.token_value;

        return n;

    // 이항 연산자일 경우 (OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_AND, OP_OR, OP_XOR, OP_SHL, OP_LSR)
    } else if (ast->token.token_number == OP_ADD || ast->token.token_number == OP_SUB || ast->token.token_number == OP_MUL || ast->token.token_number == OP_DIV || ast->token.token_number == OP_MOD || ast->token.token_number == OP_AND || ast->token.token_number == OP_OR || ast->token.token_number == OP_XOR || ast->token.token_number == OP_SHL || ast->token.token_number == OP_LSR || ast->token.token_number == OP_EQ || ast->token.token_number == OP_NE || ast->token.token_number == OP_LT || ast->token.token_number == OP_GT || ast->token.token_number == OP_LE || ast->token.token_number == OP_GE) {
        printf("enter OP_%d\n", ast->token.token_number);
        Node * n = line_maker(ast->token.token_number, TAG_TEMP, temp_count++, TAG_TEMP, temp_in_rA, TAG_TEMP, temp_in_rB);
        
        n->token.token_value = n->son->brother->token.token_value;

        return n;

    } else {
        printf("AST to TAG 과정에서 오류 발생: Instruction node들을 처리해야 하지만, instr에 해당하지 않는 노드가 입력되었습니다. 입력 노드: %d\n", ast->token.token_number);
        exit(1);
    }
}

Node * tag_generator(Node * parse_input, char * tagtree_name)
{
    Node * tag_top;


    FILE *taggenfp = fopen(tagtree_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (taggenfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("TAG Generate 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", tagtree_name);
        exit(1);
    }


    tag_top = tag_nt_program(parse_input);
    bin_tree_printer(tag_top);
    bin_tree_file_printer(tag_top, taggenfp);

    printf("TAG Generating Finished.\n");
    fclose(taggenfp);
    return tag_top;
}
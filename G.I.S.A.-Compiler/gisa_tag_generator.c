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


int temp_count;     // 0은 처음 입력값이므로 1부터 임시변수 시작.
int label_count = 1;

Node * tag_terminal(Node * ast);
Node * tag_nt_program(Node * ast);
Node * tag_nt_function(Node * ast);
Node * tag_nt_scope(Node * ast);
Node * tag_nt_instr(Node * ast);
Node * tag_nt_instr_interpreting(Node * ast, int temp_in_rA, int temp_in_rB);
Node * line_op_logic_not(Node * ast, int temp_in_rA, int temp_in_rB);
Node * line_op_logic_and_or(Node * ast, int temp_in_rA, int temp_in_rB);
Node * line_op_comp(Node * ast, int temp_in_rA, int temp_in_rB);


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
        Node * x4 = tag_nt_scope(ast->son->brother->brother->brother);

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

Node * tag_nt_scope(Node * ast){
    if (ast->token.token_number == NT_SCOPE) {
        printf("Processing: tag_nt_scope\n");
        Node * x1 = tag_nt_instr(ast->son);

        Node * n = node_maker(x1, NULL, TAG_SCOPE, 0);

        if (ast->brother != NULL) {
            Node * n1 = tag_nt_scope(ast->brother);
            n->brother = n1;
        } else {
            n->brother = NULL;
        }

        return n;
    } else {
        printf("AST to TAG 과정에서 오류 발생: NT_SCOPE node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}

Node * tag_nt_instr(Node * ast){
    if (ast->token.token_number == NT_CONTENT) {
        printf("Processing: tag_nt_instr\n");
        Node * x1 = tag_nt_instr_interpreting(ast->son, 0, 0);

        Node * n = node_maker(x1, NULL, TAG_INSTR, 0);

        if (ast->brother->token.token_number != NT_INSTR_LIST) {    // while로 실수해 무한루프 나왔던 부분. if로 할 것.
            Node * n1 = tag_nt_instr(ast->brother);
            n->brother = n1;
            //printf("DEBUG: 다음 라인 탑 토큰: <%d, %d>\n", ast->brother->token.token_number, ast->brother->token.token_value);
            //printf("DEBUG: 다음 라인 탑-son 토큰: <%d, %d>\n", ast->brother->son->token.token_number, ast->brother->son->token.token_value);
        }

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
        if (ast->son->token.token_number == OP_TILDE || ast->son->token.token_number == OP_NEG) {
            Node * n1 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);
            Node * n2 = tag_nt_instr_interpreting(ast->son, 0, n1->token.token_value);

            n1->brother = n2;

            Node * n = node_maker(n1, NULL, TAG_LINE_SET, n2->token.token_value);

            return n;
        } else if (ast->son->token.token_number >= OP_ADD && ast->son->token.token_number <= OP_LSR) {
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
        } else if (ast->son->token.token_number == OP_LOGIC_NOT) {
            Node * n = line_op_logic_not(ast, temp_in_rA, temp_in_rB);

            return n;
        } else if (ast->son->token.token_number == OP_LOGIC_AND || ast->son->token.token_number == OP_LOGIC_OR) {
            Node * n = line_op_logic_and_or(ast, temp_in_rA, temp_in_rB);

            return n;
        } else if (ast->son->token.token_number >= OP_EQ && ast->son->token.token_number <= OP_GE) {
            Node * n = line_op_comp(ast, temp_in_rA, temp_in_rB);

            return n;
        } else if (ast->son->token.token_number == OP_ASSIGN) {
            Node * n1 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);   // symbol id를 리턴하도록 만들어야 함.
            Node * n2 = tag_nt_instr_interpreting(ast->son->brother->brother, temp_in_rA, temp_in_rB);      // rside
            Node * n3 = tag_nt_instr_interpreting(ast->son, n1->token.token_value, n2->token.token_value);  // mov. 인자 1이 lside, 2가 rside.

            n1->brother = n2;
            n2->brother = n3;

            Node * n = node_maker(n1, NULL, TAG_LINE_SET, n3->token.token_value);

            return n;
        } else {
            printf("EXP로 허용되지 않는 토큰 입력: <%d, %d>\n", ast->son->token.token_number, ast->son->token.token_value);
            return NULL;
        }//에러처리



    // *** 트리 밑바닥 ***
    } else if (ast->token.token_number == NUM_INT) {
        printf("enter NUM_INT\n");
        Node * n = line_maker(TAG_MOV, TAG_TEMP, temp_count++, TAG_TEMP, 0, NUM_INT, ast->token.token_value);
                
        n->token.token_value = n->son->brother->token.token_value;

        printf("\tINSTR: %d temp.%d %d\n", n->son->token.token_number, n->son->brother->token.token_value, n->son->brother->brother->brother->token.token_value);

        return n;

    // 단항 연산자일 경우 (OP_TILDE, OP_NEG)
    } else if (ast->token.token_number == OP_TILDE || ast->token.token_number == OP_NEG) {
        printf("enter OP_%d\n", ast->token.token_number);
        Node * n = line_maker(ast->token.token_number, TAG_TEMP, temp_count++, TAG_TEMP, 0, TAG_TEMP, temp_in_rB);
        
        n->token.token_value = n->son->brother->token.token_value;

        return n;

    // 이항 연산자일 경우 (OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_AND, OP_OR, OP_XOR, OP_SHL, OP_LSR)
    } else if (ast->token.token_number >= OP_ADD && ast->token.token_number <= OP_GE) {
        printf("enter OP_%d\n", ast->token.token_number);
        Node * n = line_maker(ast->token.token_number, TAG_TEMP, temp_count++, TAG_TEMP, temp_in_rA, TAG_TEMP, temp_in_rB);
        
        n->token.token_value = n->son->brother->token.token_value;

        return n;

    // ASSIGN 연산자일 경우
    } else if (ast->token.token_number == OP_ASSIGN) {
        printf("enter OP_ASSIGN\n");
        Node * n = line_maker(ast->token.token_number, TAG_TEMP, temp_in_rA, TAG_TEMP, 0, TAG_TEMP, temp_in_rB);
        
        n->token.token_value = n->son->brother->token.token_value;

        return n;

    // Symbol일 경우
    } else if (ast->token.token_number == SEM_SYMBOL) {
        printf("enter SEM_SYMBOL\n");
        Node * n = node_maker(NULL, NULL, TAG_NOP, ast->token.token_value);
                

        printf("\tSYMBOL CALL. SYMBOL ID is %d.\n", n->token.token_value);

        return n;

    } else {
        printf("AST to TAG 과정에서 오류 발생: Instruction node들을 처리해야 하지만, instr에 해당하지 않는 노드가 입력되었습니다. 입력 노드: %d\n", ast->token.token_number);
        exit(1);
    }
}



Node * line_op_logic_not(Node * ast, int temp_in_rA, int temp_in_rB)
{
    int return_val_temp;


    return_val_temp = temp_count++;
    Node * n1 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("1"));   // true, return 1
    Node * n2 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);           // e1 calc
    Node * n3 = line_maker(TAG_CMP, TAG_TEMP, 0, TAG_TEMP, n2->token.token_value, NUM_INT, lexval_manager ("0"));  // comparing, setcc
    Node * n5 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("0"));   // false, return 0
    Node * n6 = line_maker(TAG_LABEL_MAKE, TAG_LABEL, label_count++, TAG_TEMP, 0, TAG_TEMP, 0);   // label making: end
    n6->token.token_value = n6->son->brother->token.token_value;
            
    Node * n4 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_EQ, TAG_LABEL, n6->token.token_value);

    n1->brother = n2;
    n2->brother = n3;
    n3->brother = n4;
    n4->brother = n5;
    n5->brother = n6;

    Node * n = node_maker(n1, NULL, TAG_LINE_SET, return_val_temp);

    return n;
}

Node * line_op_logic_and_or(Node * ast, int temp_in_rA, int temp_in_rB)
{
    int return_val_temp;

    Node * n2 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);           // e1 calc
    Node * n5 = tag_nt_instr_interpreting(ast->son->brother->brother, temp_in_rA, temp_in_rB);  // e2 calc

    Node * n9 = line_maker(TAG_LABEL_MAKE, TAG_LABEL, label_count++, TAG_TEMP, 0, TAG_TEMP, 0);   // label making: end
    n9->token.token_value = n9->son->brother->token.token_value;

    return_val_temp = temp_count++;

    Node * n8;
    Node * n1;
    Node * n4;
    Node * n7;

    if (ast->son->token.token_number == OP_LOGIC_AND){
        n8 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("1"));   // true, return 1
        n1 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("0"));   // false, return 0
        n4 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_EQ, TAG_LABEL, n9->token.token_value);    // e1 branch, to end
        n7 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_EQ, TAG_LABEL, n9->token.token_value);    // e2 branch, to end
    } else {
        n8 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("0"));   // true, return 0
        n1 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("1"));   // false, return 1
        n4 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_NE, TAG_LABEL, n9->token.token_value);    // e1 branch, to end
        n7 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_NE, TAG_LABEL, n9->token.token_value);    // e2 branch, to end
    }
            
    n8->token.token_value = n8->son->brother->token.token_value;
    n1->token.token_value = n1->son->brother->token.token_value;
            

    Node * n3 = line_maker(TAG_CMP, TAG_TEMP, 0, TAG_TEMP, n2->token.token_value, NUM_INT, lexval_manager ("0"));  // comparing, setcc
    Node * n6 = line_maker(TAG_CMP, TAG_TEMP, 0, TAG_TEMP, n5->token.token_value, NUM_INT, lexval_manager ("0"));  // comparing, setcc
            

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
}

Node * line_op_comp(Node * ast, int temp_in_rA, int temp_in_rB)
{
    int return_val_temp;


    return_val_temp = temp_count++;
    Node * n1 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("1"));   // true, return 1
    Node * n2 = tag_nt_instr_interpreting(ast->son->brother, temp_in_rA, temp_in_rB);           // e1 calc
    Node * n3 = tag_nt_instr_interpreting(ast->son->brother->brother, temp_in_rA, temp_in_rB);  // e2 calc
    Node * n4 = line_maker(TAG_CMP, TAG_TEMP, 0, TAG_TEMP, n2->token.token_value, TAG_TEMP, n3->token.token_value);  // comparing, setcc
            
    Node * n6 = line_maker(TAG_MOV, TAG_TEMP, return_val_temp, TAG_TEMP, 0, NUM_INT, lexval_manager ("0"));   // false, return 0
    Node * n7 = line_maker(TAG_LABEL_MAKE, TAG_LABEL, label_count++, TAG_TEMP, 0, TAG_TEMP, 0);   // label making: end
    n7->token.token_value = n7->son->brother->token.token_value;

    Node * n5;

    switch (ast->son->token.token_number) {    // if hit, branch to end
        case (OP_EQ):
            n5 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_EQ, TAG_LABEL, n7->token.token_value);
            break;
                    
        case (OP_NE):
            n5 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_NE, TAG_LABEL, n7->token.token_value);
            break;
                    
        case (OP_LT):
            n5 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_LT, TAG_LABEL, n7->token.token_value);
            break;
                    
        case (OP_GT):
            n5 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_GT, TAG_LABEL, n7->token.token_value);
            break;
                    
        case (OP_LE):
            n5 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_LE, TAG_LABEL, n7->token.token_value);
            break;
                    
        case (OP_GE):
            n5 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_GE, TAG_LABEL, n7->token.token_value);
            break;

        default:
            n5 = line_maker(TAG_BRANCH, TAG_TEMP, 0, TAG_COND, COND_AL, TAG_LABEL, n7->token.token_value);
            break;
                        
    }
            

    n1->brother = n2;
    n2->brother = n3;
    n3->brother = n4;
    n4->brother = n5;
    n5->brother = n6;
    n6->brother = n7;

    Node * n = node_maker(n1, NULL, TAG_LINE_SET, return_val_temp);

    return n;
}










/* *********************************************
************************************************
************************************************
***********                          ***********
***********    MAIN TAG GENERATOR    ***********
***********                          ***********
************************************************
************************************************
********************************************* */

Node * tag_generator(Node * parse_input, char * tagtree_name)
{
    temp_count = symbol_id_count + 1;
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
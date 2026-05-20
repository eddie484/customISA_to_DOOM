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
    [[KW_RETURN (exp)]]

    TAG
    t1 = [[exp]]
    KW_RETURN t1

*/

#include "gisa_compiler.h"


int temp_count = 1;     // 0은 처음 입력값이므로 1부터 임시변수 시작.

Node * tag_terminal(Node * ast);
Node * tag_nt_program(Node * ast);
Node * tag_nt_function(Node * ast);
Node * tag_nt_instr(Node * ast);
Node * tag_nt_instr_interpreting(Node * ast, int temp_in);


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
        Node * x1 = tag_nt_instr_interpreting(ast->son, 0);

        Node * n = node_maker(x1, NULL, TAG_INSTR, 0);

        return n;
    } else {
        printf("AST to TAG 과정에서 오류 발생: NT_CONTENT node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}


Node * tag_nt_instr_interpreting(Node * ast, int temp_in){
    if (ast->token.token_number == KW_RETURN) {
        printf("enter KW_RETURN\n");
        Node * n1 = tag_nt_instr_interpreting(ast->brother, temp_in);
        Node * n2 = line_maker(KW_RETURN, TAG_TEMP, 0, TAG_TEMP, 0, TAG_TEMP, n1->token.token_value);

        n2->token.token_value = n2->son->brother->token.token_value;
        n1->brother = n2;

        Node * n = node_maker(n1, NULL, TAG_LINE_SET, n2->token.token_value);

        return n;

    } else if (ast->token.token_number == NT_EXP) {
        printf("enter NT_EXP\n");
        if (ast->son->token.token_number == OP_TILDE || ast->son->token.token_number == OP_MINUS) {
            Node * n1 = tag_nt_instr_interpreting(ast->son->brother, temp_in);
            Node * n2 = tag_nt_instr_interpreting(ast->son, n1->token.token_value);

            n1->brother = n2;

            Node * n = node_maker(n1, NULL, TAG_LINE_SET, n2->token.token_value);

            return n;
        } else if (ast->son->token.token_number == NUM_INT) {
            Node * n1 = tag_nt_instr_interpreting(ast->son, temp_in);

            return n1;
        } else {return NULL;}//에러처리

    } else if (ast->token.token_number == NUM_INT) {
        printf("enter NUM_INT\n");
        Node * n = line_maker(TAG_MOV, TAG_TEMP, temp_count++, TAG_TEMP, 0, NUM_INT, ast->token.token_value);
                
        n->token.token_value = n->son->brother->token.token_value;

        printf("\tINSTR: %d temp.%d %d\n", n->son->token.token_number, n->son->brother->token.token_value, n->son->brother->brother->brother->token.token_value);

        return n;

    } else if (ast->token.token_number == OP_TILDE) {
        printf("enter OP_TILDE\n");
        Node * n = line_maker(OP_TILDE, TAG_TEMP, temp_count++, TAG_TEMP, 0, TAG_TEMP, temp_in);
        
        n->token.token_value = n->son->brother->token.token_value;

        return n;

    } else if (ast->token.token_number == OP_MINUS) {
        printf("enter OP_MINUS\n");
        Node * n = line_maker(OP_MINUS, TAG_TEMP, temp_count++, TAG_TEMP, 0, TAG_TEMP, temp_in);
        
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
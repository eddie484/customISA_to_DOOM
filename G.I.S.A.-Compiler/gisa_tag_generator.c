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


Node line[100];
int line_count = 0;
int temp_count = 1;     // 0은 처음 입력값이므로 1부터 임시변수 시작.

Node * tag_terminal(Node * ast);
Node * tag_nt_program(Node * ast);
Node * tag_nt_function(Node * ast);
Node * tag_nt_instr(Node * ast);
Node * tag_nt_instr_interpreting(Node * ast, int temp_in);


Node * tag_terminal(Node * ast){
    if (ast->token.token_number >= 0 && ast->token.token_number <= 9) {
        printf("Processing: terminal <%d, %d>\n", ast->token.token_number, ast->token.token_value);
        Node * n = malloc(sizeof(Node));
        n->token = ast->token;

        n->son = NULL;
        n->brother = NULL;

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

        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = TAG_PROGRAM;

        n->brother = NULL;
        n->token.token_value = 0;

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

        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = TAG_FUNCTION;

        n->brother = NULL;
        n->token.token_value = 0;

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

        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = TAG_INSTR;

        n->brother = NULL;
        n->token.token_value = 0;

        return n;
    } else {
        printf("AST to TAG 과정에서 오류 발생: NT_CONTENT node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}


Node * tag_nt_instr_interpreting(Node * ast, int temp_in){
    printf("line count: %d\n", line_count);
    if (ast->token.token_number == KW_RETURN) {
        printf("enter KW_RETURN\n");
        Node * n1 = tag_nt_instr_interpreting(ast->brother, temp_in);
        
            Node * n2 = malloc(sizeof(Node));
            Node * x1 = malloc(sizeof(Node));
            Node * x2 = malloc(sizeof(Node));
            Node * x3 = malloc(sizeof(Node));
            Node * x4 = malloc(sizeof(Node));

            n2->son = x1;        // line start
            n2->brother = NULL;
            n2->token.token_number = TAG_LINE;
            n2->token.token_value = 0;

            x1->son = NULL;     // 연산자
            x1->brother = x2;
            x1->token.token_number = KW_RETURN;
            x1->token.token_value = 0;

            x2->son = NULL;     // rD
            x2->brother = x3;
            x2->token.token_number = TAG_TEMP;
            x2->token.token_value = 0;

            x3->son = NULL;     // rA
            x3->brother = x4;
            x3->token.token_number = TAG_TEMP;
            x3->token.token_value = 0;
            
            n2->token.token_value = x2->token.token_value;

            x4->son = NULL;     // rB
            x4->brother = NULL;
            x4->token.token_number = TAG_TEMP;
            x4->token.token_value = n1->token.token_value;
            
            n2->token.token_value = x2->token.token_value;

        n1->brother = n2;

        Node * n = malloc(sizeof(Node));
        n->son = n1;
        n->brother = NULL;
        n->token.token_number = TAG_LINE_SET;
        n->token.token_value = n2->token.token_value;

        return n;

    } else if (ast->token.token_number == NT_EXP) {
        printf("enter NT_EXP\n");
        if (ast->son->token.token_number == OP_TILDE || ast->son->token.token_number == OP_MINUS) {
            Node * n1 = tag_nt_instr_interpreting(ast->son->brother, temp_in);
            Node * n2 = tag_nt_instr_interpreting(ast->son, n1->token.token_value);

            n1->brother = n2;

            Node * n = malloc(sizeof(Node));
            n->son = n1;
            n->brother = NULL;
            n->token.token_number = TAG_LINE_SET;
            n->token.token_value = n2->token.token_value;

            return n;
        } else if (ast->son->token.token_number == NUM_INT) {
            Node * n1 = tag_nt_instr_interpreting(ast->son, temp_in);

            return n1;
        } else {return NULL;}//에러처리

    } else if (ast->token.token_number == NUM_INT) {
        printf("enter NUM_INT\n");
        Node * n = malloc(sizeof(Node));
        Node * x1 = malloc(sizeof(Node));
        Node * x2 = malloc(sizeof(Node));
        Node * x3 = malloc(sizeof(Node));
        Node * x4 = malloc(sizeof(Node));

        n->son = x1;        // line start
        n->brother = NULL;
        n->token.token_number = TAG_LINE;
        n->token.token_value = 0;

        x1->son = NULL;     // 연산자
        x1->brother = x2;
        x1->token.token_number = TAG_MOV;
        x1->token.token_value = 0;

        x2->son = NULL;     // rD
        x2->brother = x3;
        x2->token.token_number = TAG_TEMP;
        x2->token.token_value = temp_count++;

        x3->son = NULL;     // rA
        x3->brother = x4;
        x3->token.token_number = TAG_TEMP;
        x3->token.token_value = 0;

        x4->son = NULL;     // rB
        x4->brother = NULL;
        x4->token.token_number = NUM_INT;
        x4->token.token_value = ast->token.token_value;
        
        n->token.token_value = x2->token.token_value;

        printf("\tINSTR: %d temp.%d %d\n", x1->token.token_number, x2->token.token_value, x4->token.token_value);

        return n;

    } else if (ast->token.token_number == OP_TILDE) {
        printf("enter OP_TILDE\n");
        Node * n = malloc(sizeof(Node));
        Node * x1 = malloc(sizeof(Node));
        Node * x2 = malloc(sizeof(Node));
        Node * x3 = malloc(sizeof(Node));
        Node * x4 = malloc(sizeof(Node));

        n->son = x1;        // line start
        n->brother = NULL;
        n->token.token_number = TAG_LINE;
        n->token.token_value = 0;

        x1->son = NULL;     // 연산자
        x1->brother = x2;
        x1->token.token_number = OP_TILDE;
        x1->token.token_value = 0;

        x2->son = NULL;     // rD
        x2->brother = x3;
        x2->token.token_number = TAG_TEMP;
        x2->token.token_value = temp_count++;

        x3->son = NULL;     // rA
        x3->brother = x4;
        x3->token.token_number = TAG_TEMP;
        x3->token.token_value = 0;

        x4->son = NULL;     // rB
        x4->brother = NULL;
        x4->token.token_number = TAG_TEMP;
        x4->token.token_value = temp_in;
        
        n->token.token_value = x2->token.token_value;

        return n;

    } else if (ast->token.token_number == OP_MINUS) {
        printf("enter OP_MINUS\n");
        Node * n = malloc(sizeof(Node));
        Node * x1 = malloc(sizeof(Node));
        Node * x2 = malloc(sizeof(Node));
        Node * x3 = malloc(sizeof(Node));
        Node * x4 = malloc(sizeof(Node));

        n->son = x1;        // line start
        n->brother = NULL;
        n->token.token_number = TAG_LINE;
        n->token.token_value = 0;

        x1->son = NULL;     // 연산자
        x1->brother = x2;
        x1->token.token_number = OP_MINUS;
        x1->token.token_value = 0;

        x2->son = NULL;     // rD
        x2->brother = x3;
        x2->token.token_number = TAG_TEMP;
        x2->token.token_value = temp_count++;

        x3->son = NULL;     // rA
        x3->brother = x4;
        x3->token.token_number = NUM_INT;
        x3->token.token_value = 0;

        x4->son = NULL;     // rB
        x4->brother = NULL;
        x4->token.token_number = TAG_TEMP;
        x4->token.token_value = temp_in;
        
        n->token.token_value = x2->token.token_value;

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
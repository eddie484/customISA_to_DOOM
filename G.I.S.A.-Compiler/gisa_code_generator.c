#include "gisa_compiler.h"


Node * asm_terminal(Node * ast);
Node * asm_nt_program(Node * ast);
Node * asm_nt_function(Node * ast);
Node * asm_nt_instr(Node * ast);
Node * asm_nt_instr_loop(Node * ast);
Node * asm_nt_mov(Node * ast);


Node * asm_terminal(Node * ast){
    if (ast->token.token_number >= 0 && ast->token.token_number <= 9) {
        printf("Processing: terminal <%d, %d>\n", ast->token.token_number, ast->token.token_value);
        Node * n = malloc(sizeof(Node));
        n->token = ast->token;

        n->son = NULL;
        n->brother = NULL;

        return n;
    } else {
        printf("AST to asm tree 과정에서 오류 발생: terminal node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}

Node * asm_nt_program(Node * ast){
    if (ast->token.token_number == NT_PROGRAM) {
        printf("Processing: asm_nt_program\n");
        Node * x1 = asm_nt_function(ast->son);

        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = ASM_PROGRAM;

        n->brother = NULL;
        n->token.token_value = 0;

        return n;
    } else {
        printf("AST to asm tree 과정에서 오류 발생: NT_PROGRAM node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}

Node * asm_nt_function(Node * ast){
    if (ast->token.token_number == NT_FUNCTION) {
        printf("Processing: asm_nt_function\n");
        Node * x1 = asm_terminal(ast->son);
        Node * x2 = asm_terminal(ast->son->brother);
        Node * x3 = asm_terminal(ast->son->brother->brother);
        Node * x4 = asm_nt_instr(ast->son->brother->brother->brother);

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
        printf("AST to asm tree 과정에서 오류 발생: NT_FUNCTION node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}

Node * asm_nt_instr(Node * ast){
    if (ast->token.token_number == NT_CONTENT) {
        printf("Processing: asm_nt_instr\n");
        Node * x1 = asm_nt_instr_loop(ast->son);

        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = ASM_INSTR;

        n->brother = NULL;
        n->token.token_value = 0;

        return n;
    } else {
        printf("AST to asm tree 과정에서 오류 발생: NT_CONTENT node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}

Node * asm_nt_instr_loop(Node * ast){
    printf("Processing: asm_nt_instr_loop\n");
    Node * n;

        if (ast->token.token_number == KW_RETURN){
            n = asm_nt_mov(ast);
            Node * n1 = malloc(sizeof(Node));
            n1->token.token_number = ASM_RET;
            n->brother = n1;

            n1->son = NULL;
            n1->brother = NULL;
            n1->token.token_value = 0;

            if (ast->brother->brother != NULL) {
                Node * x1 = asm_nt_instr_loop(ast->brother->brother);
                n->brother->brother = x1;
            }
        } else {
            printf("AST to asm tree 과정에서 오류 발생: Instruction node들을 처리해야 하지만, instr에 해당하지 않는 노드가 입력되었습니다. 입력 노드: %d\n", ast->token.token_number);
            exit(1);
        }
    
    return n;
}

Node * asm_nt_mov(Node * ast){
    if (ast->token.token_number == KW_RETURN) {
        printf("Processing: asm_nt_mov\n");
        Node * x1 = malloc(sizeof(Node));
        Node * x2 = malloc(sizeof(Node));

        x1->token = ast->brother->token;
        x2->token.token_number = ASM_REGISTER;
        x2->token.token_value = 0;

        x1->brother = x2;


        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = ASM_MOV;

        x1->son = NULL;
        x2->son = NULL;
        x2->brother = NULL;

        n->brother = NULL;
        n->token.token_value = 0;

        return n;
    } else {
        printf("AST to asm tree 과정에서 오류 발생: KW_RETURN node를 처리해야 하지만, %d 노드가 입력되었습니다.\n", ast->token.token_number);
        exit(1);
    }
}



Node * code_generator(Node * parse_input, char * codegentree_name)
{
    Node * asm_top = malloc(sizeof(Node));


    FILE *codegenfp = fopen(codegentree_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (codegenfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Parse 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", codegentree_name);
        exit(1);
    }


    asm_top = asm_nt_program(parse_input);
    bin_tree_printer(asm_top);
    bin_tree_file_printer(asm_top, codegenfp);

    printf("Code Generating Finished.\n");
    fclose(codegenfp);
    return asm_top;
}
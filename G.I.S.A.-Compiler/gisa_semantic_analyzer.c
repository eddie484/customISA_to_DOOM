#include "gisa_compiler.h"



int symbol_id_count = 0;

int table_list_limit;
int table_list_count;
int table_stack_limit;
int table_stack_count;

int * table_limit;
int * table_count;

Symbol_info *** table_list;
Symbol_info *** table_stack;

void push() {
    printf("Pushing Start. Current Table stack count: %d\n", table_stack_count);
    table_limit[table_stack_count] = 8;
    table_count[table_stack_count] = 0;
    Symbol_info ** symbol_table = malloc(sizeof(Symbol_info*) * table_limit[table_stack_count]);
    table_stack[table_stack_count++] = symbol_table;
    
    if (table_stack_count == table_stack_limit) {
        table_stack_limit = table_stack_limit * 2;
        table_stack = realloc(table_stack, sizeof(Symbol_info**) * table_stack_limit);
        memset((char*)table_stack + (table_stack_limit / 2) * sizeof(Symbol_info**), 0, (table_stack_limit / 2) * sizeof(Symbol_info**));
        
        table_limit = realloc(table_limit, sizeof(int) * table_stack_limit);
        table_count = realloc(table_count, sizeof(int) * table_stack_limit);
    }

    printf("Pushing Finish. Current Table stack count: %d\n", table_stack_count);
}

void pop() {
    printf("Popping\n");
    table_list[table_list_count++] = table_stack[--table_stack_count];
    table_stack[table_stack_count] = NULL;
    
    if (table_list_count == table_list_limit) {
        table_list_limit = table_list_limit * 2;
        table_list = realloc(table_list, sizeof(Symbol_info*) * table_list_limit);
    }
    
    if ((table_stack_count <= table_stack_limit / 4) && (table_stack_count > 0)) {
        table_stack_limit = table_stack_limit / 2;
        table_stack = realloc(table_stack, sizeof(Symbol_info*) * table_stack_limit);
        
        table_limit = realloc(table_limit, sizeof(int) * table_stack_limit);
        table_count = realloc(table_count, sizeof(int) * table_stack_limit);
    }
}


int symbol_maker(Node * declr_node) {
    Node * ident_node = declr_node->son;

    while (ident_node->token.token_number != IDENT) {
        ident_node = get_brother(ident_node);
    }

    printf("Symbol Making IDENT %d\n", ident_node->token.token_value);

    //ident_node->토큰밸류 와 같은 심볼네임이 있는지 현재 테이블 순회해 검토. 있다면 오류 발생.
    for (int j = 0; j <= table_count[table_stack_count - 1] - 1; j++) {        // 테이블 내부 순회
        printf("DEBUG. j: %d\n", j);
        if (table_stack[table_stack_count - 1][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
            printf("DEBUG. Table[%d] is NULL. Break.\n", j);
            break;
        } else if (ident_node->token.token_value == table_stack[table_stack_count - 1][j]->name) {
            printf("오류: 이미 선언된 Symbol Name <%d, %d>입니다. 종료합니다.\n", ident_node->token.token_number, ident_node->token.token_value);
            exit(1);
        }
    }

    printf("정상: 이전에 선언되지 않은 Symbol Name <%d, %d>입니다. 계속 진행합니다.\n", ident_node->token.token_number, ident_node->token.token_value);

    Symbol_info * symbol = malloc(sizeof(Symbol_info));
    symbol->name = ident_node->token.token_value;
    symbol->id = symbol_id_count++;
    symbol->type_tree = node_maker(NULL, NULL, NUM_INT, 0);    // 이후 확장할 것. 형식도 enum으로 개선하고...
    symbol->size = 4;               // 위의 while문에서 typetree 만드는 함수도 만들어 호출하면 좋을것 같음.
    symbol->location.type = 0;
    symbol->location.location = 0;

    table_stack[table_stack_count - 1][table_count[table_stack_count - 1]++] = symbol;

    if (table_count[table_stack_count - 1] == table_limit[table_stack_count - 1]) {
        table_limit[table_stack_count - 1] = table_limit[table_stack_count - 1] * 2;
        table_stack[table_stack_count - 1] = realloc(table_stack[table_stack_count - 1], sizeof(Symbol_info*) * table_limit[table_stack_count - 1]);
    }

    printf("\n***** Symbol Making End. Made Symbol's info: *****\n");
    printf("\tName: %d\n", table_stack[table_stack_count - 1][table_count[table_stack_count - 1] - 1]->name);
    printf("\tID: %d\n", table_stack[table_stack_count - 1][table_count[table_stack_count - 1] - 1]->id);
    printf("\tType Tree's Top: %d\n", table_stack[table_stack_count - 1][table_count[table_stack_count - 1] - 1]->type_tree->token.token_number);
    printf("\tSize: %d\n", table_stack[table_stack_count - 1][table_count[table_stack_count - 1] - 1]->size);
    printf("\tLocation.Type: %d\n", table_stack[table_stack_count - 1][table_count[table_stack_count - 1] - 1]->location.type);
    printf("\tLocation.Location: %d\n\n\n", table_stack[table_stack_count - 1][table_count[table_stack_count - 1] - 1]->location.location);
    return table_stack[table_stack_count - 1][table_count[table_stack_count - 1] - 1]->id;
}

int symbol_finder(int ident_token_value) {
    for (int i = table_stack_count - 1; i >= 0; i--) {      // 테이블 스택 순회
        for (int j = 0; j <= table_count[i] - 1; j++) {        // 테이블 내부 순회
            if (table_stack[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                printf("DEBUG: table_stack[%d][%d] is NULL\n", i, j);
                break;
            } else if (ident_token_value == table_stack[i][j]->name) {
                printf("DEBUG: %d는 table_stack[%d][%d]에 정상적으로 선언된 심볼입니다.\n", ident_token_value, i, j);
                return table_stack[i][j]->id;
            }

            printf("DEBUG: table_stack[%d][%d]'s name is %d\n", i, j, table_stack[i][j]->name);
        }
    }

    printf("오류: 선언되지 않은 Symbol Name %d을 사용하려 합니다. 종료합니다.\n", ident_token_value);
    exit(1);    // 미선언 변수 사용 시도한 경우.
}







void ident_symbolizer(Node * node) {
    printf("Ident_Symbolizer Start: Node <%d, %d>\n", node->token.token_number, node->token.token_value);

    // *** IN ***
    if (node->token.token_number == NT_SCOPE) {
        push();
    } 
    
    else if (node->token.token_number == NT_DECLR) {    // NT_DECLR 구조: <DECLR>->
        int symbol_id = symbol_maker(node);
        Node * ident_node = node->son;

        while (ident_node->token.token_number != IDENT) {
            ident_node = get_brother(ident_node);
        }

        Node * exp;
        Node * assign;
        Node * symbol;
        Node * right_val;
        Node * type_cleaner;
        
        right_val = ident_node->brother->son->brother;
        symbol = node_maker(NULL, right_val, SEM_SYMBOL, symbol_id);
        assign = node_maker(NULL, symbol, OP_ASSIGN, 0);
        exp = node_maker(assign, NULL, NT_EXP, 0);
        // declr_to_content = node_maker(exp, node->brother, NT_CONTENT, 0);

        free(ident_node->brother->son);
        free(ident_node->brother);

        type_cleaner = node->son;
        while (type_cleaner != ident_node) {
            Node * n = type_cleaner->brother;
            free(type_cleaner);
            type_cleaner = n;
        }
        // node->son부터 ident_node 이전까지 free 하는 반복문
        free(ident_node);

        node->son = exp;
        node->token.token_number = NT_CONTENT;

        bin_tree_printer(node);

    } 
    
    else if (node->token.token_number == IDENT) {
        int symbol_id = symbol_finder(node->token.token_value);

        node->token.token_number = SEM_SYMBOL;
        node->token.token_value = symbol_id;
    }




    // *** CALL ***
    if (node->son != NULL) {
        ident_symbolizer(node->son);
    }
    
    if (node->brother != NULL) {
        ident_symbolizer(node->brother);
    }




    // *** OUT ***
    if (node->token.token_number == NT_SCOPE) {
        pop();
    }
    
    else if (node->token.token_number == NT_EXP) {
        if (node->son->token.token_number == OP_ASSIGN && node->son->brother->token.token_number != SEM_SYMBOL) {
            printf("ERROR: Symbol이 아닌 토큰 <%d, %d>에 값 대입중. 종료합니다.\n", node->son->brother->token.token_number, node->son->brother->token.token_value);
            exit(1);   // Symbol이 아닌 토큰에 값 대입중. 잘못된 표현식이므로 오류.
        }
    }

    printf("Ident_Symbolizer END: Node <%d, %d>\n", node->token.token_number, node->token.token_value);
}

// assign일 때 왼쪽이 Symbol인지 확인해야함. 틀리면 오류.
// DECLR INT ident ASSIGN 덩어리(107~33 라인)는 exp assign 형식(109->33)으로 바꿔줄 것.
// declr 노드들의 정보는 심볼테이블에 저장되었으니 필요없다.


Node * semantic_analyzer(Node * parse_input, char * symbast_name)
{
    table_list_limit = 8;
    table_list_count = 0;
    table_stack_limit = 4;
    table_stack_count = 0;
    table_list = calloc(table_list_limit, sizeof(Symbol_info**));
    table_stack = calloc(table_stack_limit, sizeof(Symbol_info**));
    table_limit = malloc(sizeof(int) * table_stack_limit);
    table_count = malloc(sizeof(int) * table_stack_limit);
    
    Node * symbolized_ast;


    FILE *symbastfp = fopen(symbast_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (symbastfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Semantic analyze 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", symbast_name);
        exit(1);
    }


    symbolized_ast = parse_input;
    ident_symbolizer(symbolized_ast->son->son->brother->brother->brother);
    
    bin_tree_printer(symbolized_ast);
    bin_tree_file_printer(symbolized_ast, symbastfp);

    printf("Semantic analyzing Finished.\n");
    fclose(symbastfp);
    return symbolized_ast;
}
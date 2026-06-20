#include "gisa_compiler.h"







/* *********************************************
************************************************
************************************************
***********                          ***********
***********    SYMBOL TABLE STACK    ***********
***********                          ***********
************************************************
************************************************
********************************************* */

int symbol_id_count = 1;

int symbol_table_list_limit;
int symbol_table_list_count;
int symbol_table_stack_limit;
int symbol_table_stack_count;

int * symbol_table_limit;
int * symbol_table_count;

Symbol_info *** symbol_table_list;
Symbol_info *** symbol_table_stack;

void push() {
    printf("Pushing Start. Current Table stack count: %d\n", symbol_table_stack_count);
    symbol_table_limit[symbol_table_stack_count] = 8;
    symbol_table_count[symbol_table_stack_count] = 0;
    Symbol_info ** symbol_table = malloc(sizeof(Symbol_info*) * symbol_table_limit[symbol_table_stack_count]);
    symbol_table_stack[symbol_table_stack_count++] = symbol_table;
    
    if (symbol_table_stack_count == symbol_table_stack_limit) {
        symbol_table_stack_limit = symbol_table_stack_limit * 2;
        symbol_table_stack = realloc(symbol_table_stack, sizeof(Symbol_info**) * symbol_table_stack_limit);
        memset((char*)symbol_table_stack + (symbol_table_stack_limit / 2) * sizeof(Symbol_info**), 0, (symbol_table_stack_limit / 2) * sizeof(Symbol_info**));
        
        symbol_table_limit = realloc(symbol_table_limit, sizeof(int) * symbol_table_stack_limit);
        symbol_table_count = realloc(symbol_table_count, sizeof(int) * symbol_table_stack_limit);
    }

    printf("Pushing Finish. Current Table stack count: %d\n", symbol_table_stack_count);
}

void pop() {
    printf("Popping\n");
    symbol_table_list[symbol_table_list_count++] = symbol_table_stack[--symbol_table_stack_count];
    symbol_table_stack[symbol_table_stack_count] = NULL;
    
    if (symbol_table_list_count == symbol_table_list_limit) {
        symbol_table_list_limit = symbol_table_list_limit * 2;
        symbol_table_list = realloc(symbol_table_list, sizeof(Symbol_info*) * symbol_table_list_limit);
    }
    
    if ((symbol_table_stack_count <= symbol_table_stack_limit / 4) && (symbol_table_stack_count > 0)) {
        symbol_table_stack_limit = symbol_table_stack_limit / 2;
        symbol_table_stack = realloc(symbol_table_stack, sizeof(Symbol_info*) * symbol_table_stack_limit);
        
        symbol_table_limit = realloc(symbol_table_limit, sizeof(int) * symbol_table_stack_limit);
        symbol_table_count = realloc(symbol_table_count, sizeof(int) * symbol_table_stack_limit);
    }
}


int symbol_maker(Node * declr_node) {
    Node * ident_node = declr_node->son;

    while (ident_node->token.token_number != IDENT) {
        ident_node = get_brother(ident_node);
    }

    printf("Symbol Making IDENT %d\n", ident_node->token.token_value);

    //ident_node->토큰밸류 와 같은 심볼네임이 있는지 현재 테이블 순회해 검토. 있다면 오류 발생.
    for (int j = 0; j <= symbol_table_count[symbol_table_stack_count - 1] - 1; j++) {        // 테이블 내부 순회
        printf("DEBUG. j: %d\n", j);
        if (symbol_table_stack[symbol_table_stack_count - 1][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
            printf("DEBUG. Table[%d] is NULL. Break.\n", j);
            break;
        } else if (ident_node->token.token_value == symbol_table_stack[symbol_table_stack_count - 1][j]->name) {
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

    symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1]++] = symbol;

    if (symbol_table_count[symbol_table_stack_count - 1] == symbol_table_limit[symbol_table_stack_count - 1]) {
        symbol_table_limit[symbol_table_stack_count - 1] = symbol_table_limit[symbol_table_stack_count - 1] * 2;
        symbol_table_stack[symbol_table_stack_count - 1] = realloc(symbol_table_stack[symbol_table_stack_count - 1], sizeof(Symbol_info*) * symbol_table_limit[symbol_table_stack_count - 1]);
    }

    printf("\n***** Symbol Making End. Made Symbol's info: *****\n");
    printf("\tName: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->name);
    printf("\tID: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id);
    printf("\tType Tree's Top: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->type_tree->token.token_number);
    printf("\tSize: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->size);
    printf("\tLocation.Type: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->location.type);
    printf("\tLocation.Location: %d\n\n\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->location.location);

    return symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id;
}

int symbol_finder(int ident_token_value) {
    for (int i = symbol_table_stack_count - 1; i >= 0; i--) {      // 테이블 스택 순회
        for (int j = 0; j <= symbol_table_count[i] - 1; j++) {        // 테이블 내부 순회
            if (symbol_table_stack[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                printf("DEBUG: symbol_table_stack[%d][%d] is NULL\n", i, j);
                break;
            } else if (ident_token_value == symbol_table_stack[i][j]->name) {
                printf("DEBUG: %d는 symbol_table_stack[%d][%d]에 정상적으로 선언된 심볼입니다.\n", ident_token_value, i, j);
                return symbol_table_stack[i][j]->id;
            }

            printf("DEBUG: symbol_table_stack[%d][%d]'s name is %d\n", i, j, symbol_table_stack[i][j]->name);
        }
    }

    printf("오류: 선언되지 않은 Symbol Name %d을 사용하려 합니다. 종료합니다.\n", ident_token_value);
    exit(1);    // 미선언 변수 사용 시도한 경우.
}











/* *******************************************
**********************************************
**********************************************
***********                        ***********
***********    IDENT SYMBOLIZER    ***********
***********                        ***********
**********************************************
**********************************************
******************************************* */

void ident_symbolizer(Node * node) {
    printf("Ident_Symbolizer Start: Node <%d, %d>\n", node->token.token_number, node->token.token_value);

    // *** IN ***
    if (node->token.token_number == NT_BLOCK) {
        push();

    } else if (node->token.token_number == NT_DECLR) {    // NT_DECLR 구조: <DECLR>->
        int symbol_id = symbol_maker(node);
        Node * ident_node = node->son;

        while (ident_node->token.token_number != IDENT) {
            ident_node = get_brother(ident_node);
        }

        if (ident_node->brother->son != NULL) {    // 변수 선언 후 초기화하는 경우.
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
        } else {        // 변수 선언만 하는 경우. 심볼 등록만 하고 라인을 삭제한다.
            Node * type_cleaner;
            
            type_cleaner = node->son;
            while (type_cleaner != ident_node) {
                Node * n = type_cleaner->brother;
                free(type_cleaner);
                type_cleaner = n;
            }

            free(ident_node->brother);
            free(ident_node);

            if (node->brother != NULL) {
                Node * node_brother = node->brother;
            node->son = node_brother->son;
            node->brother = node_brother->brother;
            node->token = node_brother->token;
            free (node_brother);        // brother를 node 자리에 당겨왔으므로 다시 함수호출해 brother였던 노드를 처리한다.
                                        // 윗줄의 함수호출 수행을 통해, 노드의 son과 brother가 전부 처리 완료된 후 이 라인으로 돌아온다. (parent까지 순회하며 처리하지는 않는다.)
            ident_symbolizer(node);     // 따라서 여기서 리턴하여 son과 brother의 함수호출을 다시 중복 수행하지 않도록 한다.
            } else {
                node->son = NULL;
                node->brother = NULL;
                node->token.token_number = PN_SEMI;
                node->token.token_value = 0;
            }

            return;
        }
            

    } else if (node->token.token_number == IDENT) {
        int symbol_id = symbol_finder(node->token.token_value);

        node->token.token_number = SEM_SYMBOL;
        node->token.token_value = symbol_id;
        
    } else if (node->token.token_number == OP_PRE_INCRE || node->token.token_number == OP_PRE_DECRE) {
        if (node->brother->token.token_number != IDENT && node->brother->token.token_number != SEM_SYMBOL) {
            printf("오류: OP_PRE_INCRE/OP_PRE_DECRE 연산자의 인자로 변수가 아닌 노드 <%d, %d>를 사용하려 합니다. 종료합니다.\n", node->brother->token.token_number, node->brother->token.token_value);
            exit(1);    // 미선언 변수 사용 시도한 경우.
        }
    }




    // *** CALL SON ***
    if (node->son != NULL) {
        ident_symbolizer(node->son);
    }







    // *** OUT ***
    // block 범위는 node 안쪽이고, node의 brother는 범위 바깥이므로, call brother 이전에 pop을 수행해야 한다.
    if (node->token.token_number == NT_BLOCK) {
        pop();
    }
    
    else if (node->token.token_number == NT_EXP) {
        if ((node->son->token.token_number >= OP_ASSIGN && node->son->token.token_number <= OP_LSREQ) && node->son->brother->token.token_number != SEM_SYMBOL) {
            printf("ERROR: Symbol이 아닌 토큰 <%d, %d>에 값 대입중. 종료합니다.\n", node->son->brother->token.token_number, node->son->brother->token.token_value);
            exit(1);   // Symbol이 아닌 토큰에 값 대입중. 잘못된 표현식이므로 오류.
        }
    }
    






    // *** CALL BROTHER ***
    if (node->brother != NULL) {
        ident_symbolizer(node->brother);
    }





    printf("Ident_Symbolizer END: Node <%d, %d>\n", node->token.token_number, node->token.token_value);
}

// assign일 때 왼쪽이 Symbol인지 확인해야함. 틀리면 오류.
// DECLR INT ident ASSIGN 덩어리(107~33 라인)는 exp assign 형식(109->33)으로 바꿔줄 것.
// declr 노드들의 정보는 심볼테이블에 저장되었으니 필요없다.



// ********** END IDENT_SYMBOLIZER ***********











/* ********************************************
***********************************************
***********************************************
***********                         ***********
***********    LABEL TABLE QUEUE    ***********
***********                         ***********
***********************************************
***********************************************
******************************************** */

int label_id_count = 1;

int label_table_queue_limit;
int label_table_queue_top;
int label_table_queue_base;

int * label_table_limit;
int * label_table_count;

Label_info *** label_table_queue;

void enqueue() {
    printf("Enqueuing Start. Current Table Queue Count: %d\n", label_table_queue_top);
    label_table_limit[label_table_queue_top] = 8;
    label_table_count[label_table_queue_top] = 0;
    Label_info ** label_table = malloc(sizeof(Label_info*) * label_table_limit[label_table_queue_top]);
    label_table_queue[label_table_queue_top++] = label_table;
    
    if (label_table_queue_top == label_table_queue_limit) {
        label_table_queue_limit = label_table_queue_limit * 2;
        label_table_queue = realloc(label_table_queue, sizeof(Label_info**) * label_table_queue_limit);
        memset((char*)label_table_queue + (label_table_queue_limit / 2) * sizeof(Label_info**), 0, (label_table_queue_limit / 2) * sizeof(Label_info**));
        
        label_table_limit = realloc(label_table_limit, sizeof(int) * label_table_queue_limit);
        label_table_count = realloc(label_table_count, sizeof(int) * label_table_queue_limit);
    }

    printf("Enqueuing Start. Current Table Queue Count: %d\n", label_table_queue_top);
}

void dequeue() {
    printf("Dequeuing\n");
    
    // 내부 전부 free 할것.
    for (int j = 0; j <= label_table_count[label_table_queue_base] - 1; j++) {        // 테이블 내부 순회
        free(label_table_queue[label_table_queue_base][j]);
    }
    free(label_table_queue[label_table_queue_base]);
    

    label_table_queue_base++;
}


int label_name_saver(Node * label_node) {

    printf("Saving Label Name %d\n", label_node->token.token_value);

    //label_node->토큰밸류 와 같은 라벨네임이 있는지 현재 테이블 순회해 검토. 있다면 오류 발생.
    for (int j = 0; j <= label_table_count[label_table_queue_top - 1] - 1; j++) {        // 테이블 내부 순회
        // printf("DEBUG. j: %d\n", j);
        if (label_table_queue[label_table_queue_top - 1][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
            //printf("DEBUG. Table[%d] is NULL. Break.\n", j);
            break;
        } else if (label_node->token.token_value == label_table_queue[label_table_queue_top - 1][j]->name) {
            printf("오류: 이미 선언된 Label Name <%d, %d>입니다. 종료합니다.\n", label_node->token.token_number, label_node->token.token_value);
            exit(1);
        }
    }

    printf("정상: 이전에 선언되지 않은 Label Name <%d, %d>입니다. 계속 진행합니다.\n", label_node->token.token_number, label_node->token.token_value);

    Label_info * label = malloc(sizeof(Label_info));
    label->name = label_node->token.token_value;
    label->id = label_id_count++;

    label_table_queue[label_table_queue_top - 1][label_table_count[label_table_queue_top - 1]++] = label;

    if (label_table_count[label_table_queue_top - 1] == label_table_limit[label_table_queue_top - 1]) {
        label_table_limit[label_table_queue_top - 1] = label_table_limit[label_table_queue_top - 1] * 2;
        label_table_queue[label_table_queue_top - 1] = realloc(label_table_queue[label_table_queue_top - 1], sizeof(Symbol_info*) * label_table_limit[label_table_queue_top - 1]);
    }

    printf("\n***** Label Making End. Made Label's info: *****\n");
    printf("\tName: %d\n", label_table_queue[label_table_queue_top - 1][label_table_count[label_table_queue_top - 1] - 1]->name);
    printf("\tID: %d\n", label_table_queue[label_table_queue_top - 1][label_table_count[label_table_queue_top - 1] - 1]->id);
    
    return label_table_queue[label_table_queue_top - 1][label_table_count[label_table_queue_top - 1] - 1]->id;
}

int label_name_finder(int label_token_value) {
        for (int j = 0; j <= label_table_count[label_table_queue_base] - 1; j++) {        // 테이블 내부 순회
            if (label_table_queue[label_table_queue_base][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                printf("DEBUG: label_table_queue[%d][%d] is NULL\n", label_table_queue_base, j);
                break;
            } else if (label_token_value == label_table_queue[label_table_queue_base][j]->name) {
                printf("DEBUG: %d는 label_table_queue[%d][%d]에 정상적으로 선언된 심볼입니다.\n", label_token_value, label_table_queue_base, j);
                return label_table_queue[label_table_queue_base][j]->id;
            }

            printf("DEBUG: label_table_queue[%d][%d]'s name is %d\n", label_table_queue_base, j, label_table_queue[label_table_queue_base][j]->name);
        }
    

    printf("오류: 선언되지 않은 Symbol Name %d을 사용하려 합니다. 종료합니다.\n", label_token_value);
    exit(1);    // 미선언 라벨 사용 시도한 경우.
}











/* ******************************************
*********************************************
*********************************************
***********                       ***********
***********    LABEL CONNECTOR    ***********
***********                       ***********
*********************************************
*********************************************
****************************************** */




int inside_func = 0;



void label_name_checker (Node * node) {
    printf("label_name_checker Start: Node <%d, %d>\n", node->token.token_number, node->token.token_value);


    // *** IN ***
    if (node->token.token_number == NT_FUNCTION) {
        inside_func++;
        enqueue();

    } else if (node->token.token_number == NT_LABEL) {
        if (inside_func == 1) {
            int label_id = label_name_saver(node);
            node->token.token_value = label_id;
        } else {
            printf("오류! 함수 바깥에서 라벨이 발견되었습니다. 종료합니다: <%d, %d>\n", node->token.token_number, node->token.token_value);
            exit(1);
        }
    }


    // *** CALL SON ***
    if (node->son != NULL) {
        label_name_checker(node->son);
    }


    // *** OUT ***
    if (node->token.token_number == NT_FUNCTION) {
        inside_func--;
    }


    // *** CALL BROTHER ***
    if (node->brother != NULL) {
        label_name_checker(node->brother);
    }
}



void goto_name_checker (Node * node) {
    printf("goto_name_checker Start: Node <%d, %d>\n", node->token.token_number, node->token.token_value);


    // *** IN ***
    if (node->token.token_number == NT_FUNCTION) {
        inside_func++;

    } else if (node->token.token_number == KW_GOTO) {
        if (inside_func == 1) {
            int label_id = label_name_finder(node->token.token_value);
            node->token.token_value = label_id;
        } else {
            printf("오류! 함수 바깥에서 라벨이 발견되었습니다. 종료합니다: <%d, %d>\n", node->token.token_number, node->token.token_value);
            exit(1);
        }
    }


    // *** CALL SON ***
    if (node->son != NULL) {
        goto_name_checker(node->son);
    }


    // *** OUT ***
    if (node->token.token_number == NT_FUNCTION) {
        dequeue();
        inside_func--;
    }


    // *** CALL BROTHER ***
    if (node->brother != NULL) {
        goto_name_checker(node->brother);
    }
}



void label_connector (Node * node) {
    label_name_checker(node);
    goto_name_checker(node);

}

















/* *******************************
**********************************
**********************************
***********            ***********
***********    MAIN    ***********
***********            ***********
**********************************
**********************************
******************************* */




Node * semantic_analyzer(Node * parse_input, char * symbast_name)
{
    symbol_table_list_limit = 8;
    symbol_table_list_count = 0;
    symbol_table_stack_limit = 4;
    symbol_table_stack_count = 0;
    symbol_table_list = calloc(symbol_table_list_limit, sizeof(Symbol_info**));
    symbol_table_stack = calloc(symbol_table_stack_limit, sizeof(Symbol_info**));
    symbol_table_limit = malloc(sizeof(int) * symbol_table_stack_limit);
    symbol_table_count = malloc(sizeof(int) * symbol_table_stack_limit);

    label_table_queue_limit = 4;
    label_table_queue_top = 0;
    label_table_queue_base = 0;
    label_table_queue = calloc(label_table_queue_limit, sizeof(Label_info**));
    label_table_limit = malloc(sizeof(int) * label_table_queue_limit);
    label_table_count = malloc(sizeof(int) * label_table_queue_limit);
    
    Node * symbolized_ast;


    FILE *symbastfp = fopen(symbast_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (symbastfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Semantic analyze 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", symbast_name);
        exit(1);
    }


    symbolized_ast = parse_input;
    ident_symbolizer(symbolized_ast->son->son->brother->brother->brother);
    printf("Ident Symbolizer가 정상적으로 완료되었습니다.\n");

    label_connector(symbolized_ast);
    printf("Label Connector가 정상적으로 완료되었습니다.\n");

    bin_tree_printer(symbolized_ast);
    bin_tree_file_printer(symbolized_ast, symbastfp);

    printf("Semantic analyzing Finished.\n");
    fclose(symbastfp);
    return symbolized_ast;
}
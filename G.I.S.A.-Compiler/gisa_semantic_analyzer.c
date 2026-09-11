#include "gisa_compiler.h"



int case_calculator (Node * node);
Node * get_type_tree_from_var_declr(Node * declr_node, Node * ident_node);
Node * get_type_tree_from_param_var_declr(Node * declr_node, Node * ident_node);
Node * get_type_tree_from_func_declr(Node * declr_node, Node * ident_node);
Node * get_type_tree_from_func_call(Node * ident_node);
int value_cut_typesize(int input, Node * typetree);







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

int * symbol_table_list_inside_count;


int func_table_limit;
int func_table_count;
Symbol_info ** func_table;

int func_depth = 0;

Node * current_func_typetree = NULL;

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
    symbol_table_list[symbol_table_list_count] = symbol_table_stack[--symbol_table_stack_count];
    symbol_table_list_inside_count[symbol_table_list_count] = symbol_table_count[symbol_table_stack_count];
    symbol_table_stack[symbol_table_stack_count] = NULL;

    symbol_table_list_count++;
    
    if (symbol_table_list_count == symbol_table_list_limit) {
        symbol_table_list_limit = symbol_table_list_limit * 2;
        symbol_table_list = realloc(symbol_table_list, sizeof(Symbol_info**) * symbol_table_list_limit);
        symbol_table_list_inside_count = realloc(symbol_table_list_inside_count, sizeof(int) * symbol_table_list_limit);
    }
    
    if ((symbol_table_stack_count <= symbol_table_stack_limit / 4) && (symbol_table_stack_count > 0)) {
        symbol_table_stack_limit = symbol_table_stack_limit / 2;
        symbol_table_stack = realloc(symbol_table_stack, sizeof(Symbol_info**) * symbol_table_stack_limit);
        
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


    if (ident_node->brother == NULL || (ident_node->brother != NULL && ident_node->brother->token.token_number != NT_PARAM_LIST)) { // !!!변수 선언일 경우!!!
        printf("선언되는 IDENT는 변수입니다.\n");

        // ***** linkage 변수 처리부분 시작!!! *****
        if (declr_node->token.token_number == NT_VAR_DECLR && (func_depth == 0 || declr_node->son->son->brother->token.token_value == 1)) { // linkage가 있는 변수의 경우. (함수 깊이가 0 || EXTERN 키워드가 있음)
            // 현재 스코프에 같은 이름의 변수가 있고 extern이 아닐 경우. 있다면 오류.
            for (int j = 0; j <= symbol_table_count[symbol_table_stack_count - 1] - 1; j++) {        // 테이블 내부 순회
                printf("DEBUG. j: %d\n", j);
                if (symbol_table_stack[symbol_table_stack_count - 1][j] == NULL) {
                    printf("DEBUG. Table[%d] is NULL. Break.\n", j);
                    break;
                } else if ((ident_node->token.token_value == symbol_table_stack[symbol_table_stack_count - 1][j]->name) && (symbol_table_stack[symbol_table_stack_count - 1][j]->is_linkage == 0)) {
                    printf("오류: 이미 linkage가 없도록 선언된 Symbol Name <%d, %d>입니다. 종료합니다.\n", ident_node->token.token_number, ident_node->token.token_value);
                    exit(1);
                }
            }

            for (int j = 0; j <= func_table_count - 1; j++) {        // 함수 테이블 순회 <- linkage table로 이름 바꿀것
                printf("DEBUG. j: %d\n", j);
                if (ident_node->token.token_value == func_table[j]->name) {
                    if (func_table[j]->is_func == 1) {
                        printf("오류: 이전에 함수로 선언된 Name입니다: Symbol Name <%d, %d>.\n", ident_node->token.token_number, ident_node->token.token_value);
                        exit(1);
                    }
                    
                    printf("이전에 선언된 적이 있는 linkage 변수입니다: Symbol Name <%d, %d>. 새로운 심볼을 생성하지 않고, 해당 변수의 심볼을 이용합니다.\n", ident_node->token.token_number, ident_node->token.token_value);

                    Node * input_node_typetree = get_type_tree_from_var_declr(declr_node, ident_node);
                    if (compare_tree(input_node_typetree, func_table[j]->type_tree) == 0) {
                        printf("오류: 이전에 선언된 linkage 변수의 타입과 다른 타입으로 선언되었습니다.\n");
                        printf("이전에 선언된 linkage 변수의 타입 트리:\n");
                        bin_tree_printer(func_table[j]->type_tree);
                        printf("새로 선언된 linkage 변수의 타입 트리:\n");
                        bin_tree_printer(input_node_typetree);
                        tree_malloc_cleaner(input_node_typetree);
                        exit(1);
                    } else {
                        tree_malloc_cleaner(input_node_typetree);
                        printf("정상:이전에 선언된 linkage 변수의 타입과 같은 타입으로 선언되었습니다.\n");
                    }


                    
                    if ((declr_node->son->son->token.token_value == 1) && (func_table[j]->is_global == 1)) {
                        printf("오류: 이전에 global == 1로 선언된 변수를 static으로 재선언(global == 0)했습니다. 종료합니다.\n");                            
                        exit(1);
                    } else if (((declr_node->son->son->token.token_value == 0) && (declr_node->son->son->brother->token.token_value == 0)) && (func_table[j]->is_global == 0)) {
                        printf("오류: 이전에 global == 0로 선언된 변수를 키워드 없이 재선언(global == 1)했습니다. 종료합니다.\n");                            
                        exit(1);
                    }


                    if((ident_node->brother->son != NULL) && (ident_node->brother->son->brother->token.token_number == NUM_INT || ident_node->brother->son->brother->token.token_number == NUM_LONG)) { // init_option = 1로 재선언한 경우 
                        if (func_table[j]->init_option == 1) {
                            printf("오류: 초기값이 확정된 변수를 재선언하며 다시 초기값을 설졍하려고 합니다. 종료합니다.\n");
                            exit(1);
                        } else {
                            printf("이전 init_option이 %d이었던 변수의 초기값을 설정했습니다.\n", func_table[j]->init_option);
                            func_table[j]->init_option = 1;
                            char cut_val[12];
                            snprintf(cut_val, sizeof(cut_val), "%d", value_cut_typesize(atoi(lexval_finder(ident_node->brother->son->brother->token.token_value)), func_table[j]->type_tree));
                            int type_set_init_value = lexval_manager(cut_val);
                            func_table[j]->init_value = type_set_init_value;    // 주의: 실제 상수값이 아니라 lexeme number이 저장된다. lexval_manager에서 꺼내 사용해야 함!
                        }

                    } else if (!(declr_node->son->son->brother->token.token_value == 1) && (ident_node->brother->son == NULL)) { // init_option = 1로 재선언한 경우 
                        if (func_table[j]->init_option == 3) {
                            printf("이전 init_option이 %d이었던 변수의 option을 2로 설정했습니다.\n", func_table[j]->init_option);
                            func_table[j]->init_option = 2;
                        }
                    }
                    



                    symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1]++] = func_table[j];

                    if (symbol_table_count[symbol_table_stack_count - 1] == symbol_table_limit[symbol_table_stack_count - 1]) {
                        symbol_table_limit[symbol_table_stack_count - 1] = symbol_table_limit[symbol_table_stack_count - 1] * 2;
                        symbol_table_stack[symbol_table_stack_count - 1] = realloc(symbol_table_stack[symbol_table_stack_count - 1], sizeof(Symbol_info*) * symbol_table_limit[symbol_table_stack_count - 1]);
                    }

                    return symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id;
                    // 이전에 선언된 linkage 변수 처리 끝.
                }
            }

            printf("이전에 선언되지 않은 linkage 변수 Symbol Name <%d, %d>입니다. 새로운 심볼을 생성해 저장합니다.\n", ident_node->token.token_number, ident_node->token.token_value);

            // ***** 심볼 만들어 정보 채우기 *****
            Symbol_info * symbol = malloc(sizeof(Symbol_info));
            symbol->name = ident_node->token.token_value;
            symbol->id = symbol_id_count++;
            symbol->type_tree = get_type_tree_from_var_declr(declr_node, ident_node);    // 이후 확장할 것. 형식도 enum으로 개선하고...
            symbol->size = 4;               // 수정 및 처리가 필요할듯.
            symbol->location.type = 0;
            symbol->location.location = 0;
            symbol->is_func = 0;
            symbol->having_body = 0;
            symbol->is_linkage = 1;
            symbol->is_global = 0;
            symbol->init_option = 0;
            symbol->init_value = 0;

            

            // ***** 심볼을 테이블에 저장 *****
            func_table[func_table_count++] = symbol;

            if (func_table_count == func_table_limit) {
                func_table_limit = func_table_limit * 2;
                func_table = realloc(func_table, sizeof(Symbol_info*) * func_table_limit);
            }

            symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1]++] = func_table[func_table_count - 1];

            if (symbol_table_count[symbol_table_stack_count - 1] == symbol_table_limit[symbol_table_stack_count - 1]) {
                symbol_table_limit[symbol_table_stack_count - 1] = symbol_table_limit[symbol_table_stack_count - 1] * 2;
                symbol_table_stack[symbol_table_stack_count - 1] = realloc(symbol_table_stack[symbol_table_stack_count - 1], sizeof(Symbol_info*) * symbol_table_limit[symbol_table_stack_count - 1]);
            }

            // ***** 저장한 심볼을 프린트하기 *****
            printf("\n***** Symbol Making End. Made Symbol's info: *****\n");
            printf("\tName: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->name);
            printf("\tID: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id);
            printf("\tType Tree:\n");
            bin_tree_printer(symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->type_tree);
            printf("\tSize: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->size);
            printf("\tLocation.Type: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->location.type);
            printf("\tLocation.Location: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->location.location);
            printf("\tIs Function: NO\n");
            printf("\tHaving Body: NO (It's only about function.)\n");
            printf("\tIs Linkage: YES\n");


            if (func_depth == 0) {  // 파일 스코프인 경우
                if (declr_node->son->son->token.token_value == 1) { // static 키워드가 있는 경우
                    symbol->is_global = 0;
                    printf("\tIs Global: NO\n");
                    if (ident_node->brother->son == NULL) { // 초기값이 없는 경우. 
                        symbol->init_option = 2;
                        symbol->init_value = 0;
                    } else if ((ident_node->brother->son->brother != NULL) && (ident_node->brother->son->brother->token.token_number == NUM_INT || ident_node->brother->son->brother->token.token_number == NUM_LONG)) { // 초기값이 상수인 경우
                        symbol->init_option = 1;
                        char cut_val[12];
                        snprintf(cut_val, sizeof(cut_val), "%d", value_cut_typesize(atoi(lexval_finder(ident_node->brother->son->brother->token.token_value)), symbol->type_tree));
                        int type_set_init_value = lexval_manager(cut_val);
                        symbol->init_value = type_set_init_value;    // 주의: 실제 상수값이 아니라 lexeme number이 저장된다. lexval_manager에서 꺼내 사용해야 함!
                    } else {    // 초기값이 있지만 상수가 아닌경우
                        printf("오류: static 키워드가 있는 파일 스코프 변수는 초기값으로 상수 외의 값을 가질 수 없습니다. 종료합니다.\n");
                        exit(1);
                    }

                } else if (declr_node->son->son->brother->token.token_value == 1) { // extern 키워드가 있는 경우
                    symbol->is_global = 1;
                    printf("\tIs Global: YES\n");
                    if (ident_node->brother->son == NULL) { // 초기값이 없는 경우. 
                        symbol->init_option = 3;
                        symbol->init_value = 0;
                    } else if ((ident_node->brother->son->brother != NULL) && (ident_node->brother->son->brother->token.token_number == NUM_INT || ident_node->brother->son->brother->token.token_number == NUM_LONG)) { // 초기값이 상수인 경우
                        symbol->init_option = 1;
                        char cut_val[12];
                        snprintf(cut_val, sizeof(cut_val), "%d", value_cut_typesize(atoi(lexval_finder(ident_node->brother->son->brother->token.token_value)), symbol->type_tree));
                        int type_set_init_value = lexval_manager(cut_val);
                        symbol->init_value = type_set_init_value;    // 주의: 실제 상수값이 아니라 lexeme number이 저장된다. lexval_manager에서 꺼내 사용해야 함!
                    } else {    // 초기값이 있지만 상수가 아닌경우
                        printf("오류: extern 키워드가 있는 파일 스코프 변수는 초기값으로 상수 외의 값을 가질 수 없습니다. 종료합니다.\n");
                        exit(1);
                    }          

                } else {    // 키워드가 없는 경우
                    symbol->is_global = 1;
                    printf("\tIs Global: YES\n");
                    if (ident_node->brother->son == NULL) { // 초기값이 없는 경우. 
                        symbol->init_option = 2;
                        symbol->init_value = 0;
                    } else if ((ident_node->brother->son->brother != NULL) && (ident_node->brother->son->brother->token.token_number == NUM_INT || ident_node->brother->son->brother->token.token_number == NUM_LONG)) { // 초기값이 상수인 경우
                        symbol->init_option = 1;
                        char cut_val[12];
                        snprintf(cut_val, sizeof(cut_val), "%d", value_cut_typesize(atoi(lexval_finder(ident_node->brother->son->brother->token.token_value)), symbol->type_tree));
                        int type_set_init_value = lexval_manager(cut_val);
                        symbol->init_value = type_set_init_value;    // 주의: 실제 상수값이 아니라 lexeme number이 저장된다. lexval_manager에서 꺼내 사용해야 함!
                    } else {    // 초기값이 있지만 상수가 아닌경우
                        printf("오류: 키워드가 없는 파일 스코프 변수는 초기값으로 상수 외의 값을 가질 수 없습니다. 종료합니다.\n");
                        exit(1);
                    }
                }
            } else {
                symbol->is_global = 1;
                printf("\tIs Global: YES\n");
                if (ident_node->brother->son == NULL) { // 초기값이 없는 경우. 
                    symbol->init_option = 3;
                    symbol->init_value = 0;
                } else {    // 초기값이 있지만 상수가 아닌경우
                    printf("오류: extern 키워드가 있는 블록 스코프 변수는 초기값을 가질 수 없습니다. 종료합니다.\n");
                    exit(1);
                }     
                
            }
            
            printf("\tInitial Option: %d\n", symbol->init_option);
            printf("\tInitial Value: %d\n\n\n", symbol->init_value);

            

            return symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id;
        }
        // ***** linkage 변수 처리부분 끝!!! *****



        

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

        // ***** 심볼 만들어 정보 채우기 *****
        Symbol_info * symbol = malloc(sizeof(Symbol_info));
        symbol->name = ident_node->token.token_value;
        symbol->id = symbol_id_count++;
        if (declr_node->token.token_number == NT_PARAM_TYPE_LIST) {
            symbol->type_tree = get_type_tree_from_param_var_declr(declr_node, ident_node);
        } else {
            symbol->type_tree = get_type_tree_from_var_declr(declr_node, ident_node);    // 이후 확장할 것. 형식도 enum으로 개선하고...
        }
        symbol->size = 4;               // 위의 while문에서 typetree 만드는 함수도 만들어 호출하면 좋을것 같음.
        symbol->location.type = 0;
        symbol->location.location = 0;
        symbol->is_func = 0;
        symbol->having_body = 0;
        symbol->is_linkage = 0;
        symbol->is_global = 0;
        if (declr_node->son->son != NULL && declr_node->son->son->token.token_value == 1) { // static 키워드가 있는 경우
            symbol->init_option = 1;
            if (ident_node->brother->son == NULL) { // 초기값이 없는 경우. 
                symbol->init_value = 0;
            } else if ((ident_node->brother->son->brother != NULL) && (ident_node->brother->son->brother->token.token_number == NUM_INT || ident_node->brother->son->brother->token.token_number == NUM_LONG)) { // 초기값이 상수인 경우
                char cut_val[12];
                snprintf(cut_val, sizeof(cut_val), "%d", value_cut_typesize(atoi(lexval_finder(ident_node->brother->son->brother->token.token_value)), symbol->type_tree));
                int type_set_init_value = lexval_manager(cut_val);
                symbol->init_value = type_set_init_value;    // 주의: 실제 상수값이 아니라 lexeme number이 저장된다. lexval_manager에서 꺼내 사용해야 함!
            } else {    // 초기값이 있지만 상수가 아닌경우
                printf("오류: static 키워드가 있는 블록 스코프 변수는 초기값으로 상수 외의 값을 가질 수 없습니다. 종료합니다.\n");
                exit(1);
            }
        } else {
            symbol->init_option = 0;
            symbol->init_value = 0;
        }

        // ***** 심볼을 테이블에 저장 *****
        symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1]++] = symbol;

        if (symbol_table_count[symbol_table_stack_count - 1] == symbol_table_limit[symbol_table_stack_count - 1]) {
            symbol_table_limit[symbol_table_stack_count - 1] = symbol_table_limit[symbol_table_stack_count - 1] * 2;
            symbol_table_stack[symbol_table_stack_count - 1] = realloc(symbol_table_stack[symbol_table_stack_count - 1], sizeof(Symbol_info*) * symbol_table_limit[symbol_table_stack_count - 1]);
        }

        // ***** 저장한 심볼을 프린트하기 *****
        printf("\n***** Symbol Making End. Made Symbol's info: *****\n");
        printf("\tName: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->name);
        printf("\tID: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id);
        printf("\tType Tree's Top: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->type_tree->token.token_number);
        printf("\tSize: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->size);
        printf("\tLocation.Type: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->location.type);
        printf("\tLocation.Location: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->location.location);
        printf("\tIs Function: NO\n");
        printf("\tHaving Body: NO (It's only about function.)\n");
        printf("\tIs Linkage: NO\n");
        printf("\tIs Global: NO\n");
        printf("\tInitial Option: %d\n", symbol->init_option);
        printf("\tInitial Value: %d\n\n\n", symbol->init_value);

        return symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id;
        // 변수 선언 끝!


    } else { // !!!함수 선언일 경우!!!
        printf("선언되는 IDENT는 함수입니다.\n");
        // 이 함수가 이전에 변수로 선언된 적이 있는지 확인.
        for (int j = 0; j <= symbol_table_count[symbol_table_stack_count - 1] - 1; j++) {        // 테이블 내부 순회
            printf("DEBUG. j: %d\n", j);
            if (symbol_table_stack[symbol_table_stack_count - 1][j] == NULL) {     
                printf("DEBUG. Table[%d] is NULL. Break.\n", j);
                break;
            } else if (ident_node->token.token_value == symbol_table_stack[symbol_table_stack_count - 1][j]->name && symbol_table_stack[symbol_table_stack_count - 1][j]->is_func == 0) {
                printf("오류: 이미 변수로 선언된 Symbol Name <%d, %d>을, 함수로 다시 선언 시도하고 있습니다. 종료합니다.\n", ident_node->token.token_number, ident_node->token.token_value);
                exit(1);
            }
        }

        // 이 함수가 이전에 함수로 선언된 적이 있는지 확인.
        for (int j = 0; j <= func_table_count - 1; j++) {        // 함수 테이블 순회
            printf("DEBUG. j: %d\n", j);
            if (ident_node->token.token_value == func_table[j]->name) {
                if (func_table[j]->is_func == 0) {
                    printf("오류: 이전에 linkage가 있는 변수로 선언된 Name입니다: Symbol Name <%d, %d>.\n", ident_node->token.token_number, ident_node->token.token_value);
                    exit(1);
                }

                printf("이전에 선언된 적이 있는 함수입니다: Symbol Name <%d, %d>. 새로운 심볼을 생성하지 않고, 해당 함수의 심볼을 이용합니다.\n", ident_node->token.token_number, ident_node->token.token_value);

                Node * input_node_typetree = get_type_tree_from_func_declr(declr_node, ident_node);
                if (compare_tree(input_node_typetree, func_table[j]->type_tree) == 0) {
                    printf("오류: 이전에 선언된 함수의 타입과 다른 타입으로 선언되었습니다.\n");
                    printf("이전에 선언된 함수의 타입 트리:\n");
                    bin_tree_printer(func_table[j]->type_tree);
                    printf("새로 선언된 함수의 타입 트리:\n");
                    bin_tree_printer(input_node_typetree);
                    tree_malloc_cleaner(input_node_typetree);
                    exit(1);
                } else {
                    tree_malloc_cleaner(input_node_typetree);
                    printf("정상:이전에 선언된 함수의 타입과 같은 타입으로 선언되었습니다.\n");
                }

                if (func_table[j]->is_global == 1 && declr_node->son->son->token.token_value == 1) {
                    printf("오류: 이전에 extern/키워드 없음으로 선언된 함수를 static으로 선언하고 있습니다. 종료합니다.\n");
                    exit(1);
                }

                if (func_table[j]->having_body == 0) {  // 정의된적 없는 함수인 경우
                    if (ident_node->brother->brother != NULL && ident_node->brother->brother->token.token_number == NT_BLOCK) {
                        if (symbol_table_stack_count == 1) {
                            printf("정상: 함수가 정의되었습니다.\n");
                            func_table[j]->having_body = 1;
                            func_table[j]->location.type = 1;
                            func_table[j]->location.location = label_id_count++;
                        } else {
                            printf("오류: 스코프 베이스가 아닌 곳에서 함수 정의가 시도되었습니다.\n");
                            exit(1);
                        }
                    }
                } else {    // 정의된적 있는 함수인 경우
                    if (ident_node->brother->brother != NULL && ident_node->brother->brother->token.token_number == NT_BLOCK) {
                        printf("오류: 함수가 두 번 이상 정의되었습니다.\n");
                        exit(1);
                    }
                }

                symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1]++] = func_table[j];

                if (symbol_table_count[symbol_table_stack_count - 1] == symbol_table_limit[symbol_table_stack_count - 1]) {
                    symbol_table_limit[symbol_table_stack_count - 1] = symbol_table_limit[symbol_table_stack_count - 1] * 2;
                    symbol_table_stack[symbol_table_stack_count - 1] = realloc(symbol_table_stack[symbol_table_stack_count - 1], sizeof(Symbol_info*) * symbol_table_limit[symbol_table_stack_count - 1]);
                }

                return symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id;
                // 이전에 선언된 함수 처리 끝.
            }
        }

        printf("이전에 선언되지 않은 함수 Symbol Name <%d, %d>입니다. 새로운 심볼을 생성해 저장합니다.\n", ident_node->token.token_number, ident_node->token.token_value);

        // ***** 심볼 만들어 정보 채우기 *****
        Symbol_info * symbol = malloc(sizeof(Symbol_info));
        symbol->name = ident_node->token.token_value;
        symbol->id = symbol_id_count++;
        symbol->type_tree = get_type_tree_from_func_declr(declr_node, ident_node);    // 이후 확장할 것. 형식도 enum으로 개선하고...
        symbol->size = 4;               // 수정 및 처리가 필요할듯.
        symbol->location.type = 0;
        symbol->location.location = 0;
        symbol->is_func = 1;
        symbol->having_body = 0;
        symbol->is_linkage = 1;
        symbol->init_option = 0;
        symbol->init_value = 0;

        if (ident_node->brother->brother != NULL && ident_node->brother->brother->token.token_number == NT_BLOCK) {
            if (symbol_table_stack_count == 1) {
                symbol->having_body = 1;
                symbol->location.type = 1;
                symbol->location.location = label_id_count++;
            } else {
                printf("오류: 스코프 베이스가 아닌 곳에서 함수 정의가 시도되었습니다.");
                exit(1);
            }
        }

        if (declr_node->son->son->token.token_value == 1) { // static인 경우.
            symbol->is_global = 0;
        } else {
            symbol->is_global = 1;
        }

        

        // ***** 심볼을 테이블에 저장 *****
        func_table[func_table_count++] = symbol;

        if (func_table_count == func_table_limit) {
            func_table_limit = func_table_limit * 2;
            func_table = realloc(func_table, sizeof(Symbol_info*) * func_table_limit);
        }

        symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1]++] = func_table[func_table_count - 1];

        if (symbol_table_count[symbol_table_stack_count - 1] == symbol_table_limit[symbol_table_stack_count - 1]) {
            symbol_table_limit[symbol_table_stack_count - 1] = symbol_table_limit[symbol_table_stack_count - 1] * 2;
            symbol_table_stack[symbol_table_stack_count - 1] = realloc(symbol_table_stack[symbol_table_stack_count - 1], sizeof(Symbol_info*) * symbol_table_limit[symbol_table_stack_count - 1]);
        }

        // ***** 저장한 심볼을 프린트하기 *****
        printf("\n***** Symbol Making End. Made Symbol's info: *****\n");
        printf("\tName: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->name);
        printf("\tID: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id);
        printf("\tType Tree:\n");
        bin_tree_printer(symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->type_tree);
        printf("\tSize: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->size);
        printf("\tLocation.Type: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->location.type);
        printf("\tLocation.Location: %d\n", symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->location.location);
        printf("\tIs Function: YES\n");
        if(symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->having_body == 1) {
            printf("\tHaving Body: YES\n");
        } else {
            printf("\tHaving Body: NO\n");
        }
        printf("\tIs Linkage: YES\n");
        if(symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->is_global == 1) {
            printf("\tIs Global: YES\n");
        } else {
            printf("\tIs Global: NO\n");
        }
        printf("\tInit Option: 0 (It's only about variation.)\n");
        printf("\tInit Value: 0 (It's only about variation.)\n\n\n");
        

        return symbol_table_stack[symbol_table_stack_count - 1][symbol_table_count[symbol_table_stack_count - 1] - 1]->id;
    }

    
}

Symbol_info * symbol_finder_from_ident_node(Node * ident_node) {
    for (int i = symbol_table_stack_count - 1; i >= 0; i--) {      // 테이블 스택 순회
        for (int j = 0; j <= symbol_table_count[i] - 1; j++) {        // 테이블 내부 순회
            if (symbol_table_stack[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                printf("DEBUG: symbol_table_stack[%d][%d] is NULL\n", i, j);
                break;
            } else if (ident_node->token.token_value == symbol_table_stack[i][j]->name) {
                printf("DEBUG: %d는 symbol_table_stack[%d][%d]에 정상적으로 선언된 심볼입니다.\n", ident_node->token.token_value, i, j);

                if (symbol_table_stack[i][j]->is_func == 0 && ident_node->brother != NULL && ident_node->brother->token.token_number == NT_ARG_LIST) {
                    printf("오류: 변수로 선언된 심볼(id: %d, name: %d)를, 노드 <%d, %d>에서 함수처럼 호출하고 있습니다.", symbol_table_stack[i][j]->id, symbol_table_stack[i][j]->name, ident_node->token.token_number, ident_node->token.token_value);
                    exit(1);
                } else if (symbol_table_stack[i][j]->is_func == 1 && (ident_node->brother == NULL || (ident_node->brother != NULL && ident_node->brother->token.token_number != NT_ARG_LIST))) {
                    printf("오류: 함수로 선언된 심볼(id: %d, name: %d)를, 노드 <%d, %d>에서 변수처럼 호출하고 있습니다.", symbol_table_stack[i][j]->id, symbol_table_stack[i][j]->name, ident_node->token.token_number, ident_node->token.token_value);
                    exit(1);
                }

                
                return symbol_table_stack[i][j];
            }

            printf("DEBUG: symbol_table_stack[%d][%d]'s name is %d\n", i, j, symbol_table_stack[i][j]->name);
        }
    }

    printf("오류: 선언되지 않은 Symbol Name %d을 사용하려 합니다. 종료합니다.\n", ident_node->token.token_value);
    exit(1);    // 미선언 변수 사용 시도한 경우.
}

Symbol_info * symbol_finder_from_symbol_node(Node * symbol_node) {
    printf("DEBUG: 심볼을 찾기 위해 심볼 테이블 리스트를 순회합니다.\n");
    for (int i = symbol_table_list_count - 1; i >= 0; i--) {      // 테이블 스택 순회
        for (int j = 0; j <= symbol_table_list_inside_count[i] - 1; j++) {        // 테이블 내부 순회
            if (symbol_table_list[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                printf("DEBUG: symbol_table_list[%d][%d] is NULL\n", i, j);
                break;
            } else if (symbol_node->token.token_value == symbol_table_list[i][j]->id) {
                printf("DEBUG: Symbol ID %d는 symbol_table_list[%d][%d]에 정상적으로 선언된 심볼입니다.\n", symbol_node->token.token_value, i, j);
                
                return symbol_table_list[i][j];
            }

            printf("DEBUG: symbol_table_list[%d][%d]'s name is %d\n", i, j, symbol_table_list[i][j]->name);
        }
    }

    printf("DEBUG: 심볼 테이블 리스트에서 심볼을 찾지 못했습니다. 심볼 테이블 스택을 순회합니다.\n");
    for (int i = symbol_table_stack_count - 1; i >= 0; i--) {      // 테이블 스택 순회
        for (int j = 0; j <= symbol_table_count[i] - 1; j++) {        // 테이블 내부 순회
            if (symbol_table_stack[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                printf("DEBUG: symbol_table_stack[%d][%d] is NULL\n", i, j);
                break;
            } else if (symbol_node->token.token_value == symbol_table_stack[i][j]->id) {
                printf("DEBUG: Symbol ID %d는 symbol_table_list[%d][%d]에 정상적으로 선언된 심볼입니다.\n", symbol_node->token.token_value, i, j);

                return symbol_table_stack[i][j];
            }

            printf("DEBUG: symbol_table_stack[%d][%d]'s name is %d\n", i, j, symbol_table_stack[i][j]->name);
        }
    }

    printf("오류: 선언되지 않은 Symbol Name %d을 사용하려 합니다. 종료합니다.\n", symbol_node->token.token_value);
    exit(1);    // 미선언 변수 사용 시도한 경우.
}

Symbol_info * symbol_finder_from_symbol_id(int symbol_id) {
    if (symbol_id <= symbol_id_count) {
        printf("DEBUG: id가 명시변수 심볼 id 범위인 1~%d에 속하므로, 명시변수들 사이에서 심볼을 찾습니다.\n", symbol_id_count);
        printf("DEBUG: id가 %d인 심볼을 찾기 위해 심볼 테이블 리스트를 순회합니다.\n", symbol_id);
        for (int i = symbol_table_list_count - 1; i >= 0; i--) {      // 테이블 스택 순회
            for (int j = 0; j <= symbol_table_list_inside_count[i] - 1; j++) {        // 테이블 내부 순회
                if (symbol_table_list[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                    printf("DEBUG: symbol_table_list[%d][%d] is NULL\n", i, j);
                    break;
                } else if (symbol_id == symbol_table_list[i][j]->id) {
                    printf("DEBUG: Symbol ID %d는 symbol_table_list[%d][%d]에 정상적으로 선언된 심볼입니다.\n", symbol_id, i, j);
                    
                    return symbol_table_list[i][j];
                }

                printf("DEBUG: symbol_table_list[%d][%d]'s name is %d\n", i, j, symbol_table_list[i][j]->name);
            }
        }

        printf("DEBUG: 심볼 테이블 리스트에서 심볼을 찾지 못했습니다. 심볼 테이블 스택을 순회합니다.\n");
        for (int i = symbol_table_stack_count - 1; i >= 0; i--) {      // 테이블 스택 순회
            for (int j = 0; j <= symbol_table_count[i] - 1; j++) {        // 테이블 내부 순회
                if (symbol_table_stack[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                    printf("DEBUG: symbol_table_stack[%d][%d] is NULL\n", i, j);
                    break;
                } else if (symbol_id == symbol_table_stack[i][j]->id) {
                    printf("DEBUG: Symbol ID %d는 symbol_table_list[%d][%d]에 정상적으로 선언된 심볼입니다.\n", symbol_id, i, j);

                    return symbol_table_stack[i][j];
                }

                printf("DEBUG: symbol_table_stack[%d][%d]'s name is %d\n", i, j, symbol_table_stack[i][j]->name);
            }
        }

        printf("오류: 선언되지 않은 Symbol ID %d을 사용하려 합니다. 종료합니다.\n", symbol_id);
        exit(1);    // 미선언 변수 사용 시도한 경우.
        
    } else {
        printf("DEBUG: id가 임시변수 심볼 id 범위인 (%d + 1)~ 에 속하므로, 임시변수들 사이에서 심볼을 찾습니다.\n", symbol_id_count);
        
        for (int i = 0; i < temp_count - (symbol_id_count + 1); i++) {      // 테이블 스택 순회
            if (temp_table[i] != NULL && symbol_id == temp_table[i]->id) {
                printf("DEBUG: Symbol ID %d는 temp_table[%d]에 정상적으로 선언된 심볼입니다.\n", symbol_id, i);
                    
                return temp_table[i];
            }
        }

        printf("오류: 선언되지 않은 Symbol ID %d을 사용하려 합니다. 종료합니다.\n", symbol_id);
        exit(1);    // 미선언 변수 사용 시도한 경우.
    }
    
}

Symbol_info * symbol_finder_from_symbol_name(int symbol_name) {
    printf("DEBUG: NAME이 %d인 심볼을 찾기 위해 심볼 테이블 리스트를 순회합니다.\n", symbol_name);
    for (int i = symbol_table_list_count - 1; i >= 0; i--) {      // 테이블 스택 순회
        for (int j = 0; j <= symbol_table_list_inside_count[i] - 1; j++) {        // 테이블 내부 순회
            if (symbol_table_list[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                printf("DEBUG: symbol_table_list[%d][%d] is NULL\n", i, j);
                break;
            } else if (symbol_name == symbol_table_list[i][j]->name) {
                printf("DEBUG: Symbol NAME %d는 symbol_table_list[%d][%d]에 정상적으로 선언된 심볼입니다.\n", symbol_name, i, j);
                
                return symbol_table_list[i][j];
            }

            printf("DEBUG: symbol_table_list[%d][%d]'s name is %d\n", i, j, symbol_table_list[i][j]->name);
        }
    }

    printf("DEBUG: 심볼 테이블 리스트에서 심볼을 찾지 못했습니다. 심볼 테이블 스택을 순회합니다.\n");
    for (int i = symbol_table_stack_count - 1; i >= 0; i--) {      // 테이블 스택 순회
        for (int j = 0; j <= symbol_table_count[i] - 1; j++) {        // 테이블 내부 순회
            if (symbol_table_stack[i][j] == NULL) {     // 테이블 및 info들 초기화 하도록 수정해야 함.
                printf("DEBUG: symbol_table_stack[%d][%d] is NULL\n", i, j);
                break;
            } else if (symbol_name == symbol_table_stack[i][j]->name) {
                printf("DEBUG: Symbol NAME %d는 symbol_table_list[%d][%d]에 정상적으로 선언된 심볼입니다.\n", symbol_name, i, j);

                return symbol_table_stack[i][j];
            }

            printf("DEBUG: symbol_table_stack[%d][%d]'s name is %d\n", i, j, symbol_table_stack[i][j]->name);
        }
    }

    printf("오류: 선언되지 않은 Symbol NAME %d을 사용하려 합니다. 종료합니다.\n", symbol_name);
    exit(1);    // 미선언 변수 사용 시도한 경우.
}

void func_typetree_validate(Node * symbol_node) {
    Symbol_info * func_node = (symbol_finder_from_symbol_node(symbol_node));
    if (func_node->is_func == 1) {
                    Node * input_node_typetree_copy = symbol_node->brother->son;
                    Node * current_input_typetree = input_node_typetree_copy;
                    Node * func_typetree = func_node->type_tree->brother;
                    if (func_typetree->son->token.token_number == KW_VOID) {
                        if (current_input_typetree == NULL) {
                            printf("정상:이전에 선언된 함수의 인자와 같은 인자로 호출했습니다.\n");
                        } else {
                            printf("오류: 이전에 선언된 NULL 함수를 인자를 가진 함수로 호출하고 있습니다.\n");
                            printf("이전에 선언된 함수의 인자 타입 트리:\n");
                            bin_tree_printer(func_node->type_tree->brother);
                            printf("호출하는 함수의 인자 타입 트리:\n");
                            bin_tree_printer(input_node_typetree_copy);
                            exit(1);
                        }
                        
                    } else {
                        if (current_input_typetree == NULL) {
                            printf("오류: 인자가 필요한 함수를 인자 없이 호출했습니다.\n");
                            exit(1);
                        } 

                        while (current_input_typetree != NULL) {
                            if (func_typetree == NULL) {
                                printf("오류: 이전에 선언된 함수의 인자 타입보다 많은 개수의 인자들로 호출하고 있습니다.\n");
                                printf("이전에 선언된 함수의 인자 타입 트리:\n");
                                bin_tree_printer(func_node->type_tree->brother);
                                printf("호출하는 함수의 인자 타입 트리:\n");
                                bin_tree_printer(input_node_typetree_copy);
                                exit(1);
                            }

                            if (compare_tree(current_input_typetree->son->son, func_typetree->son) == 0) {
                                Node * current_exp_content = current_input_typetree->son;
                                Node * cast = node_maker(copy_tree(func_typetree->son), node_maker(current_exp_content, NULL, NT_EXP, 0), NT_CAST, 0);
                                current_input_typetree->son = cast;
                                current_input_typetree->token.token_number = NT_EXP;
                                current_input_typetree->token.token_value = 0;
                            }

                            current_input_typetree = current_input_typetree->brother;
                            func_typetree = func_typetree->brother;

                        }
                        if (func_typetree != NULL) {
                            printf("오류: 이전에 선언된 함수의 인자 타입보다 적은 개수의 인자들로 호출하고 있습니다.\n");
                            printf("이전에 선언된 함수의 인자 타입 트리:\n");
                            bin_tree_printer(func_node->type_tree->brother);
                            printf("호출하는 함수의 인자 타입 트리:\n");
                            bin_tree_printer(input_node_typetree_copy);
                            exit(1);
                        } else {
                            printf("정상:이전에 선언된 함수의 인자와 같은 인자로 호출했습니다.\n");
                        }
                    }
                }
}


Node * get_type_tree_from_var_declr(Node * declr_node, Node * ident_node) {
    // return_type-param1_type-param2_type-param3_type....
    Node * type_tree = node_maker(NULL, NULL, declr_node->son->son->brother->brother->token.token_number, 0);

    return type_tree;
}

Node * get_type_tree_from_param_var_declr(Node * declr_node, Node * ident_node) {
    // return_type-param1_type-param2_type-param3_type....
    Node * type_tree = node_maker(NULL, NULL, declr_node->son->token.token_number, 0);

    return type_tree;
}

Node * get_type_tree_from_func_declr(Node * declr_node, Node * ident_node) {
    // return_type-param1_type-param2_type-param3_type....
    Node * type_tree = node_maker(node_maker(NULL, NULL, declr_node->son->son->brother->brother->token.token_number, 0), NULL, SEM_TYPE, 0);
    Node * type_tree_current = type_tree;
    Node * param_node = ident_node->brother->son;
    while (param_node != NULL) {
        Node * param_node_son_holder = param_node->son;
        Node * param_node_type_holder = node_maker(NULL, NULL, param_node_son_holder->token.token_number, param_node_son_holder->token.token_value);
        param_node_son_holder = param_node_son_holder->brother;
        while (param_node_son_holder != NULL && param_node_son_holder->token.token_number != IDENT) {
        param_node_type_holder->brother = node_maker(NULL, NULL, param_node_son_holder->token.token_number, param_node_son_holder->token.token_value);
            param_node_son_holder = param_node_son_holder->brother;
            param_node_type_holder = param_node_type_holder->brother;
        }

        type_tree_current->brother = node_maker(param_node_type_holder, NULL, SEM_TYPE, 0);
        type_tree_current = type_tree_current->brother;

        param_node = param_node->brother;
    }

    return type_tree;
}

Node * get_type_tree_from_func_call(Node * ident_node) {
    // arg1_type-arg2type-arg3type...
    Node * param_node = ident_node->brother->son;
    Node * type_tree = node_maker(NULL, NULL, SEM_TYPE, 0);
    Node * type_tree_current = type_tree;

    if (param_node == NULL) {
        type_tree_current->son = node_maker(NULL, NULL, KW_VOID, 0);
    }else {
        while (param_node != NULL) {
            Node * param_return_type = NULL;
            Node * param_node_type = NULL;

            if (param_node->token.token_number == NT_EXP) {
                if (param_node->son->token.token_number == IDENT) {
                    if (symbol_finder_from_ident_node(param_node->son)->is_func == 1) {
                        param_return_type = symbol_finder_from_ident_node(param_node->son)->type_tree->son;
                    } else {
                        param_return_type = symbol_finder_from_ident_node(param_node->son)->type_tree;
                    }

                    param_node_type = param_return_type;
                } else {
                    param_node_type = node_maker(NULL, NULL, KW_INT, 0);
                }
                
            } else if (param_node->token.token_number == NUM_INT) {
                param_node_type = node_maker(NULL, NULL, KW_INT, 0);
            } else if (param_node->token.token_number == NUM_LONG) {
                param_node_type = node_maker(NULL, NULL, KW_LONG, 0);
            } else {
                printf("잘못된 노드가 func call 인자 체킹에 들어왔습니다: <%d, %d>\n", param_node->token.token_number, param_node->token.token_value);
                exit(1);
            }
            
            

            type_tree_current->brother = node_maker(param_node_type, NULL, SEM_TYPE, 0);
            type_tree_current = type_tree_current->brother;
            param_node = param_node->brother;
        }

        type_tree_current = type_tree->brother;
        free(type_tree);
    }

    return type_tree_current;

}



int value_cut_typesize(int input, Node * type_tree) {
    if ((type_tree->token.token_number == KW_INT || type_tree->token.token_number == KW_LONG) && type_tree->brother == NULL) {
        printf("인풋 값 %d를 32비트 사이즈로 자릅니다. 변환된 값: %d\n", input, (uint32_t)input);
        return (uint32_t)input;
    } else {
        printf("[경고] 잘못된 타입이 발생했을 수도 있습니다. value: %d\n", input);
        bin_tree_printer(type_tree);
        return input;
    }
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

    } else if (node->token.token_number == NT_VAR_DECLR) {    // NT_VAR_DECLR 구조: <DECLR>->
        int symbol_id = symbol_maker(node);
        Node * ident_node = node->son;

        while (ident_node->token.token_number != IDENT) {
            ident_node = get_brother(ident_node);
        }

        if ((ident_node->brother->son != NULL) && !(symbol_finder_from_symbol_id(symbol_id)->init_option == 1 || symbol_finder_from_symbol_id(symbol_id)->init_option == 2)) {    // 변수 선언 후 초기화하는 경우.
            Node * exp;
            Node * assign;
            Node * symbol;
            Node * right_val;
            Node * type_cleaner;
            
            right_val = ident_node->brother->son->brother;
            symbol = node_maker(node_maker(NULL, NULL, SEM_SYMBOL, symbol_id), right_val, NT_EXP, 0);
            assign = node_maker(NULL, symbol, OP_ASSIGN, 0);
            exp = node_maker(assign, NULL, NT_EXP, 0);
            // declr_to_content = node_maker(exp, node->brother, NT_CONTENT, 0);

            free(ident_node->brother->son);
            free(ident_node->brother);

            type_cleaner = node->son;
            tree_malloc_cleaner(type_cleaner->son);
            free(type_cleaner);
            free(ident_node);

            node->son = exp;
            node->token.token_number = NT_CONTENT;

            bin_tree_printer(node);
        } else {        // 변수 선언만 하는 경우. 심볼 등록만 하고 라인을 삭제한다.
            Node * type_cleaner;
            
            type_cleaner = node->son;
            tree_malloc_cleaner(type_cleaner->son);
            free(type_cleaner);

            tree_malloc_cleaner(ident_node->brother);
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
                node->token.token_number = NT_CONTENT;
                node->token.token_value = 0;
            }

            return;
        }
            

    } else if (node->token.token_number == NT_FUNC_DECLR) {    
        if (func_depth != 0 && node->son->son->token.token_value == 1) {
            printf("ERROR: 블록 스코프에서 static 함수를 선언하고 있습니다. 종료합니다.\n");
            exit(1);
        }
        int symbol_id = symbol_maker(node);
        func_depth++;
        Node * ident_node = node->son;

        while (ident_node->token.token_number != IDENT) {
            ident_node = get_brother(ident_node);
        }
        
        ident_node->token.token_number = SEM_SYMBOL;
        ident_node->token.token_value = symbol_id;

        push();

        Node * param_node = ident_node->brother->son;
        while (param_node != NULL && param_node->token.token_number == NT_PARAM_TYPE_LIST && param_node->son->token.token_number != KW_VOID) {
            int param_node_id = symbol_maker(param_node);

            Node * param_node_son = param_node->son;
            while (param_node_son != NULL) {
                Node * n = param_node_son->brother;
                free(param_node_son);   // 봐야함. tree_malloc_cleaner가 아닐까?
                param_node_son = n;
            }

            param_node->son = node_maker(NULL, NULL, SEM_SYMBOL, param_node_id);
            param_node = param_node->brother;
        }

        if (node->son->brother->brother->brother != NULL) {     // 정의가 있는 함수에서만 현재 함수 타입을 기록해, 리턴 타입을 함수에 맞추도록 한다.
            current_func_typetree = node->son->son->brother->brother;
        }
            

    } else if (node->token.token_number == IDENT) {
        int symbol_id = symbol_finder_from_ident_node(node)->id;

        node->token.token_number = SEM_SYMBOL;
        node->token.token_value = symbol_id;
        
    } else if (node->token.token_number == OP_PRE_INCRE || node->token.token_number == OP_PRE_DECRE) {
        if (node->brother->son->token.token_number != IDENT && node->brother->son->token.token_number != SEM_SYMBOL) {
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

    } else if (node->token.token_number == NUM_INT || node->token.token_number == NUM_LONG) {
        Node * type = node_maker(NULL, NULL, SEM_TYPE, 0);
        if (node->token.token_number == NUM_INT) {
            type->son = node_maker(NULL, NULL, KW_INT, 0);
        } else if (node->token.token_number == NUM_LONG) {
            type->son = node_maker(NULL, NULL, KW_LONG, 0);
        }
        
        Node * num = node_maker(NULL, NULL, node->token.token_number, node->token.token_value);
        
        node->son = type;
        node->token.token_number = NT_EXP;
        node->token.token_value = 0;

        type->brother = num;

    } else if (node->token.token_number == NT_EXP) {
        if ((node->son->token.token_number >= OP_ASSIGN && node->son->token.token_number <= OP_ASREQ) && node->son->brother->token.token_number != NT_EXP && node->son->brother->token.token_number != SEM_SYMBOL) {
        
            printf("ERROR: Symbol이 아닌 토큰 <%d, %d>에 값 대입중. 종료합니다.\n", node->son->brother->token.token_number, node->son->brother->token.token_value);
            exit(1);   // Symbol이 아닌 토큰에 값 대입중. 잘못된 표현식이므로 오류.
        } else if ((node->son->token.token_number >= OP_ASSIGN && node->son->token.token_number <= OP_ASREQ) && node->son->brother->token.token_number == NT_EXP && node->son->brother->son->brother->token.token_number != SEM_SYMBOL) {

            printf("ERROR: Symbol이 아닌 토큰 <%d, %d>에 값 대입중. 종료합니다.\n", node->son->brother->token.token_number, node->son->brother->token.token_value);
            exit(1);   // Symbol이 아닌 토큰에 값 대입중. 잘못된 표현식이므로 오류.
        }

        // NT_EXP에 타입 붙이기.
        if (node->son->token.token_number == SEM_SYMBOL) {
            Node * symbol = node_maker(node->son->son, node->son->brother, node->son->token.token_number, node->son->token.token_value);
            if (symbol_finder_from_symbol_id(symbol->token.token_value)->is_func == 1) {
                node->son->son = copy_tree(symbol_finder_from_symbol_id(symbol->token.token_value)->type_tree->son);
            } else {
                node->son->son = copy_tree(symbol_finder_from_symbol_id(symbol->token.token_value)->type_tree);
            }            
            node->son->brother = symbol;
            node->son->token.token_number = SEM_TYPE;
            node->son->token.token_value = 0;
            
        } else if (node->son->token.token_number == OP_TILDE || node->son->token.token_number == OP_NEG) {
            Node * type = node_maker(copy_tree(node->son->brother->son->son), NULL, SEM_TYPE, 0);
            type->brother = node->son;
            node->son = type;
            
        } else if (node->son->token.token_number == OP_LOGIC_NOT) {
            Node * type = node_maker(node_maker(NULL, NULL, KW_INT, 0), NULL, SEM_TYPE, 0);
            type->brother = node->son;
            node->son = type;
            
        } else if (node->son->token.token_number == OP_PRE_INCRE || node->son->token.token_number == OP_PRE_DECRE) {
            Node * type = node_maker(copy_tree(node->son->brother->son->son), NULL, SEM_TYPE, 0);
            type->brother = node->son;
            node->son = type;
            
        } else if (node->son->token.token_number == NT_CAST) {
            // 그대로
        } else if (node->son->token.token_number == OP_ADD || node->son->token.token_number == OP_SUB || node->son->token.token_number == OP_MUL || node->son->token.token_number == OP_DIV || node->son->token.token_number == OP_MOD || node->son->token.token_number == OP_AND || node->son->token.token_number == OP_OR || node->son->token.token_number == OP_XOR) {
            if (node->son->brother->son->son->token.token_number == KW_LONG && node->son->brother->brother->son->son->token.token_number == KW_INT) {
                Node * previous_exp = node->son->brother->brother;
                Node * cast = node_maker(node_maker(NULL, NULL, KW_LONG, 0), previous_exp, NT_CAST, 0);
                Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
                previous_exp->brother = NULL;
                node->son->brother->brother = exp;
            } else if (node->son->brother->son->son->token.token_number == KW_INT && node->son->brother->brother->son->son->token.token_number == KW_LONG) {
                Node * previous_exp = node->son->brother;
                Node * cast = node_maker(node_maker(NULL, NULL, KW_LONG, 0), previous_exp, NT_CAST, 0);
                Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
                previous_exp->brother = NULL;
                node->son->brother = exp;
            }
            Node * type = node_maker(copy_tree(node->son->brother->son->son), NULL, SEM_TYPE, 0);
            type->brother = node->son;
            node->son = type;
            
        } else if (node->son->token.token_number == OP_SHL || node->son->token.token_number == OP_ASR) {
            Node * type = node_maker(copy_tree(node->son->brother->son->son), NULL, SEM_TYPE, 0);
            type->brother = node->son;
            node->son = type;
            
        } else if (node->son->token.token_number == OP_EQ || node->son->token.token_number == OP_NE || node->son->token.token_number == OP_LT || node->son->token.token_number == OP_GT || node->son->token.token_number == OP_LE || node->son->token.token_number == OP_GE) {
            if (node->son->brother->son->son->token.token_number == KW_LONG && node->son->brother->brother->son->son->token.token_number == KW_INT) {
                Node * previous_exp = node->son->brother->brother;
                Node * cast = node_maker(node_maker(NULL, NULL, KW_LONG, 0), previous_exp, NT_CAST, 0);
                Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
                previous_exp->brother = NULL;
                node->son->brother->brother = exp;
            } else if (node->son->brother->son->son->token.token_number == KW_INT && node->son->brother->brother->son->son->token.token_number == KW_LONG) {
                Node * previous_exp = node->son->brother;
                Node * cast = node_maker(node_maker(NULL, NULL, KW_LONG, 0), previous_exp, NT_CAST, 0);
                Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
                previous_exp->brother = NULL;
                node->son->brother = exp;
            }
            Node * type = node_maker(node_maker(NULL, NULL, KW_INT, 0), NULL, SEM_TYPE, 0);
            type->brother = node->son;
            node->son = type;
            
        } else if (node->son->token.token_number == OP_LOGIC_AND || node->son->token.token_number == OP_LOGIC_OR) {
            Node * type = node_maker(node_maker(NULL, NULL, KW_INT, 0), NULL, SEM_TYPE, 0);
            type->brother = node->son;
            node->son = type;
            
        } else if (node->son->token.token_number == OP_ASSIGN) {
            Node * type = node_maker(copy_tree(node->son->brother->son->son), NULL, SEM_TYPE, 0);
            if (node->son->brother->son->son->token.token_number == KW_LONG && node->son->brother->brother->son->son->token.token_number == KW_INT) {
                Node * previous_exp = node->son->brother->brother;
                Node * cast = node_maker(node_maker(NULL, NULL, KW_LONG, 0), previous_exp, NT_CAST, 0);
                Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
                previous_exp->brother = NULL;
                node->son->brother->brother = exp;
            } else if (node->son->brother->son->son->token.token_number == KW_INT && node->son->brother->brother->son->son->token.token_number == KW_LONG) {
                Node * previous_exp = node->son->brother->brother;
                Node * cast = node_maker(node_maker(NULL, NULL, KW_INT, 0), previous_exp, NT_CAST, 0);
                Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
                previous_exp->brother = NULL;
                node->son->brother->brother = exp;
            }
            type->brother = node->son;
            node->son = type;
            
        } else if (node->son->token.token_number == OP_QUESTION) {
            if (node->son->brother->brother->son->son->token.token_number == KW_LONG && node->son->brother->brother->brother->son->son->token.token_number == KW_INT) {
                Node * previous_exp = node->son->brother->brother->brother;
                Node * cast = node_maker(node_maker(NULL, NULL, KW_LONG, 0), previous_exp, NT_CAST, 0);
                Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
                previous_exp->brother = NULL;
                node->son->brother->brother->brother = exp;
            } else if (node->son->brother->brother->son->son->token.token_number == KW_INT && node->son->brother->brother->brother->son->son->token.token_number == KW_LONG) {
                Node * previous_exp = node->son->brother->brother;
                Node * cast = node_maker(node_maker(NULL, NULL, KW_LONG, 0), previous_exp, NT_CAST, 0);
                Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
                previous_exp->brother = NULL;
                node->son->brother->brother = exp;
            }
            Node * type = node_maker(copy_tree(node->son->brother->brother->son->son), NULL, SEM_TYPE, 0);
            type->brother = node->son;
            node->son = type;
            
        }


        if (node->son->brother->token.token_number == SEM_SYMBOL) {
            if (node->son->brother->brother != NULL && node->son->brother->brother->token.token_number == NT_ARG_LIST) {
                func_typetree_validate(node->son->brother);
            }
        }

    } else if (node->token.token_number == NT_FUNC_DECLR) {    
        pop(); 
        func_depth--;
    } 
    






    // *** CALL BROTHER ***
    if (node->brother != NULL) {
        ident_symbolizer(node->brother);
    }


    // *** AFTER PROCESSING ***
    if (node->token.token_number == KW_RETURN) {
        if (compare_tree(node->brother->son->son, current_func_typetree) == 0) {
            Node * previous_exp = node->brother;
            Node * cast = node_maker(copy_tree(current_func_typetree), previous_exp, NT_CAST, 0);
            Node * exp = node_maker(cast, previous_exp->brother, NT_EXP, 0);
            previous_exp->brother = NULL;
            node->brother = exp;
        }
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
        label_table_queue[label_table_queue_top - 1] = realloc(label_table_queue[label_table_queue_top - 1], sizeof(Label_info*) * label_table_limit[label_table_queue_top - 1]);
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











/* *******************************************
**********************************************
**********************************************
***********                        ***********
***********    CASE TABLE STACK    ***********
***********                        ***********
**********************************************
**********************************************
******************************************* */

int case_table_list_limit;
int case_table_list_count;

int case_table_stack_limit;
int case_table_stack_count;

int * case_table_limit;
int * case_table_count;
int * case_table_stack_saving_for_list;

Switch_info *** case_table_list;
Switch_info *** case_table_stack;

void push_switch() {
    printf("Pushing Start. Current Case Table stack count: %d\n", case_table_stack_count);
    case_table_limit[case_table_stack_count] = 8;
    case_table_count[case_table_stack_count] = 0;
    Switch_info ** case_table = malloc(sizeof(Switch_info*) * case_table_limit[case_table_stack_count]);
    case_table_stack[case_table_stack_count++] = case_table;
    
    if (case_table_stack_count == case_table_stack_limit) {
        case_table_stack_limit = case_table_stack_limit * 2;
        case_table_stack = realloc(case_table_stack, sizeof(Switch_info**) * case_table_stack_limit);
        memset((char*)case_table_stack + (case_table_stack_limit / 2) * sizeof(Switch_info**), 0, (case_table_stack_limit / 2) * sizeof(Switch_info**));
        
        case_table_limit = realloc(case_table_limit, sizeof(int) * case_table_stack_limit);
        case_table_count = realloc(case_table_count, sizeof(int) * case_table_stack_limit);
    }

    printf("Pushing Finish. Current Table stack count: %d.\n", case_table_stack_count);
}

int pop_switch() {
    printf("Popping\n");
    case_table_stack_saving_for_list[case_table_list_count] = case_table_count[--case_table_stack_count];
    case_table_list[case_table_list_count++] = case_table_stack[case_table_stack_count];
    case_table_stack[case_table_stack_count] = NULL;
    
    if (case_table_list_count == case_table_list_limit) {
        case_table_list_limit = case_table_list_limit * 2;
        case_table_list = realloc(case_table_list, sizeof(Switch_info**) * case_table_list_limit);
        case_table_stack_saving_for_list = realloc(case_table_stack_saving_for_list, sizeof(Switch_info**) * case_table_list_limit);
    }

    if ((case_table_stack_count <= case_table_stack_limit / 4) && (case_table_stack_count > 0)) {
        case_table_stack_limit = case_table_stack_limit / 2;
        case_table_stack = realloc(case_table_stack, sizeof(Switch_info**) * case_table_stack_limit);
        
        case_table_limit = realloc(case_table_limit, sizeof(int) * case_table_stack_limit);
        case_table_count = realloc(case_table_count, sizeof(int) * case_table_stack_limit);
    }

    return case_table_list_count - 1;
}


int case_saver(int case_val) {

    printf("Saving Case Value %d\n", case_val);

    //case_val와 같은 값이 있는지 현재 테이블 순회해 검토. 있다면 오류 발생.
    for (int j = 0; j <= case_table_count[case_table_stack_count - 1] - 1; j++) {        // 테이블 내부 순회
        printf("DEBUG. j: %d\n", j);
        if ((case_val == case_table_stack[case_table_stack_count - 1][j]->name) && (case_table_stack[case_table_stack_count - 1][j]->is_default == 0)) {
            printf("오류: 이미 선언된 Case Value %d입니다. 종료합니다.\n", case_val);
            exit(1);
        }
    }

    printf("정상: 이전에 선언되지 않은 Case Value %d입니다. 계속 진행합니다.\n", case_val);

    Switch_info * case_label = malloc(sizeof(Switch_info));
    case_label->is_default = 0;
    case_label->name = case_val;
    case_label->id = label_id_count++;
    
    case_table_stack[case_table_stack_count - 1][case_table_count[case_table_stack_count - 1]++] = case_label;

    if (case_table_count[case_table_stack_count - 1] == case_table_limit[case_table_stack_count - 1]) {
        case_table_limit[case_table_stack_count - 1] = case_table_limit[case_table_stack_count - 1] * 2;
        case_table_stack[case_table_stack_count - 1] = realloc(case_table_stack[case_table_stack_count - 1], sizeof(Switch_info*) * case_table_limit[case_table_stack_count - 1]);
    }

    printf("Case Value %d를 ID %d로 Table %d에 저장했습니다.\n", case_table_stack[case_table_stack_count - 1][case_table_count[case_table_stack_count - 1] - 1]->name, case_table_stack[case_table_stack_count - 1][case_table_count[case_table_stack_count - 1] - 1]->id, case_table_stack_count - 1);

    return case_table_stack[case_table_stack_count - 1][case_table_count[case_table_stack_count - 1] - 1]->id;
}


int default_saver() {

    printf("Saving Default\n");

    //default가 이미 있는지 현재 테이블 순회해 검토. 있다면 오류 발생.
    for (int j = 0; j <= case_table_count[case_table_stack_count - 1] - 1; j++) {        // 테이블 내부 순회
        printf("DEBUG. j: %d\n", j);
        if (case_table_stack[case_table_stack_count - 1][j]->is_default == 1) {
            printf("오류: 한 개의 Switch 문 안에 두개 이상의 default가 사용되었습니다. 종료합니다.\n");
            exit(1);
        }
    }


    printf("정상: default가 사용된 적 없는 Switch 문입니다. 계속 진행합니다.\n");

    Switch_info * case_label = malloc(sizeof(Switch_info));
    case_label->is_default = 1;
    case_label->name = 0;
    case_label->id = label_id_count++;
    
    case_table_stack[case_table_stack_count - 1][case_table_count[case_table_stack_count - 1]++] = case_label;

    if (case_table_count[case_table_stack_count - 1] == case_table_limit[case_table_stack_count - 1]) {
        case_table_limit[case_table_stack_count - 1] = case_table_limit[case_table_stack_count - 1] * 2;
        case_table_stack[case_table_stack_count - 1] = realloc(case_table_stack[case_table_stack_count - 1], sizeof(Switch_info*) * case_table_limit[case_table_stack_count - 1]);
    }

    printf("Default를 ID %d로 Table %d에 저장했습니다.\n", case_table_stack[case_table_stack_count - 1][case_table_count[case_table_stack_count - 1] - 1]->id, case_table_stack_count - 1);

    return case_table_stack[case_table_stack_count - 1][case_table_count[case_table_stack_count - 1] - 1]->id;
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




int inside_func;

int can_break = 0;
int can_continue = 0;



void label_name_checker (Node * node) {
    printf("label_name_checker Start: Node <%d, %d>\n", node->token.token_number, node->token.token_value);


    // *** IN ***
    if (node->token.token_number == NT_FUNC_DECLR) {
        inside_func++;
        enqueue();

    } else if (node->token.token_number == KW_WHILE || node->token.token_number == KW_DO || node->token.token_number == KW_FOR || node->token.token_number == KW_SWITCH) {
        if (node->token.token_number != KW_SWITCH) {
            can_continue++;
        }

        can_break++;
        
    } else if (node->token.token_number == NT_LABEL) {
        if (inside_func == 1) {
            int label_id = label_name_saver(node);
            node->token.token_value = label_id;
        } else {
            printf("오류! 함수 바깥에서 라벨이 발견되었습니다. 종료합니다: <%d, %d>\n", node->token.token_number, node->token.token_value);
            exit(1);
        }
    } else if (node->token.token_number == KW_BREAK) {
        if (can_break < 1) {
            printf("오류! 사용할 수 없는 위치에서 BREAK가 발견되었습니다. 종료합니다.\n");
            exit(1);
        }
    } else if (node->token.token_number == KW_CONTINUE) {
        if (can_continue < 1) {
            printf("오류! 사용할 수 없는 위치에서 CONTINUE가 발견되었습니다. 종료합니다.\n");
            exit(1);
        }
    }


    // *** CALL SON ***
    if (node->son != NULL) {
        label_name_checker(node->son);
    }


    // *** OUT ***
    if (node->token.token_number == NT_FUNC_DECLR) {
        inside_func--;

    } else if (node->token.token_number == KW_WHILE || node->token.token_number == KW_DO || node->token.token_number == KW_FOR || node->token.token_number == KW_SWITCH) {
        if (node->token.token_number != KW_SWITCH) {
            can_continue--;
        }

        can_break--;
        
    }


    // *** CALL BROTHER ***
    if (node->brother != NULL) {
        label_name_checker(node->brother);
    }
}



void goto_name_checker (Node * node) {
    printf("goto_name_checker Start: Node <%d, %d>\n", node->token.token_number, node->token.token_value);


    // *** IN ***
    if (node->token.token_number == NT_FUNC_DECLR) {
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
    if (node->token.token_number == NT_FUNC_DECLR) {
        dequeue();
        inside_func--;
    }


    // *** CALL BROTHER ***
    if (node->brother != NULL) {
        goto_name_checker(node->brother);
    }
}




/**/

// int값을 받아 저장해두는 테이블의 스택이면 충분할듯. ID는 안됨->value를 보고 결정해야 하기에. 네임만 받아서 현재 테이블 상의 중복만 체크.
// int malloc의 malloc






///


int switch_depth = 0;

void switch_checker (Node * node) {
    printf("switch_checker Start: Node <%d, %d>\n", node->token.token_number, node->token.token_value);


    // *** IN ***
    if (node->token.token_number == KW_SWITCH) {
        switch_depth++;
        push_switch();

    } else if (node->token.token_number == KW_CASE) {
        if (switch_depth >= 1) {
            int case_val = case_calculator(node->son->son->brother);
            int case_id = case_saver(case_val);
            node->token.token_value = case_id;

            tree_malloc_cleaner(node->son);
            node->son = NULL;

        } else {
            printf("오류! SWITCH 바깥에서 CASE가 발견되었습니다. 종료합니다: <%d, %d>\n", node->token.token_number, node->token.token_value);
            exit(1);
        }
    } else if (node->token.token_number == KW_DEFAULT) {
        if (switch_depth >= 1) {
            int default_id = default_saver();
            node->token.token_value = default_id;
        } else {
            printf("오류! SWITCH 바깥에서 DEFAULT가 발견되었습니다. 종료합니다: <%d, %d>\n", node->token.token_number, node->token.token_value);
            exit(1);
        }
    }


    // *** CALL SON ***
    if (node->son != NULL) {
        switch_checker(node->son);
    }


    // *** OUT ***
    if (node->token.token_number == KW_SWITCH) {
        switch_depth--;
        int case_table_num = pop_switch();
        node->token.token_value = case_table_num;
    }


    // *** CALL BROTHER ***
    if (node->brother != NULL) {
        switch_checker(node->brother);
    }
}




int case_calculator (Node * node) {
    if (node->token.token_number == NUM_INT || node->token.token_number == NUM_LONG) {
        return node->token.token_value;
    } else {
        printf("오류: case의 값으로 동적 exp가 들어왔습니다: <%d, %d>\n", node->token.token_number, node->token.token_value);
        exit(1);
    }
}



void label_connector (Node * node) {
    printf("라벨 네임 체킹을 시작합니다.\n");
    inside_func = 0;
    label_name_checker(node);
    printf("라벨 네임 체킹을 완료했습니다.\n고투 네임 체킹을 시작합니다.\n");
    inside_func = 0;
    goto_name_checker(node);
    printf("고투 네임 체킹을 완료했습니다.\n스위치 체킹을 시작합니다.\n");
    switch_checker(node);

}







/* *********************************************
************************************************
************************************************
***********                          ***********
***********    SYMBOL TABLE STACK    ***********
***********                          ***********
************************************************
************************************************
********************************************* */

int reverting_compound_assign_amount = OP_ADDEQ - OP_ADD;

void reverting_compound_assign(Node * node) {
    // *** IN ***
    if (node->token.token_number >= OP_ADDEQ && node->token.token_number <= OP_ASREQ) {
        if (node->brother->son == NULL) {
            printf("ERROR: Symbol이 아닌 토큰 <%d, %d>에 값 대입중. 종료합니다.\n", node->brother->token.token_number, node->brother->token.token_value);
            exit(1);
        }
        Node * cal = node_maker(NULL, NULL, NT_EXP, 0);
        Node * op = node_maker(NULL, NULL, node->token.token_number - reverting_compound_assign_amount, 0);
        Node * val = node_maker(copy_tree(node->brother->son), node->brother->brother, NT_EXP, 0);

        cal->son = op;
        op->brother = val;

        node->token.token_number = OP_ASSIGN;
        node->brother->brother = cal;
    }
    


    // *** CALL SON ***
    if (node->son != NULL) {
        reverting_compound_assign(node->son);
    }


    // *** OUT ***
    


    // *** CALL BROTHER ***
    if (node->brother != NULL) {
        reverting_compound_assign(node->brother);
    }
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
    symbol_table_list_inside_count = calloc(symbol_table_list_limit, sizeof(int));

    symbol_table_limit = malloc(sizeof(int) * symbol_table_stack_limit);
    symbol_table_count = malloc(sizeof(int) * symbol_table_stack_limit);

    func_table_limit = 4;
    func_table_count = 0;
    func_table = malloc(sizeof(Symbol_info*) * func_table_limit);

    label_table_queue_limit = 4;
    label_table_queue_top = 0;
    label_table_queue_base = 0;
    label_table_queue = calloc(label_table_queue_limit, sizeof(Label_info**));
    label_table_limit = malloc(sizeof(int) * label_table_queue_limit);
    label_table_count = malloc(sizeof(int) * label_table_queue_limit);

    case_table_list_limit = 8;
    case_table_list_count = 0;
    case_table_stack_limit = 4;
    case_table_stack_count = 0;
    case_table_limit = malloc(sizeof(int) * case_table_stack_limit);
    case_table_count = malloc(sizeof(int) * case_table_stack_limit);
    case_table_stack_saving_for_list = malloc(sizeof(int) * case_table_list_limit);
    case_table_list = calloc(case_table_list_limit, sizeof(Switch_info**));
    case_table_stack = calloc(case_table_stack_limit, sizeof(Switch_info**));

    
    Node * symbolized_ast;


    FILE *symbastfp = fopen(symbast_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (symbastfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Semantic analyze 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", symbast_name);
        exit(1);
    }


    reverting_compound_assign(parse_input);
    symbolized_ast = parse_input;
    push();
    ident_symbolizer(symbolized_ast);
    pop();
    printf("Ident Symbolizer가 정상적으로 완료되었습니다.\n");

    label_connector(symbolized_ast);
    printf("Label Connector가 정상적으로 완료되었습니다.\n");

    bin_tree_printer(symbolized_ast);
    bin_tree_file_printer(symbolized_ast, symbastfp);

    printf("Semantic analyzing Finished.\n");
    fclose(symbastfp);
    return symbolized_ast;
}
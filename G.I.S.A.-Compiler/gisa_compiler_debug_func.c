#include "gisa_compiler.h"


void lexer_result_printer(Lexer_result lexer_result){

    printf("\n\n\n*** Lexing result test start ***\n");

    printf("lexeme count = %d\n", lexer_result.lexeme_count);
    for (int i = 0; i < lexer_result.lexeme_count; i++) {
        printf("lexeme number %d: <%d, %d>\n", i, lexer_result.lexeme_list[i].token_number, lexer_result.lexeme_list[i].token_value);
    }

    printf("\nvalue table count = %d\n", lexer_result.value_count);
    for (int i = 0; i < lexer_result.value_count; i++) {
        printf("value table number %d: <%s, %d>\n", i, lexer_result.value_table[i].lexeme_name, lexer_result.value_table[i].name_number);
    }

    printf("*** Lexing result test finish ***\n\n\n");
}


void node_printer(Node * node, int depth){
    for (int i = 0; i < depth; i++){
        printf("\t");
    }
    printf("<%d, %d>\n", node->token.token_number, node->token.token_value);
    
    if (node->son != NULL) {
        int new_depth = depth + 1;
        node_printer(node->son, new_depth);
    }
    
    if (node->brother != NULL) {
        node_printer(node->brother, depth);
    }
}


void bin_tree_printer(Node * tree_top){

    printf("\n\n\n*** Binary tree print start ***\n");

    int depth = 0;
    node_printer(tree_top, depth);

}


void node_file_printer(Node * node, int depth, FILE *parsefp){
    for (int i = 0; i < depth; i++){
        fprintf(parsefp, "\t");
    }
    fprintf(parsefp, "<%d, %d>\n", node->token.token_number, node->token.token_value);
    
    if (node->son != NULL) {
        int new_depth = depth + 1;
        node_file_printer(node->son, new_depth, parsefp);
    }
    
    if (node->brother != NULL) {
        node_file_printer(node->brother, depth, parsefp);
    }
}


void bin_tree_file_printer(Node * tree_top, FILE *parsefp){

    int depth = 0;
    node_file_printer(tree_top, depth, parsefp);

}


void tree_malloc_cleaner(Node * node) {
    if (node->son != NULL) {
        tree_malloc_cleaner(node->son);
    }
    
    if (node->brother != NULL) {
        tree_malloc_cleaner(node->brother);
    }

    free(node);
}


Node * node_maker(Node * n_son, Node * n_brother, int n_num, int n_val) {
    Node * n = malloc(sizeof(Node));
    
    n->son = n_son;        
    n->brother = n_brother;
    n->token.token_number = n_num;
    n->token.token_value = n_val;

    return n;
}


Node * line_maker(int line_type, int p1_num, int p1_val, int p2_num, int p2_val, int p3_num, int p3_val) {
    Node * n = malloc(sizeof(Node));

    Node * x1 = malloc(sizeof(Node));
    Node * x2 = malloc(sizeof(Node));
    Node * x3 = malloc(sizeof(Node));
    Node * x4 = malloc(sizeof(Node));

    x1->son = NULL;     
    x1->brother = x2;
    x1->token.token_number = line_type;
    x1->token.token_value = 0;

    x2->son = NULL;     
    x2->brother = x3;
    x2->token.token_number = p1_num;
    x2->token.token_value = p1_val;

    x3->son = NULL;     
    x3->brother = x4;
    x3->token.token_number = p2_num;
    x3->token.token_value = p2_val;

    x4->son = NULL;     
    x4->brother = NULL;
    x4->token.token_number = p3_num;
    x4->token.token_value = p3_val;
        

    n->son = x1;        
    n->brother = NULL;
    n->token.token_value = 0;

    if ((line_type >= 200 && line_type < 300) || (line_type >= 400 && line_type < 500)) {
        n->token.token_number = ASM_LINE;
    } else {
        n->token.token_number = TAG_LINE;
    }

    return n;
}
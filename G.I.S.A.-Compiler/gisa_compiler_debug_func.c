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
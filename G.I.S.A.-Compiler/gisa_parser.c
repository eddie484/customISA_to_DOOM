/*
    <program> ::= <function>
    <function> ::= "int" IDENT "(" <param> ")" "{" <content> "}"
    <param> ::= "void"
    <content> ::= "return" <exp> ";"
    <exp> ::= NUM_INT

    <program> ::= <function>
    <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    <param> ::= "KW_VOID"
    <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    <exp> ::= NUM_INT

    <program> ::= <function>
    <function> ::= 2 0 5 <param> 6 7 <content> 8
    <param> ::= 3
    <content> ::= 4 <exp> 9
    <exp> ::= 1

    ---

    FIRST(<program>) = {2}
    FIRST(<function>) = {2}
    FIRST(<param>) = {3}
    FIRST(<content>) = {4}
    FIRST(<exp>) = {1}

    FOLLOW(<program>) = {$}
    FOLLOW(<function>) = {$}
    FOLLOW(<param>) = {6}
    FOLLOW(<content>) = {8}
    FOLLOW(<exp>) = {9}

    ---

    *** removed token list ***
    OPEN_PAREN
    CLOSE_PAREN
    OPEN_BRACE
    CLOSE_BRACE
    PN_SEMI

    *** ast rules ***
    <program> ::= <function>
    <function> ::= "KW_INT" IDENT "KW_VOID" <content>
    <content> ::= "KW_RETURN" NUM_INT

*/



#include "gisa_compiler.h"



int symbolCount = 0;
Lexeme nextSymbol;

int token_type[1024];          // 읽어온 lexeme들의 타입을 저장. 
int token_value[1024];          // 읽어온 lexeme들의 값을 저장. 
int cur_line = 0;


Node * p_nt_program();
Node * p_nt_function();
Node * p_nt_param();
Node * p_nt_content();
Node * p_nt_exp();



void get_nextSymbol(Lexer_result lex_input){
    nextSymbol = lex_input.lexeme_list[symbolCount];
    printf("next symbol: %d\n", nextSymbol.token_number);
    symbolCount++;
    //printf("SymbolCount = %d, token_count = %d\n", symbolCount, lex_input.lexeme_count);
    if (symbolCount > (lex_input.lexeme_count + 1)) {
        printf("Parse Error: 제공된 lexeme보다 많은 symbol을 호출하고 있습니다.\n");
        exit(1);
    }
}

void error(int predLex, int givenLex){
    printf("Parse Error: lexeme %d가 주어져야 할 차례지만, 제공된 lexeme는 %d입니다.\n", predLex, givenLex);//: %d번 lexeme <%d, %d>를 파싱하는 과정에서 에러 발생");
    exit(1);
}

Node * p_t_IDENT(Lexer_result lex_input){
    if (nextSymbol.token_number == IDENT){
        printf("parsing: IDENT\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(0, nextSymbol.token_number);
}

Node * p_t_NUM_INT(Lexer_result lex_input){
    if (nextSymbol.token_number == NUM_INT){
        printf("parsing: NUM_INT\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(1,nextSymbol.token_number);
}

Node * p_t_KW_INT(Lexer_result lex_input){
    if (nextSymbol.token_number == KW_INT){
        printf("parsing: KW_INT\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(2, nextSymbol.token_number);
}

Node * p_t_KW_VOID(Lexer_result lex_input){
    if (nextSymbol.token_number == KW_VOID){
        printf("parsing: KW_VOID\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(3, nextSymbol.token_number);
}

Node * p_t_KW_RETURN(Lexer_result lex_input){
    if (nextSymbol.token_number == KW_RETURN){
        printf("parsing: KW_RETURN\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(4, nextSymbol.token_number);
}

Node * p_t_OPEN_PAREN(Lexer_result lex_input){
    if (nextSymbol.token_number == OPEN_PAREN){
        printf("parsing: OPEN_PAREN\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(5, nextSymbol.token_number);
}

Node * p_t_CLOSE_PAREN(Lexer_result lex_input){
    if (nextSymbol.token_number == CLOSE_PAREN){
        printf("parsing: CLOSE_PAREN\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(6, nextSymbol.token_number);
}

Node * p_t_OPEN_BRACE(Lexer_result lex_input){
    if (nextSymbol.token_number == OPEN_BRACE){
        printf("parsing: OPEN_BRACE\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(7, nextSymbol.token_number);
}

Node * p_t_CLOSE_BRACE(Lexer_result lex_input){
    if (nextSymbol.token_number == CLOSE_BRACE){
        printf("parsing: CLOSE_BRACE\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(8, nextSymbol.token_number);
}

Node * p_t_PN_SEMI(Lexer_result lex_input){
    if (nextSymbol.token_number == PN_SEMI){
        printf("parsing: PN_SEMI\n");

        Node * n = malloc(sizeof(Node));
        n->token = nextSymbol;

        get_nextSymbol(lex_input);

        return n;
    }
    else error(9, nextSymbol.token_number);
}


Node * p_nt_program(Lexer_result lex_input){    // <program> ::= <function>
    if (nextSymbol.token_number == 2) {
        printf("parsing: nt_program\n");
        Node * x1 = p_nt_function(lex_input);
        
        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = NT_PROGRAM;

        return n;
    } else exit(1);
}

Node * p_nt_function(Lexer_result lex_input){   // <function> ::= "KW_INT" IDENT "OPEN_PAREN" <param> "CLOSE_PAREN" "OPEN_BRACE" <content> "CLOSE_BRACE"
    if (nextSymbol.token_number == 2) {
        printf("parsing: nt_function\n");
        Node * x1 = p_t_KW_INT(lex_input);
        Node * x2 = p_t_IDENT(lex_input);
        Node * x3 = p_t_OPEN_PAREN(lex_input);
        Node * x4 = p_nt_param(lex_input);
        Node * x5 = p_t_CLOSE_PAREN(lex_input);
        Node * x6 = p_t_OPEN_BRACE(lex_input);
        Node * x7 = p_nt_content(lex_input);
        Node * x8 = p_t_CLOSE_BRACE(lex_input);

        x1->brother = x2;
        x2->brother = x4;
        x4->brother = x7;
        
        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = NT_FUNCTION;

        return n;
    } else exit(1);
}

Node * p_nt_param(Lexer_result lex_input){      // <param> ::= "KW_VOID"
    if (nextSymbol.token_number == 3) {
        printf("parsing: nt_param\n");
        Node * x1 = p_t_KW_VOID(lex_input);
        
        return x1;
    } else exit(1);
}

Node * p_nt_content(Lexer_result lex_input){    // <content> ::= "KW_RETURN" <exp> "PN_SEMI"
    if (nextSymbol.token_number == 4) {
        printf("parsing: nt_content\n");
        Node * x1 = p_t_KW_RETURN(lex_input);
        Node * x2 = p_nt_exp(lex_input);
        Node * x3 = p_t_PN_SEMI(lex_input);

        x1->brother = x2;
        
        Node * n = malloc(sizeof(Node));
        n->son = x1;
        n->token.token_number = NT_CONTENT;

        return n;
    } else exit(1);
}

Node * p_nt_exp(Lexer_result lex_input){        // <exp> ::= NUM_INT
    if (nextSymbol.token_number == 1) {
        printf("parsing: nt_exp\n");
        Node * x1 = p_t_NUM_INT(lex_input);
        
        return x1;
    } else exit(1);
}




/* **************************************
*****************************************
*****************************************
***********                   ***********
***********    MAIN PARSER    ***********
***********                   ***********
*****************************************
*****************************************
************************************** */

Node * parser(Lexer_result lex_input, char *parse_name)
{
    Node * ast_top = malloc(sizeof(Node));
    /*
    char *lex_name;
    char *parse_name;

        // 인자 개수 확인해 파일이름 지정.
        if (argc == 3) {
            lex_name = argv[1];
            parse_name = argv[2];
        } else {
            lex_name = "lexeme.lex";
            parse_name = "ast.parse";
        }
    */


    FILE *parsefp = fopen(parse_name, "w");         // 처리 결과 ast를 저장할 파일 오픈

    if (parsefp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("Parse 결과를 저장할 파일 읽기 실패. 시도한 파일명: %s\n", parse_name);
        exit(1);
    }

    
    //lexer_result_printer(lex_input);

    

    printf("\nLexeme 로딩 성공. Parsing 시작.\n\n");

    get_nextSymbol(lex_input);
    ast_top = p_nt_program(lex_input);


    bin_tree_file_printer(ast_top, parsefp);


    printf("Parsing Finished.\n");
    fclose(parsefp);

    bin_tree_printer(ast_top);

    return ast_top;
}
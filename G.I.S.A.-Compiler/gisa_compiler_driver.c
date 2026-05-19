#include "gisa_compiler.h"



int main(int argc, char *argv[]) {
    if (argc == 2 || argc == 3) {
        char *option;
        char *name;
        char *file_name_with_dot;
        char file_dir [128];
        char file_name [64];


        // 인자 개수 확인해 옵션 및 파일이름 지정.
        if (argc == 3) {
            option = argv[1];
            name = argv[2];
        } else {
            option = "--no-option";
            name = argv[1];
        }


        // 입력 파일의 경로와 이름 추출 시작.
        char *slash = strrchr(name, '/');
        char *dot = strrchr(name, '.');

        if (slash) {
            int slash_len = (slash - name) + 1;
            strncpy(file_dir, name, slash_len);
            file_dir[slash_len] = '\0';
            file_name_with_dot = slash + 1;
        } else {
            file_dir[0] = '\0';
            file_name_with_dot = name;
        }

        if (dot) {
            int dot_len = (dot - file_name_with_dot);
            strncpy(file_name, file_name_with_dot, dot_len);
            file_name[dot_len] = '\0';
        } else {
            strcpy(file_name, file_name_with_dot);
        }
        // 입력 파일의 경로와 이름 추출 끝.

        printf("option: \"%s\", full name: \"%s\", dir: \"%s\", file name: \"%s\".\n", option, name, file_dir, file_name);

        if (strcmp(option, "--lex") != 0 && strcmp(option, "--parse") != 0 && strcmp(option, "--tag") != 0 && strcmp(option, "--codegen") != 0 && strcmp(option, "-S") != 0 && strcmp(option, "--keep") != 0 && strcmp(option, "--no-option") != 0) {
            printf("잘못된 인자가 입력되었습니다.\n인자로는 \"파일명\" 혹은 \"옵션명 파일명\"만 사용 가능합니다.\n옵션: --lex, --parse, --tag --codegen, -S, --keep\n");
            return 1;
        }

        // ***** 각 단계별 생성될 파일명 지정 ****
        char preprocess_filename[256];
        char lex_filename[256];
        char parse_filename[256];
        char tag_filename[256];
        char asmtreegen_filename[256];
        char asmemit_filename[256];
        char binary_filename[256];

        snprintf(preprocess_filename, sizeof(preprocess_filename), "%sa_PREPROCESSED_%s.i", file_dir, file_name);
        snprintf(lex_filename, sizeof(lex_filename), "%sb_LEXEME_%s.lex", file_dir, file_name);
        snprintf(parse_filename, sizeof(parse_filename), "%sc_AST_%s.parse", file_dir, file_name);
        snprintf(tag_filename, sizeof(tag_filename), "%sd_TAG_%s.tag", file_dir, file_name);
        snprintf(asmtreegen_filename, sizeof(asmtreegen_filename), "%se_ASMTREE_%s.codegen", file_dir, file_name);
        snprintf(asmemit_filename, sizeof(asmemit_filename), "%sf_ASSEMBLY_%s.asm", file_dir, file_name);
        snprintf(binary_filename, sizeof(preprocess_filename), "%sg_BINART_%s.bin", file_dir, file_name);


        // ***** 전처리 진행 *****
        printf("Start Preprocessing.\n");

        char preprocessing_cmd[1024];

        snprintf(preprocessing_cmd, sizeof(preprocessing_cmd), "gcc -E -P %s -o %s", name, preprocess_filename);
        if (system(preprocessing_cmd) != 0) return 2;

        printf("Preprocessing is finished.\n");



        // ***** Lexing *****
        printf("Start Lexing.\n\n");

        Lexer_result lexer_result;      // Lexer의 결과로 나온 lexeme list와 value table을 저장하는 구조체.
        lexer_result = lexer(preprocess_filename, lex_filename);
        
        printf("\nLexing is finished. Remove Preprocessed file.\n");
        
        //lexer_result_printer(lex_input);

        if (strcmp(option, "--keep") != 0) {
            char remove_preprocess_file_cmd[1024];
            snprintf(remove_preprocess_file_cmd, sizeof(remove_preprocess_file_cmd), "rm %s", preprocess_filename);
            if (system(remove_preprocess_file_cmd) != 0) return 4;
        }


        if (strcmp(option, "--lex") == 0) {
            printf("Option --lex: Lexing finished. Program exit.\n");

            free(lexer_result.lexeme_list);
            free(lexer_result.value_table);

            return 0;
        }



        // ***** Parsing *****
        printf("Start Parsing.\n\n");

        Node * parser_result;
        parser_result = parser(lexer_result, parse_filename);
        
        printf("\nParsing is finished.\n");

        if (strcmp(option, "--keep") != 0) {
            char remove_lex_file_cmd[1024];
            snprintf(remove_lex_file_cmd, sizeof(remove_lex_file_cmd), "rm %s", lex_filename);
            if (system(remove_lex_file_cmd) != 0) return 4;
        }

        
        if (strcmp(option, "--parse") == 0) {
            printf("Option --parse: Parsing finished. Program exit.\n");

            free(lexer_result.lexeme_list);
            free(lexer_result.value_table);
            tree_malloc_cleaner(parser_result);

            return 0;
        }



        // ***** TAG Generating *****
        printf("Start TAG generating.\n\n");

        Node * tag_result;
        tag_result = tag_generator(parser_result, tag_filename);
        
        printf("TAG Generating is finished.\n");

        if (strcmp(option, "--keep") != 0) {
            char remove_parse_file_cmd[1024];
            snprintf(remove_parse_file_cmd, sizeof(remove_parse_file_cmd), "rm %s", parse_filename);
            if (system(remove_parse_file_cmd) != 0) return 4;
        }

        
        if (strcmp(option, "--tag") == 0) {
            printf("Option --tag: Parsing finished. Program exit.\n");

            free(lexer_result.lexeme_list);
            free(lexer_result.value_table);
            tree_malloc_cleaner(parser_result);
            tree_malloc_cleaner(tag_result);


            return 0;
        }



        // ***** Code Generating *****
        printf("Start Code Generating.\n\n");

        Node * codegen_result;
        codegen_result = code_generator(tag_result, asmtreegen_filename);
        
        printf("Code Generating is finished.\n");

        if (strcmp(option, "--keep") != 0) {
            char remove_tag_file_cmd[1024];
            snprintf(remove_tag_file_cmd, sizeof(remove_tag_file_cmd), "rm %s", tag_filename);
            if (system(remove_tag_file_cmd) != 0) return 4;
        }
        
        
        if (strcmp(option, "--codegen") == 0) {
            printf("Option --codegen: Code Generating finished. Program exit.\n");

            free(lexer_result.lexeme_list);
            free(lexer_result.value_table);
            tree_malloc_cleaner(parser_result);
            tree_malloc_cleaner(tag_result);
            tree_malloc_cleaner(codegen_result);

            return 0;
        }



        // ***** Code Emission *****
        printf("Start Code Emission.\n");

        code_emitter(codegen_result, lexer_result, asmemit_filename);

        printf("Code Emission is finished.\n");

        if (strcmp(option, "--keep") != 0) {
            char remove_codegen_file_cmd[1024];
            snprintf(remove_codegen_file_cmd, sizeof(remove_codegen_file_cmd), "rm %s", asmtreegen_filename);
            if (system(remove_codegen_file_cmd) != 0) return 4;
        }
        
        
        if (strcmp(option, "-S") == 0) {
            printf("Option -S: Code Emission finished. Program exit.\n");

            free(lexer_result.lexeme_list);
            free(lexer_result.value_table);
            tree_malloc_cleaner(parser_result);
            tree_malloc_cleaner(tag_result);
            tree_malloc_cleaner(codegen_result);

            return 0;
        }


        // ***** Assemble *****
        printf("Start Assembling.\n");

        char assembling_cmd[1024];
        
        snprintf(assembling_cmd, sizeof(assembling_cmd), "./gisa_assembler %s %s", asmemit_filename, binary_filename);
        if (system(assembling_cmd) != 0) return 8;

        
        printf("Assembling is finished. Remove Assembly file.\n");

        if (strcmp(option, "--keep") != 0) {
            char remove_assembly_file_cmd[1024];
            snprintf(remove_assembly_file_cmd, sizeof(remove_assembly_file_cmd), "rm %s", asmemit_filename);
            if (system(remove_assembly_file_cmd) != 0) return 4;
        }
        
        printf("GISA compiler driver's every process is finished well. Program exit.\n");
        
        free(lexer_result.lexeme_list);
        free(lexer_result.value_table);
        tree_malloc_cleaner(parser_result);
        tree_malloc_cleaner(tag_result);
        tree_malloc_cleaner(codegen_result);
        

    } else {
        printf("입력된 인자 개수가 잘못되었습니다.\n인자로는 \"파일명\" 혹은 \"옵션명 파일명\"만 사용 가능합니다.\n옵션: --lex, --parse, --codegen, -S\n");
        return 1;
    }

    return 0;
}
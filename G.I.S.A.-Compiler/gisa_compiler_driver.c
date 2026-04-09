#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

        if (strcmp(option, "--lex") != 0 && strcmp(option, "--parse") != 0 && strcmp(option, "--codegen") != 0 && strcmp(option, "-S") != 0 && strcmp(option, "--no-option") != 0) {
            printf("잘못된 인자가 입력되었습니다.\n인자로는 \"파일명\" 혹은 \"옵션명 파일명\"만 사용 가능합니다.\n옵션: --lex, --parse, --codegen, -S\n");
            return 1;
        }


        // ***** 전처리 진행 *****
        printf("Start Preprocessing.\n");

        char preprocessing_cmd[1024];

        snprintf(preprocessing_cmd, sizeof(preprocessing_cmd), "gcc -E -P %s -o %sPREPROCESSED_%s.i", name, file_dir, file_name);
        if (system(preprocessing_cmd) != 0) return 2;

        printf("Preprocessing is finished.\n");



        // ***** Lexing *****
        printf("Start Lexing.\n");

        //asdf;
        
        printf("Lexing is finished. Remove Preprocessed file.\n");


        char remove_preprocess_file_cmd[1024];

        snprintf(remove_preprocess_file_cmd, sizeof(remove_preprocess_file_cmd), "rm %sPREPROCESSED_%s.i", file_dir, file_name);
        if (system(remove_preprocess_file_cmd) != 0) return 4;


        if (strcmp(option, "--lex") == 0) {
            printf("Option --lex: Lexing finished. Program exit.\n");
            return 0;
        }



        // ***** Parsing *****
        printf("Start Parsing.\n");

        //asdf;
        
        printf("Parsing is finished.\n");

        
        if (strcmp(option, "--parse") == 0) {
            printf("Option --parse: Parsing finished. Program exit.\n");
            return 0;
        }



        // ***** Code Generating *****
        printf("Start Code Generating.\n");

        //asdf;
        
        printf("Code Generating is finished.\n");
        
        
        if (strcmp(option, "--codegen") == 0) {
            printf("Option --codegen: Code Generating finished. Program exit.\n");
            return 0;
        }



        // ***** Code Emission *****
        printf("Start Code Emission.\n");

        //asdf;
        
        printf("Code Emission is finished.\n");
        
        
        if (strcmp(option, "-S") == 0) {
            printf("Option -S: Code Emission finished. Program exit.\n");
            return 0;
        }


        // ***** Assemble *****
        printf("Start Assembling.\n");

        char assembling_cmd[1024];

        snprintf(assembling_cmd, sizeof(assembling_cmd), "./gisa_assembler %sASSEMBLY_%s.s %s%s", file_dir, file_name, file_dir, file_name);
        if (system(assembling_cmd) != 0) return 8;

        
        printf("Assembling is finished. Remove Assembly file.\n");

        char remove_assembly_file_cmd[1024];

        snprintf(remove_assembly_file_cmd, sizeof(remove_assembly_file_cmd), "rm %sASSEMBLY_%s.s", file_dir, file_name);
        if (system(remove_assembly_file_cmd) != 0) return 9;
        
        printf("GISA compiler driver's every process is finished well. Program exit.\n");
        

    } else {
        printf("입력된 인자 개수가 잘못되었습니다.\n인자로는 \"파일명\" 혹은 \"옵션명 파일명\"만 사용 가능합니다.\n옵션: --lex, --parse, --codegen, -S\n");
        return 1;
    }

    return 0;
}
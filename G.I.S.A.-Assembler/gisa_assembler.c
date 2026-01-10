#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 나눗셈 연산 세 종류, 로테이트 연산 두 종류, 시스템 명령어들은 일단 처리되지 않음.
// b, jmp에서 라벨 받을 수 있도록 해야 함
// 즉시값 범위 초과 감지는 부호확장 확정한 뒤...

int main()
{
    FILE *fp = fopen("assembly.asm", "r");

    if (fp == NULL) {
        printf("파일 읽기 실패\n");
        return 1;
    }


    char buf[100];
    char **lines = NULL;
    char **original_lines = NULL;
    int count = 0;
    int cap = 10;

    lines = malloc(cap * sizeof(char *));
    original_lines = malloc(cap * sizeof(char *));

    while (fgets(buf, sizeof(buf), fp)) {

        if (strchr(buf, ':') != NULL) {
            printf("라벨 라인! %d줄!\n", count);
            continue;
        } else if (buf[0] == '\n') {
            printf("빈 라인! %d줄!\n", count);
            continue;
        }

        if (count >= cap) {
            cap *= 2;
            lines = realloc(lines, cap * sizeof(char *));
            original_lines = realloc(original_lines, cap * sizeof(char *));
        }
        lines[count] = malloc(strlen(buf) + 1);
        original_lines[count] = malloc(strlen(buf) + 1);
        strcpy(lines[count], buf);
        strcpy(original_lines[count], buf);

        printf("%d     %s", count, lines[count]);
        count++;
    }

    printf("\n\n--- 어셈블리 불러오기 완료! ---\n\n\n", count, lines[count]);



    for (int i = 0; i < count; i++) {
        printf("line %d\t", i);
        uint32_t binary = 0;
        char *token = strtok(lines[i], " \t\n");    // 첫 번째 토큰으로 opcode를 확인한다.

        if (strcmp(token, "MOV") == 0) {            // opcode가 MOV일 경우
            token = strtok(NULL, " \t\n");          // 두 번째 토큰으로 목적지 레지스터를 확인한다.

            if (token[0] != 'r') {
                printf("Line %d 명령어 %s의 첫 번째 인자로 레지스터가 입력되지 않았습니다: %s\n오류 명령어: %s\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            int rD = atoi(token + 1);               // 목적지 레지스터의 번호를 int로 변환

            if (rD > 15 || rD <1) {
                printf("Line %d 명령어 %s의 목적지 레지스터로 존재하지 않는 레지스터가 입력되었습니다: %s\n오류 명령어: %s\n범용 레지스터는 r0부터 r15까지 존재합니다.\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            binary |= (rD << 20);                   // 바이너리 코드의 rD 자리를 채운다.
            


            token = strtok(NULL, " \t\n");

            if (token[0] != 'r') {
                printf("Line %d 명령어 %s의 두 번째 인자로 레지스터가 입력되지 않았습니다: %s\n오류 명령어: %s\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            int rB = atoi(token + 1);

            if (rB > 15 || rB <1) {
                printf("Line %d 명령어 %s의 소스 레지스터로 존재하지 않는 레지스터가 입력되었습니다: %s\n오류 명령어: %s\n범용 레지스터는 r0부터 r15까지 존재합니다.\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            binary |= (rB << 12);

        } else if (strcmp(token, "MOVI") == 0) {    // opcode가 MOVI일 경우
            binary |= (0b00000010U << 24);          // 바이너리 코드의 opcode, i, s 자리를 채운다
            
            token = strtok(NULL, " \t\n");

            if (token[0] != 'r') {
                printf("Line %d 명령어 %s의 첫 번째 인자로 레지스터가 입력되지 않았습니다: %s\n오류 명령어: %s\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            int rD = atoi(token + 1);

            if (rD > 15 || rD <1) {
                printf("Line %d 명령어 %s의 목적지 레지스터로 존재하지 않는 레지스터가 입력되었습니다: %s\n오류 명령어: %s\n범용 레지스터는 r0부터 r15까지 존재합니다.\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            binary |= (rD << 20);
            


            token = strtok(NULL, " \t\n");

            if (token[0] != '#') {
                printf("Line %d 명령어 %s의 두 번째 인자로 즉시값이 입력되지 않았습니다: %s\n오류 명령어: %s\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MOVS") == 0) {
            binary |= (0b00000001U << 24);

            token = strtok(NULL, " \t\n");

            if (token[0] != 'r') {
                printf("Line %d 명령어 %s의 첫 번째 인자로 레지스터가 입력되지 않았습니다: %s\n오류 명령어: %s\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            int rD = atoi(token + 1);

            if (rD > 15 || rD <1) {
                printf("Line %d 명령어 %s의 목적지 레지스터로 존재하지 않는 레지스터가 입력되었습니다: %s\n오류 명령어: %s\n범용 레지스터는 r0부터 r15까지 존재합니다.\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            binary |= (rD << 20);
            


            token = strtok(NULL, " \t\n");

            if (token[0] != 'r') {
                printf("Line %d 명령어 %s의 두 번째 인자로 레지스터가 입력되지 않았습니다: %s\n오류 명령어: %s\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            int rB = atoi(token + 1);

            if (rB > 15 || rB <1) {
                printf("Line %d 명령어 %s의 소스 레지스터로 존재하지 않는 레지스터가 입력되었습니다: %s\n오류 명령어: %s\n범용 레지스터는 r0부터 r15까지 존재합니다.\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            binary |= (rB << 12);

        } else if (strcmp(token, "MOVIS") == 0) {
            binary |= (0b00000011U << 24);
            
            token = strtok(NULL, " \t\n");

            if (token[0] != 'r') {
                printf("Line %d 명령어 %s의 첫 번째 인자로 레지스터가 입력되지 않았습니다: %s\n오류 명령어: %s\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            int rD = atoi(token + 1);

            if (rD > 15 || rD <1) {
                printf("Line %d 명령어 %s의 목적지 레지스터로 존재하지 않는 레지스터가 입력되었습니다: %s\n오류 명령어: %s\n범용 레지스터는 r0부터 r15까지 존재합니다.\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            binary |= (rD << 20);
            


            token = strtok(NULL, " \t\n");

            if (token[0] != '#') {
                printf("Line %d 명령어 %s의 두 번째 인자로 즉시값이 입력되지 않았습니다: %s\n오류 명령어: %s\n어셈블러를 종료합니다.\n", i, lines[i], token, original_lines[i]);
                return 1;
            }

            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "ADD") == 0) {    // opcode가 ADD일 경우
            binary |= (0b00000100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ADDI") == 0) {
            binary |= (0b00000110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "ADDS") == 0) {
            binary |= (0b00000101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ADDIS") == 0) {
            binary |= (0b00000111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "SUB") == 0) {    // opcode가 SUB일 경우
            binary |= (0b00001000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "SUBI") == 0) {
            binary |= (0b00001010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "SUBS") == 0) {
            binary |= (0b00001001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "SUBIS") == 0) {
            binary |= (0b00001011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MOVH") == 0) {    // opcode가 MOVH일 경우
            binary |= (0b00001100U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MOVHL") == 0) {
            binary |= (0b00001110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MOVHS") == 0) {
            binary |= (0b00001101U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MOVHLS") == 0) {
            binary |= (0b00001111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "CMP") == 0) {    // opcode가 CMP일 경우
            binary |= (0b00010000U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "CMPI") == 0) {
            binary |= (0b00010010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "CMPS") == 0) {
            binary |= (0b00010001U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "CMPIS") == 0) {
            binary |= (0b00010011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MUL") == 0) {    // opcode가 MUL일 경우
            binary |= (0b00010100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MULI") == 0) {
            binary |= (0b00010110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MULS") == 0) {
            binary |= (0b00010101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MULIS") == 0) {
            binary |= (0b00010111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MULH") == 0) {    // opcode가 MULH일 경우
            binary |= (0b00011000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MULHI") == 0) {
            binary |= (0b00011010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MULHS") == 0) {
            binary |= (0b00011001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MULHIS") == 0) {
            binary |= (0b00011011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MULHU") == 0) {    // opcode가 MULHU일 경우
            binary |= (0b00011100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MULHUI") == 0) {
            binary |= (0b00011110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MULHUS") == 0) {
            binary |= (0b00011101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MULHUIS") == 0) {
            binary |= (0b00011111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MULFX") == 0) {    // opcode가 MULFX일 경우
            binary |= (0b00100000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MULFXI") == 0) {
            binary |= (0b00100010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "MULFXS") == 0) {
            binary |= (0b00100001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MULFXIS") == 0) {
            binary |= (0b00100011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "SHL") == 0) {    // opcode가 SHL일 경우
            binary |= (0b01100000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "SHLI") == 0) {
            binary |= (0b01100010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "SHLS") == 0) {
            binary |= (0b01100001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "SHLIS") == 0) {
            binary |= (0b01100011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "ASR") == 0) {    // opcode가 ASR일 경우
            binary |= (0b01100100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ASRI") == 0) {
            binary |= (0b01100110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "ASRS") == 0) {
            binary |= (0b01100101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ASRIS") == 0) {
            binary |= (0b01100111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LSR") == 0) {    // opcode가 LSR일 경우
            binary |= (0b01101000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "LSRI") == 0) {
            binary |= (0b01101010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LSRS") == 0) {
            binary |= (0b01101001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "LSRIS") == 0) {
            binary |= (0b01101011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "AND") == 0) {    // opcode가 AND인 경우
            binary |= (0b10000100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ANDI") == 0) {
            binary |= (0b10000110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "ANDS") == 0) {
            binary |= (0b10000101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ANDIS") == 0) {
            binary |= (0b10000111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "OR") == 0) {    // opcode가 OR인 경우
            binary |= (0b10001000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ORI") == 0) {
            binary |= (0b10001010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "ORS") == 0) {
            binary |= (0b10001001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ORIS") == 0) {
            binary |= (0b10001011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "XOR") == 0) {    // opcode가 XOR인 경우
            binary |= (0b10001100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "XORI") == 0) {
            binary |= (0b10001110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "XORS") == 0) {
            binary |= (0b10001101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "XORIS") == 0) {
            binary |= (0b10001111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "NOT") == 0) {    // opcode가 NOT인 경우
            binary |= (0b10010000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "NOTI") == 0) {
            binary |= (0b10010010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "NOTS") == 0) {
            binary |= (0b10010001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "NOTIS") == 0) {
            binary |= (0b10010011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "BCHK") == 0) {    // opcode가 BCHK인 경우
            binary |= (0b10010100U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "BCHKI") == 0) {
            binary |= (0b10010110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "BCHKS") == 0) {
            binary |= (0b10010101U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "BCHKIS") == 0) {
            binary |= (0b10010111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDR") == 0) {    // opcode가 LDR인 경우
            binary |= (0b10100000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRI") == 0) {
            binary |= (0b10100010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRR") == 0) {
            binary |= (0b10100001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRB") == 0) {    // opcode가 LDRB인 경우
            binary |= (0b10100100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRBI") == 0) {
            binary |= (0b10100110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRBR") == 0) {
            binary |= (0b10100101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRSB") == 0) {    // opcode가 LDRSB인 경우
            binary |= (0b10101000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRSBI") == 0) {
            binary |= (0b10101010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRSBR") == 0) {
            binary |= (0b10101001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRH") == 0) {    // opcode가 LDRH인 경우
            binary |= (0b10101100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRHI") == 0) {
            binary |= (0b10101110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRHR") == 0) {
            binary |= (0b10101101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRSH") == 0) {    // opcode가 LDRSH인 경우
            binary |= (0b10110000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRSHI") == 0) {
            binary |= (0b10110010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "LDRSHR") == 0) {
            binary |= (0b10110001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "STR") == 0) {    // opcode가 STR인 경우
            binary |= (0b10110100U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 20 | immBb);

        } else if (strcmp(token, "STRI") == 0) {
            binary |= (0b10110110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 16 | immBb);

        } else if (strcmp(token, "STRR") == 0) {
            binary |= (0b10110101U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 16 | immBb);

        } else if (strcmp(token, "STRB") == 0) {    // opcode가 STRB인 경우
            binary |= (0b10111000U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 20 | immBb);

        } else if (strcmp(token, "STRBI") == 0) {
            binary |= (0b10111010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 16 | immBb);

        } else if (strcmp(token, "STRBR") == 0) {
            binary |= (0b10111001U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 16 | immBb);

        } else if (strcmp(token, "STRH") == 0) {    // opcode가 STRH인 경우
            binary |= (0b10111100U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 20 | immBb);

        } else if (strcmp(token, "STRHI") == 0) {
            binary |= (0b10111110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 16 | immBb);

        } else if (strcmp(token, "STRHR") == 0) {
            binary |= (0b10111101U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= (immBf << 16 | immBb);

        } else if (strcmp(token, "BEQ") == 0) {    // opcode가 B인 경우
            binary |= (0b110010U << 26);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BNE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0001U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BHS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0010U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BLO") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0011U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BMI") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0100U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BPL") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0101U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BVS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0110U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BVC") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0111U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BHI") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1000U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BLS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1001U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BGE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1010U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BLT") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1011U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BGT") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1100U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BLE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1101U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "BAL") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1110U);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB << 4);

        } else if (strcmp(token, "JMP") == 0) {    // opcode가 JMP일 경우
            binary |= (0b11001100U << 24);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "JMPI") == 0) {
            binary |= (0b11001110U << 24);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "JMPL") == 0) {
            binary |= (0b11001101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else if (strcmp(token, "JMPIL") == 0) {
            binary |= (0b11001111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB);

        } else {
            printf ("Line %d에 정의되지 않은 명령어가 입력되었습니다: %s\n어셈블러를 종료합니다.\n", i, token);
            return 1;
        }


            for (int i = 31; i >=0; i--) {      // 이진 코드 출력
                printf("%d", (binary >> i) & 1);
                if (i % 4 == 0 && i != 0) printf("_");
            }
            printf("\n");

    }


    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);


    fclose(fp);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char name[50];
    uint32_t line;
} Label;

Label labels [100];
int labelcount = 0;

void labelput(char *labelname, uint32_t labelline){
    strcpy(labels[labelcount].name, labelname);
    labels[labelcount].line = labelline;

    printf("라벨 저장. 라벨 카운트: %d, 라벨 네임: %s, 라벨 라인: %d.\n", labelcount, labelname, labelline);

    labelcount++;
}

uint32_t labelget(char *labelname){
    int count = 0;
    while(count < labelcount){
        if (strcmp(labelname, labels[count].name) == 0){
            return labels[count].line;
        } else {
            count++;
        }
    }
    printf("선언되지 않은 라벨입니다: %s\n", labelname);
    exit(1);
}

// b, jmp에서 라벨 받을 수 있도록 해야 함. 아직 라벨처리 미지원.

int main()
{
    FILE *asmfp = fopen("assembly.asm", "r");
    FILE *binfp = fopen("binary.bin", "w");

    if (asmfp == NULL || binfp == NULL) {
        printf("파일 읽기 실패\n");
        return 1;
    }


    char buf[100];
    char binbuf[40];
    char **lines = NULL;
    char **original_lines = NULL;
    uint32_t count = 0;
    int cap = 10;

    lines = malloc(cap * sizeof(char *));
    original_lines = malloc(cap * sizeof(char *));

    while (fgets(buf, sizeof(buf), asmfp)) {

        char *findcomment = strchr(buf, ';');
        if (findcomment != NULL){
            findcomment[0] = '\n';
            findcomment[1] = '\0';
        }

        char *findcomma = strchr(buf, ',');
        if (findcomma != NULL){
           * findcomma = ' ';
        }

        char *islabel = strchr(buf, ':');

        if (islabel != NULL) {
            printf("라벨 라인! %d줄!\n", count);
            *islabel = '\0';
            labelput(buf, count);
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

        /*for (int i = 0; i < sizeof(buf); i++){
            if (buf[i] == ';'){
                buf[i] = '\0';
            }
        }*/



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
            int rD = atoi(token + 1);               // 목적지 레지스터의 번호를 int로 변환
            binary |= (rD << 20);                   // 바이너리 코드의 rD 자리를 채운다.
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MOVI") == 0) {    // opcode가 MOVI일 경우
            binary |= (0b00000010U << 24);          // 바이너리 코드의 opcode, i, s 자리를 채운다
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU); // <- 나머지에도 이거 해서 즉시값 부호가 범위 추가하지 않도록 고쳐야 함!!

        } else if (strcmp(token, "MOVIZ") == 0) {
            binary |= (0b00000011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
        
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "MOVH") == 0) {    // opcode가 MOVH일 경우
            binary |= (0b00001100U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "MOVHL") == 0) {
            binary |= (0b00001110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

        } else if (strcmp(token, "MOVHK") == 0) {
            binary |= (0b00001101U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "MOVHLK") == 0) {
            binary |= (0b00001111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "DIV") == 0) {    // opcode가 DIV일 경우
            binary |= (0b00100100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "DIVI") == 0) {
            binary |= (0b00100110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "DIVS") == 0) {
            binary |= (0b00100101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "DIVIS") == 0) {
            binary |= (0b00100111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "DIVU") == 0) {    // opcode가 DIVU일 경우
            binary |= (0b00101000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "DIVUI") == 0) {
            binary |= (0b00101010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "DIVUS") == 0) {
            binary |= (0b00101001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "DIVUIS") == 0) {
            binary |= (0b00101011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "MOD") == 0) {    // opcode가 MOD일 경우
            binary |= (0b00101100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MODI") == 0) {
            binary |= (0b00101110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "MODS") == 0) {
            binary |= (0b00101101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MODIS") == 0) {
            binary |= (0b00101111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "MODU") == 0) {    // opcode가 MODU일 경우
            binary |= (0b00110000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MODUI") == 0) {
            binary |= (0b00110010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "MODUS") == 0) {
            binary |= (0b00110001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MODUIS") == 0) {
            binary |= (0b00110011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "ROL") == 0) {    // opcode가 ROL일 경우
            binary |= (0b01101100U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ROLI") == 0) {
            binary |= (0b01101110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "ROLS") == 0) {
            binary |= (0b01101101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "ROLIS") == 0) {
            binary |= (0b01101111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "ROR") == 0) {    // opcode가 ROR일 경우
            binary |= (0b01110000U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "RORI") == 0) {
            binary |= (0b01110010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "RORS") == 0) {
            binary |= (0b01110001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "RORIS") == 0) {
            binary |= (0b01110011U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int rA = atoi(token + 1);
            binary |= (rA << 16);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

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
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "LDRI") == 0) {
            binary |= (0b10100010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

        } else if (strcmp(token, "LDRR") == 0) {
            binary |= (0b10100001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

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
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "LDRBI") == 0) {
            binary |= (0b10100110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

        } else if (strcmp(token, "LDRBR") == 0) {
            binary |= (0b10100101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

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
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "LDRSBI") == 0) {
            binary |= (0b10101010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

        } else if (strcmp(token, "LDRSBR") == 0) {
            binary |= (0b10101001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

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
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "LDRHI") == 0) {
            binary |= (0b10101110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

        } else if (strcmp(token, "LDRHR") == 0) {
            binary |= (0b10101101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

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
            binary |= (immB & 0x0000ffffU);

        } else if (strcmp(token, "LDRSHI") == 0) {
            binary |= (0b10110010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

        } else if (strcmp(token, "LDRSHR") == 0) {
            binary |= (0b10110001U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

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
            binary |= ((immBf << 20 | immBb) & 0x00f00fffU);

        } else if (strcmp(token, "STRI") == 0) {
            binary |= (0b10110110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= ((immBf << 16 | immBb) & 0x00ff0fffU);

        } else if (strcmp(token, "STRR") == 0) {
            binary |= (0b10110101U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= ((immBf << 16 | immBb) & 0x00ff0fffU);

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
            binary |= ((immBf << 20 | immBb) & 0x00f00fffU);

        } else if (strcmp(token, "STRBI") == 0) {
            binary |= (0b10111010U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= ((immBf << 16 | immBb) & 0x00ff0fffU);

        } else if (strcmp(token, "STRBR") == 0) {
            binary |= (0b10111001U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= ((immBf << 16 | immBb) & 0x00ff0fffU);

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
            binary |= ((immBf << 20 | immBb) & 0x00f00fffU);

        } else if (strcmp(token, "STRHI") == 0) {
            binary |= (0b10111110U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= ((immBf << 16 | immBb) & 0x00ff0fffU);

        } else if (strcmp(token, "STRHR") == 0) {
            binary |= (0b10111101U << 24);
            
            token = strtok(NULL, " \t\n");
            int rB = atoi(token + 1);
            binary |= (rB << 12);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            int immBf = immB >> 12;
            int immBb = immB & 0xFFF;
            binary |= ((immBf << 16 | immBb) & 0x00ff0fffU);

        } else if (strcmp(token, "BEQ") == 0) {    // opcode가 B인 경우
            binary |= (0b110010U << 26);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BNE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0001U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 || immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BCS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0010U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }


        }  else if (strcmp(token, "BHS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0010U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BCC") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0011U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BLO") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0011U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BMI") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0100U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BPL") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0101U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BVS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0110U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BVC") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0111U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BHI") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1000U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BLS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1001U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BGE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1010U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BLT") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1011U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BGT") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1100U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BLE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1101U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "BAL") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1110U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 || immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                binary |= (immB << 4);
            }

        } else if (strcmp(token, "JMP") == 0) {    // opcode가 JMP일 경우
            binary |= (0b11001100U << 24);
            
            char *tokenjmp1 = strtok(NULL, " \t\n");
            char *tokenjmp2 = strtok(NULL, " \t\n");

            if (tokenjmp2 == NULL){
                uint32_t labelline = labelget(tokenjmp1);
                labelline *= 4;
                if (labelline <= 0xffffff){
                    binary = 0;
                    binary |= (0b11001110U << 24);
                    int immB = labelline;
                    binary |= (immB & 0x00ffffffU);
                } else {
                    // 24비트 초과 시.
                }
            } else {
                int rA = atoi(tokenjmp1 + 1);
                binary |= (rA << 16);
                
                int immB = atoi(tokenjmp2 + 1);
                binary |= (immB & 0x0000ffffU);
            }

        } else if (strcmp(token, "JMPI") == 0) {
            binary |= (0b11001110U << 24);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x00ffffffU);

        } else if (strcmp(token, "JMPL") == 0) {
            binary |= (0b11001101U << 24);

            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            char *tokenjmp1 = strtok(NULL, " \t\n");
            char *tokenjmp2 = strtok(NULL, " \t\n");

            if (tokenjmp2 == NULL){
                int labelline = labelget(tokenjmp1);
                labelline *= 4;
                if (labelline <= 0xfffff){
                    binary &= 0x00f00000;
                    binary |= (0b11001111U << 24);
                    int immB = labelline;
                    binary |= (immB & 0x000fffffU);
                } else {
                    // 20비트 초과 시.
                }
            } else {
                int rA = atoi(tokenjmp1 + 1);
                binary |= (rA << 16);
                
                int immB = atoi(tokenjmp2 + 1);
                binary |= (immB & 0x0000ffffU);
            }

        } else if (strcmp(token, "JMPIL") == 0) {
            binary |= (0b11001111U << 24);
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);

        } else {
            printf ("Line %d에 정의되지 않은 명령어가 입력되었습니다: %s\n어셈블러를 종료합니다.\n", i, token);
            return 1;
        }


            for (int i = 31; i >=0; i--) {      // 이진 코드 출력
                printf("%d", (binary >> i) & 1);
                if (i % 4 == 0 && i != 0) printf("_");
            }
            printf("\n");

            int binindex = 0;
            for (int i = 38; i >= 0; i--){
                if ((i + 1) % 5 != 0){
                    binbuf[i] = '0' + ((binary >> binindex) & 1);
                    binindex++;
                } else binbuf[i] = '_';
                
            }
            binbuf[39] = '\n';
            fwrite (binbuf, 1, 40, binfp); 

    }

    char finish[39] = "1111_1111_1111_1111_1111_1111_1111_1111";
    fwrite (finish, sizeof(char), 39, binfp); 


    for (int i = 0; i < count; i++) {
        free(lines[i]);
        free(original_lines[i]);
    }
    free(lines);
    free(original_lines);


    fclose(asmfp);
    fclose(binfp);
    return 0;
}
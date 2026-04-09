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

// b, jmp의 단거리 라벨 지원. 아직 장거리 라벨처리 미지원.


/* *******************************
**********************************
**********************************
***********            ***********
***********    MAIN    ***********
***********            ***********
**********************************
**********************************
******************************* */

int main(int argc, char *argv[])
{
    char *asm_name;
    char *bin_name;

        // 인자 개수 확인해 파일이름 지정.
        if (argc == 3) {
            asm_name = argv[1];
            bin_name = argv[2];
        } else {
            asm_name = "assembly.asm";
            bin_name = "binary.bin";
        }

    FILE *asmfp = fopen(asm_name, "r");       // 처리할 어셈블리가 적힌 파일 오픈
    FILE *binfp = fopen(bin_name, "w");         // 처리 결과 바이너리를 저장할 파일 오픈

    if (asmfp == NULL || binfp == NULL) {           // 두 파일 중 하나라도 열지 못할 시 비정상 종료
        printf("파일 읽기 실패\n");
        return 1;
    }


    char buf[100];          // 읽어온 어셈블리 코드 라인을 저장. 
    char binbuf[40];        // 변환된 바이너리 코드를 이진수로 저장. 가독성을 위해 4자리마다 _를 표기한다.
    char **lines = NULL;            // 명령 어셈블리 라인을 저장. 이 문자열 배열을 쪼개 어셈블리를 바이너리로 번역한다.
    char **original_lines = NULL;   // 명령 어셈블리의 원본을 보존. 번역 후에도 손상되지 않은 어셈블리가 들어있다.
    uint32_t count = 0;     // 명령 어셈블리의 라인 수를 계산한다.
    int cap = 10;           // 문자열 배열의 크기를 의미한다.

    lines = malloc(cap * sizeof(char *));               // 라인 수를 cap만큼으로, 문자열 배열 lines을 malloc한다.
    original_lines = malloc(cap * sizeof(char *));

    while (fgets(buf, sizeof(buf), asmfp)) {        // 입력으로 받은 어셈블리 파일을 한 줄씩 읽어온다. 끝까지 읽어올 때까지 루프한다.

        char *findcomment = strchr(buf, ';');       // buf에서 ;를 찾아 이후 내용을 제거한다. 주석 처리를 담당.
        if (findcomment != NULL){       // ;이 있다면 ;를 줄바꿈으로, ; 다음 자리를 null로 바꾼다.
            findcomment[0] = '\n';      
            findcomment[1] = '\0';      
        }

        char *findcomma = strchr(buf, ',');     // buf에서 ,를 찾아 space로 바꾼다. asm의 인자 구분자의 통일을 담당.
        while (findcomma != NULL){         // ,이 있다면 space로 바꾼다.
            *findcomma = ' ';              
            findcomma = strchr(buf, ',');     // ,이 더 있는지 확인한다. 있다면 반복, 없다면 탈출.
        }

        char *islabel = strchr(buf, ':');       // buf에서 :를 찾는다. :이 존재한다면 라벨 라인이다.

        if (islabel != NULL) {          // 라벨 라인이라면 해당 라인을 비우고 labelput을 호출해, 라벨명과 라인 위치를 기록한다.
            printf("라벨 라인! %d줄!\n", count);    
            *islabel = '\0';                      
            labelput(buf, count);                 
            continue;                             // 명령어 라인이 아니므로, 라인수 카운트 증가 및 어셈블리 로딩 없이 다음 라인으로 넘어간다.
        } else if (buf[0] == '\n') {    // 라벨 라인이 아니며, 라인이 비어있는 경우.
            printf("빈 라인! %d줄!\n", count);     
            continue;                             // 명령어 라인이 아니므로, 라인수 카운트 증가 및 어셈블리 로딩 없이 다음 라인으로 넘어간다.
        }

        if (count >= cap) {         // 라인 수가 배열 크기 한계에 도달한다면
            cap *= 2;               // 배열 크기 한계를 두배로 늘린다.
            lines = realloc(lines, cap * sizeof(char *));   // realloc를 통해 lines 배열 크기를 두배로 늘어난 cap에 맞춘다.
            original_lines = realloc(original_lines, cap * sizeof(char *));   
        }

        /*for (int i = 0; i < sizeof(buf); i++){
            if (buf[i] == ';'){
                buf[i] = '\0';
            }
        }*/



        lines[count] = malloc(strlen(buf) + 1);             // lines[count]의 크기를 어셈블리 코드만큼 할당한다.
        original_lines[count] = malloc(strlen(buf) + 1);    
        strcpy(lines[count], buf);              // 전처리 완료되어 buf에 들어있는 어셈블리를 lines에 저장한다.
        strcpy(original_lines[count], buf);     

        printf("%d     %s", count, lines[count]);       // 검증을 위해, lines에 저장한 어셈블리를 출력한다.
        count++;        // 다음 라인 처리를 위해 라인 수를 1 증가시킨다.
    }

    printf("\n\n--- 어셈블리 불러오기 완료! ---\n\n\n", count, lines[count]);





/* *******************************
**********************************
**********************************
*********                *********
*********    ASSEMBLE    *********
*********                *********
**********************************
**********************************
******************************* */

    for (int i = 0; i < count; i++) {       // 모든 명령 어셈블리 라인을 처리할 때까지 루프.
        printf("line %d\t", i);             // 몇번째 라인을 처리중인지 출력
        uint32_t binary = 0;                // 번역된 바이너리 코드가 담길, 32비트 부호없는 정수 변수 선언.
        char *token = strtok(lines[i], " \t\n");    // 첫 번째 토큰으로 opcode를 확인한다.

        if (strcmp(token, "MOV") == 0) {            // opcode가 MOV일 경우
            token = strtok(NULL, " \t\n");          // 두 번째 토큰으로 목적지 레지스터를 확인한다.
            int rD = atoi(token + 1);               // 목적지 레지스터의 번호를 int로 변환
            binary |= (rD << 20);                   // 바이너리 코드의 rD 자리를 채운다.
            
            token = strtok(NULL, " \t\n");          // 세 번째 토큰으로 소스 레지스터 B를 확인한다.
            int rB = atoi(token + 1);
            binary |= (rB << 12);

        } else if (strcmp(token, "MOVI") == 0) {    // opcode가 MOVI일 경우
            binary |= (0b00000010U << 24);          // 바이너리 코드의 opcode, i, s 자리를 채운다
            
            token = strtok(NULL, " \t\n");
            int rD = atoi(token + 1);
            binary |= (rD << 20);
            
            token = strtok(NULL, " \t\n");          // 세 번째 토큰으로 immB를 확인한다.
            int immB = atoi(token + 1);
            binary |= (immB & 0x000fffffU);         // 즉시값 부호가 범위를 초과하지 않도록 제한한다.

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
            if (token[0] != '#') {      // 라벨 branch인 경우
                int labelline = labelget(token);    // 어떤 라벨인지 감지 후, 라벨이 라인 몇을 가리키는지 가져옴
                int immB = (labelline - i) * 4 ;    // (현재 라인 - 라벨 라인) * 4. 현재 라인은 int i에서 명시중.
                if(immB < 2097151 && immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {      // 주소지정 branch인 경우
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){    // 결과값이 22비트 안쪽인지 확인 후 immB로 만들고 바이너리 생성
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {    // 22비트 바깥쪽이면 오류 처리.
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BNE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0001U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BCS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0010U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }


        }  else if (strcmp(token, "BHS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0010U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BCC") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0011U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BLO") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0011U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BMI") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0100U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BPL") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0101U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BVS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0110U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BVC") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b0111U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BHI") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1000U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BLS") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1001U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BGE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1010U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BLT") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1011U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BGT") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1100U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BLE") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1101U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "BAL") == 0) {
            binary |= (0b110010U << 26);
            binary |= (0b1110U);
            
            token = strtok(NULL, " \t\n");
            if (token[0] != '#') {
                int labelline = labelget(token);
                int immB = (labelline - i) * 4 ;
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
                
            } else {
                int immB = atoi(token + 1);
                if(immB < 2097151 && immB > -2097151){
                    immB = immB & 0x3fffff;
                    binary |= (immB << 4);
                } else {
                    printf("너무 먼 주소로 Branch를 시도하고 있습니다.");
                    exit(1);
                }
            }

        } else if (strcmp(token, "JMP") == 0) {    // opcode가 JMP일 경우
            binary |= (0b11001100U << 24);
            
            char *tokenjmp1 = strtok(NULL, " \t\n");

            if (strcmp(tokenjmp1, "R0") != 0 &&     // 두 번째 토큰이 레지스터 지칭이 아닐 경우, 라벨 점프로 간주한다.
                strcmp(tokenjmp1, "R1") != 0 &&
                strcmp(tokenjmp1, "R2") != 0 &&
                strcmp(tokenjmp1, "R3") != 0 &&
                strcmp(tokenjmp1, "R4") != 0 &&
                strcmp(tokenjmp1, "R5") != 0 &&
                strcmp(tokenjmp1, "R6") != 0 &&
                strcmp(tokenjmp1, "R7") != 0 &&
                strcmp(tokenjmp1, "R8") != 0 &&
                strcmp(tokenjmp1, "R9") != 0 &&
                strcmp(tokenjmp1, "R10") != 0 &&
                strcmp(tokenjmp1, "R11") != 0 &&
                strcmp(tokenjmp1, "R12") != 0 &&
                strcmp(tokenjmp1, "R13") != 0 &&
                strcmp(tokenjmp1, "R14") != 0 &&
                strcmp(tokenjmp1, "R15") != 0 &&
                strcmp(tokenjmp1, "r0") != 0 &&
                strcmp(tokenjmp1, "r1") != 0 &&
                strcmp(tokenjmp1, "r2") != 0 &&
                strcmp(tokenjmp1, "r3") != 0 &&
                strcmp(tokenjmp1, "r4") != 0 &&
                strcmp(tokenjmp1, "r5") != 0 &&
                strcmp(tokenjmp1, "r6") != 0 &&
                strcmp(tokenjmp1, "r7") != 0 &&
                strcmp(tokenjmp1, "r8") != 0 &&
                strcmp(tokenjmp1, "r9") != 0 &&
                strcmp(tokenjmp1, "r10") != 0 &&
                strcmp(tokenjmp1, "r11") != 0 &&
                strcmp(tokenjmp1, "r12") != 0 &&
                strcmp(tokenjmp1, "r13") != 0 &&
                strcmp(tokenjmp1, "r14") != 0 &&
                strcmp(tokenjmp1, "r15") != 0)
            {
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
            } else {    // 레지스터 지정 점프일 경우
                int rB = atoi(tokenjmp1 + 1);
                binary |= (rB << 12);
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

            if (strcmp(tokenjmp1, "R0") != 0 &&
                strcmp(tokenjmp1, "R1") != 0 &&
                strcmp(tokenjmp1, "R2") != 0 &&
                strcmp(tokenjmp1, "R3") != 0 &&
                strcmp(tokenjmp1, "R4") != 0 &&
                strcmp(tokenjmp1, "R5") != 0 &&
                strcmp(tokenjmp1, "R6") != 0 &&
                strcmp(tokenjmp1, "R7") != 0 &&
                strcmp(tokenjmp1, "R8") != 0 &&
                strcmp(tokenjmp1, "R9") != 0 &&
                strcmp(tokenjmp1, "R10") != 0 &&
                strcmp(tokenjmp1, "R11") != 0 &&
                strcmp(tokenjmp1, "R12") != 0 &&
                strcmp(tokenjmp1, "R13") != 0 &&
                strcmp(tokenjmp1, "R14") != 0 &&
                strcmp(tokenjmp1, "R15") != 0 &&
                strcmp(tokenjmp1, "r0") != 0 &&
                strcmp(tokenjmp1, "r1") != 0 &&
                strcmp(tokenjmp1, "r2") != 0 &&
                strcmp(tokenjmp1, "r3") != 0 &&
                strcmp(tokenjmp1, "r4") != 0 &&
                strcmp(tokenjmp1, "r5") != 0 &&
                strcmp(tokenjmp1, "r6") != 0 &&
                strcmp(tokenjmp1, "r7") != 0 &&
                strcmp(tokenjmp1, "r8") != 0 &&
                strcmp(tokenjmp1, "r9") != 0 &&
                strcmp(tokenjmp1, "r10") != 0 &&
                strcmp(tokenjmp1, "r11") != 0 &&
                strcmp(tokenjmp1, "r12") != 0 &&
                strcmp(tokenjmp1, "r13") != 0 &&
                strcmp(tokenjmp1, "r14") != 0 &&
                strcmp(tokenjmp1, "r15") != 0)
            {
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
                int rB = atoi(tokenjmp1 + 1);
                binary |= (rB << 12);
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
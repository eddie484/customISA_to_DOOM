#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define IMEM_SIZE (4 * 1024 * 1024)
#define DMEM_SIZE (4 * 1024 * 1024)


enum Opcode{
    MOV = 0b000000,
    ADD = 0b000001,
    SUB = 0b000010,
    MOVH = 0b000011,
    CMP = 0b000100,
    MUL = 0b000101,
    MULH = 0b000110,
    MULHU = 0b000111,
    MULFX = 0b001000,
    DIV = 0b001001,
    DIVU = 0b001010,
    MOD = 0b001011,
    MODU = 0b001100,
    SHL = 0b011000,
    ASR = 0b011001,
    LSR = 0b011010,
    ROL = 0b011011,
    ROR = 0b011100,
    AND = 0b100001,
    OR = 0b100010,
    XOR = 0b100011,
    NOT = 0b100100,
    BCHK = 0b100101,
    LDR = 0b101000,
    LDRB = 0b101001,
    LDRSB = 0b101010,
    LDRH = 0b101011,
    LDRSH = 0b101100,
    STR = 0b101101,
    STRB = 0b101110,
    STRH = 0b101111,
    B = 0b110010,
    JMP = 0b110011,
    KILL = 0b111111,
}; 

enum Cond{
    EQ = 0b0000,
    NE = 0b0001,
    HS = 0b0010,
    LO = 0b0011,
    MI = 0b0100,
    PL = 0b0101,
    VS = 0b0110,
    VC = 0b0111,
    HI = 0b1000,
    LS = 0b1001,
    GE = 0b1010,
    LT = 0b1011,
    GT = 0b1100,
    LE = 0b1101,
    AL = 0b1110,
};

struct GISA {
    uint32_t reg[16];
    uint32_t pc;
    bool nzcv[4];
    uint8_t DMEM[DMEM_SIZE];
    uint8_t IMEM[IMEM_SIZE];
};




/* ---------- Execute Part Start ---------- */

void mov(struct GISA *cpu, int rD, int rB)
{
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\tRegister[%d] updated from %d to %d.\n", cpu->pc, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void movi(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 12) >> 12);
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;
    
    printf("IMEM[%d]\tRegister[%d] updated from %d to %d.\n", cpu->pc, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void moviz(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;
    
    printf("IMEM[%d]\tRegister[%d] updated from %d to %d.\n", cpu->pc, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void add(struct GISA *cpu, int rD, int rA, int rB)  
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata + (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d + %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void addi(struct GISA *cpu, int rD, int rA, int immB)   
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata + (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d + %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void adds(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata + (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;
    bool oldnzcv[4];

    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((result >> 32) & 1);
    cpu->nzcv[3] = (((rAdata >> 31) == (rBdata >> 31)) && ((rAdata >> 31) != (result32 >> 31)));

    printf("IMEM[%d]\t%d + %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void addis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata + (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((result >> 32) & 1);
    cpu->nzcv[3] = (((rAdata >> 31) == (rBdata >> 31)) && ((rAdata >> 31) != (result32 >> 31)));

    printf("IMEM[%d]\t%d + %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void sub(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t notB = ~rBdata;
    uint64_t result = (uint64_t)rAdata + (uint64_t)notB + 1;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d - %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void subi(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t notB = ~rBdata;
    uint64_t result = (uint64_t)rAdata + (uint64_t)notB + 1;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d - %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void subs(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t notB = ~rBdata;
    uint64_t result = (uint64_t)rAdata + (uint64_t)notB + 1;
    uint32_t result32 = (uint32_t)result;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((result >> 32) & 1);
    cpu->nzcv[3] = (((rAdata >> 31) != (rBdata >> 31)) && ((rAdata >> 31) != (result32 >> 31)));

    printf("IMEM[%d]\t%d - %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void subis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t notB = ~rBdata;
    uint64_t result = (uint64_t)rAdata + (uint64_t)notB + 1;
    uint32_t result32 = (uint32_t)result;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((result >> 32) & 1);
    cpu->nzcv[3] = (((rAdata >> 31) != (rBdata >> 31)) && ((rAdata >> 31) != (result32 >> 31)));

    printf("IMEM[%d]\t%d - %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void movh(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = immB;
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)(rBdata << 16) | 0x00000000;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\trBdata =  %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rBdata, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void movhl(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = immB;
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)(rBdata << 12) | 0x00000000;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\trBdata =  %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rBdata, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void movhk(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = immB;
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)(rBdata << 16) | (uint64_t)(uint16_t)oldrD;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\trBdata =  %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rBdata, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void movhlk(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = immB;
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)(rBdata << 12) | (uint64_t)(uint16_t)oldrD;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\trBdata =  %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rBdata, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}


void cmp(struct GISA *cpu, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t notB = ~rBdata;
    uint64_t result = (uint64_t)rAdata + (uint64_t)notB + 1;
    uint32_t result32 = (uint32_t)result;

    printf("IMEM[%d]\t%d - %d = %d,   Compare result isn't saved.\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result);
    cpu->pc = cpu->pc + 4;
}

void cmpi(struct GISA *cpu, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t notB = ~rBdata;
    uint64_t result = (uint64_t)rAdata + (uint64_t)notB + 1;
    uint32_t result32 = (uint32_t)result;

    printf("IMEM[%d]\t%d - %d = %d,   Compare result isn't saved.\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result);
    cpu->pc = cpu->pc + 4;
}

void cmps(struct GISA *cpu, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t notB = ~rBdata;
    uint64_t result = (uint64_t)rAdata + (uint64_t)notB + 1;
    uint32_t result32 = (uint32_t)result;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((result >> 32) & 1);
    cpu->nzcv[3] = (((rAdata >> 31) != (rBdata >> 31)) && ((rAdata >> 31) != (result32 >> 31)));

    printf("IMEM[%d]\t%d - %d = %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void cmpis(struct GISA *cpu, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t notB = ~rBdata;
    uint64_t result = (uint64_t)rAdata + (uint64_t)notB + 1;
    uint32_t result32 = (uint32_t)result;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((result >> 32) & 1);
    cpu->nzcv[3] = (((rAdata >> 31) != (rBdata >> 31)) && ((rAdata >> 31) != (result32 >> 31)));

    printf("IMEM[%d]\t%d - %d = %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int)result, oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void mul(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata * (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d × %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void muli(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata * (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d × %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void muls(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata * (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((result >> 32) & 1);
    cpu->nzcv[3] = ((result >> 32) != 0);

    printf("IMEM[%d]\t%d × %d = %lld,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void mulis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata * (uint64_t)rBdata;
    uint32_t result32 = (uint32_t)result;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((result >> 32) & 1);
    cpu->nzcv[3] = ((result >> 32) != 0);

    printf("IMEM[%d]\t%d × %d = %lld,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void mulh(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    int64_t result = (int64_t)(int32_t)rAdata * (int64_t)(int32_t)rBdata;
    int64_t result_shifted = (result >> 32);
    uint32_t result32 = (uint32_t)result_shifted;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d × %d = %lld, %lld >> 32 = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, (int64_t)result, (int64_t)result_shifted, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void mulhi(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    int64_t result = (int64_t)(int32_t)rAdata * (int64_t)(int32_t)rBdata;
    int64_t result_shifted = (result >> 32);
    uint32_t result32 = (uint32_t)result_shifted;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d × %d = %lld, %lld >> 32 = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, (int64_t)result, (int64_t)result_shifted, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void mulhs(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    int64_t result = (int64_t)(int32_t)rAdata * (int64_t)(int32_t)rBdata;
    int64_t result_shifted = (result >> 32);
    uint32_t result32 = (uint32_t)result_shifted;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d × %d = %d, %lld >> 32 = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, (int64_t)result, (int64_t)result_shifted, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void mulhis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    int64_t result = (int64_t)(int32_t)rAdata * (int64_t)(int32_t)rBdata;
    int64_t result_shifted = (result >> 32);
    uint32_t result32 = (uint32_t)result_shifted;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d × %d = %d, %lld >> 32 = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, (int64_t)result, (int64_t)result_shifted, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void mulhu(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata * (uint64_t)rBdata;
    uint64_t result_shifted = (result >> 32);
    uint32_t result32 = (uint32_t)result_shifted;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d × %d = %llu, %llu >> 32 = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (uint64_t)result, (uint64_t)result, (int64_t)result_shifted, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void mulhui(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata * (uint64_t)rBdata;
    uint64_t result_shifted = (result >> 32);
    uint32_t result32 = (uint32_t)result_shifted;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%d × %d = %llu, %llu >> 32 = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, (int64_t)result, (int64_t)result_shifted, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}


void mulhus(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata * (uint64_t)rBdata;
    uint64_t result_shifted = (result >> 32);
    uint32_t result32 = (uint32_t)result_shifted;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d × %d = %d, %lld >> 32 = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, (int64_t)result, (int64_t)result_shifted, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void mulhuis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint64_t result = (uint64_t)rAdata * (uint64_t)rBdata;
    uint64_t result_shifted = (result >> 32);
    uint32_t result32 = (uint32_t)result_shifted;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d × %d = %d, %lld >> 32 = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int)rAdata, (int)rBdata, (int64_t)result, (int64_t)result, (int64_t)result_shifted, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void mulfx(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    int64_t result = (int64_t)(int32_t)rAdata * (int64_t)(int32_t)rBdata;
    int64_t result_shifted = (result >> 16);
    uint32_t result32 = (uint32_t)result_shifted;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%.4f × %.4f = %.4f,   Register[%d] updated from %d to %.4f (%d).\n", cpu->pc, (int32_t)rAdata / 65536.0, (int32_t)rBdata / 65536.0, (int32_t)result32 / 65536.0, rD, oldrD, (int32_t)result32 / 65536.0, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void mulfxi(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    int64_t result = (int64_t)(int32_t)rAdata * (int64_t)(int32_t)rBdata;
    int64_t result_shifted = (result >> 16);
    uint32_t result32 = (uint32_t)result_shifted;
    cpu->reg[rD] = result32;

    printf("IMEM[%d]\t%.4f × %.4f = %.4f,   Register[%d] updated from %d to %.4f (%d).\n", cpu->pc, (int32_t)rAdata / 65536.0, (int32_t)rBdata / 65536.0, (int32_t)result32 / 65536.0, rD, oldrD, (int32_t)result32 / 65536.0, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void mulfxs(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    int64_t result = (int64_t)(int32_t)rAdata * (int64_t)(int32_t)rBdata;
    int64_t result_shifted = (result >> 16);
    uint32_t result32 = (uint32_t)result_shifted;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((int16_t)result) != 0;
    cpu->nzcv[3] = ((result >> 48) != ((int32_t)result32 >>31));

    printf("IMEM[%d]\t%.4f × %.4f = %.4f,   Register[%d] updated from %d to %.4f (%d),   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata / 65536.0, (int32_t)rBdata / 65536.0, (int32_t)result32 / 65536.0, rD, oldrD, (int32_t)result32 / 65536.0, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void mulfxis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    int64_t result = (int64_t)(int32_t)rAdata * (int64_t)(int32_t)rBdata;
    int64_t result_shifted = (result >> 16);
    uint32_t result32 = (uint32_t)result_shifted;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result32;
    cpu->nzcv[0] = (result32 >> 31);
    cpu->nzcv[1] = (result32 == 0);
    cpu->nzcv[2] = ((int16_t)result) != 0;
    cpu->nzcv[3] = ((result >> 48) != ((int32_t)result32 >>31));

    printf("IMEM[%d]\t%.4f × %.4f = %.4f,   Register[%d] updated from %d to %.4f (%d),   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata / 65536.0, (int32_t)rBdata / 65536.0, (int32_t)result32 / 65536.0, rD, oldrD, (int32_t)result32 / 65536.0, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void div_op(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    int32_t result;

    if (rBdata != 0 && (rAdata != 0x80000000 || rBdata != -1)){
        result = (int32_t)rAdata / (int32_t)rBdata;
    } else {
        result = 0;
    }
    
    cpu->reg[rD] = (uint32_t)result;

    printf("IMEM[%d]\t%d / %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void divi(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    int32_t result;

    if (rBdata != 0){
        result = (int32_t)rAdata / (int32_t)rBdata;
    } else {
        result = 0;
    }
    
    cpu->reg[rD] = (uint32_t)result;

    printf("IMEM[%d]\t%d / %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void divs(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    int32_t result;

    if (rBdata != 0 && (rAdata != 0x80000000 || rBdata != -1)){
        result = (int32_t)rAdata / (int32_t)rBdata;
    } else {
        result = 0;
    }

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = (uint32_t)result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = (rBdata == 0);
    cpu->nzcv[3] = (rAdata == 0x80000000 && rBdata == -1);

    printf("IMEM[%d]\t%d / %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void divis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    int32_t result;

    if (rBdata != 0){
        result = (int32_t)rAdata / (int32_t)rBdata;
    } else {
        result = 0;
    }

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = (uint32_t)result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = (rBdata == 0);
    cpu->nzcv[3] = (rAdata == 0x80000000 && rBdata == -1);

    printf("IMEM[%d]\t%d / %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void divu(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result;

    if (rBdata != 0){
        result = rAdata / rBdata;
    } else {
        result = 0;
    }
    
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d / %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void divui(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result;

    if (rBdata != 0){
        result = rAdata / rBdata;
    } else {
        result = 0;
    }
    
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d / %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void divus(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result;

    if (rBdata != 0){
        result = rAdata / rBdata;
    } else {
        result = 0;
    }

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = (uint32_t)result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = (rBdata == 0);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d / %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void divuis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result;

    if (rBdata != 0){
        result = rAdata / rBdata;
    } else {
        result = 0;
    }

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = (uint32_t)result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = (rBdata == 0);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d / %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void mod(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    int32_t result;

    if (rBdata != 0){
        result = (int32_t)rAdata % (int32_t)rBdata;
    } else {
        result = 0;
    }
    
    cpu->reg[rD] = (uint32_t)result;

    printf("IMEM[%d]\t%d %% %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void modi(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    int32_t result;

    if (rBdata != 0){
        result = (int32_t)rAdata % (int32_t)rBdata;
    } else {
        result = 0;
    }
    
    cpu->reg[rD] = (uint32_t)result;

    printf("IMEM[%d]\t%d %% %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void mods(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    int32_t result;

    if (rBdata != 0){
        result = (int32_t)rAdata % (int32_t)rBdata;
    } else {
        result = 0;
    }

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = (uint32_t)result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = (rBdata == 0);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d %% %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void modis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    int32_t result;

    if (rBdata != 0){
        result = (int32_t)rAdata % (int32_t)rBdata;
    } else {
        result = 0;
    }

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = (uint32_t)result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = (rBdata == 0);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d %% %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void modu(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result;

    if (rBdata != 0){
        result = rAdata % rBdata;
    } else {
        result = 0;
    }
    
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d %% %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void modui(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result;

    if (rBdata != 0){
        result = rAdata % rBdata;
    } else {
        result = 0;
    }
    
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d %% %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void modus(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result;

    if (rBdata != 0){
        result = rAdata % rBdata;
    } else {
        result = 0;
    }

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = (uint32_t)result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = (rBdata == 0);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d %% %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void moduis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result;

    if (rBdata != 0){
        result = rAdata % rBdata;
    } else {
        result = 0;
    }

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = (uint32_t)result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = (rBdata == 0);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d %% %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void shl(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata << rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d << %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void shli(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata << rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d << %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void shls(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata << rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (32 - rBdata)) & 1);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d << %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void shlis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata << rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (32 - rBdata)) & 1);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d << %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void asr(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (uint32_t)((int32_t)rAdata >> rBdata);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d ASR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void asri(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (uint32_t)((int32_t)rAdata >> rBdata);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d ASR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, (int)rAdata, (int)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void asrs(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (uint32_t)((int32_t)rAdata >> rBdata);

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (rBdata - 1)) & 1);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d ASR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void asris(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (uint32_t)((int32_t)rAdata >> rBdata);

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (rBdata - 1)) & 1);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d ASR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, (int32_t)rAdata, (int32_t)rBdata, (int32_t)result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void lsr(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata >> rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d LSR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void lsri(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata >> rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d LSR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void lsrs(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata >> rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (rBdata - 1)) & 1);
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d LSR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void lsris(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata >> rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (rBdata - 1)) & 1);  // shifter에서 출력되도록?
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d LSR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void rol(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (rAdata << rBdata) | (rAdata >> (32 - rBdata));
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d ROL %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void roli(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (rAdata << rBdata) | (rAdata >> (32 - rBdata));
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d ROL %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void rols(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (rAdata << rBdata) | (rAdata >> (32 - rBdata));

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (32 - rBdata)) & 1); // (result[0])
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d ROL %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void rolis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (rAdata << rBdata) | (rAdata >> (32 - rBdata));

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (32 - rBdata)) & 1); // (result[0])
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d ROL %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void ror(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (rAdata >> rBdata) | (rAdata << (32 - rBdata));
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d ROR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void rori(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (rAdata >> rBdata) | (rAdata << (32 - rBdata));
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d ROR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void rors(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (rAdata >> rBdata) | (rAdata << (32 - rBdata));

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (32 - rBdata)) & 1); // (result[0])
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d ROR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void roris(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = (rAdata >> rBdata) | (rAdata << (32 - rBdata));

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = ((result >> (32 - rBdata)) & 1); // (result[0])
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d ROR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void and(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata & rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d AND %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void andi(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata & rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d AND %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ands(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata & rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d AND %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void andis(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata & rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d AND %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void or(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata | rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d OR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ori(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata | rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d OR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ors(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata | rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d OR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void oris(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata | rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d OR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void xor(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata ^ rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d XOR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void xori(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata ^ rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\t%d XOR %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void xors(struct GISA *cpu, int rD, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata ^ rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d XOR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void xoris(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = rAdata ^ rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d XOR %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void not(struct GISA *cpu, int rD, int rB)
{
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = ~rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tNOT %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void noti(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = ~rBdata;
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tNOT %d = %d,   Register[%d] updated from %d to %d.\n", cpu->pc, rBdata, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void nots(struct GISA *cpu, int rD, int rB)
{
    uint32_t rBdata = cpu->reg[rB];
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = ~rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\tNOT %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void notis(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t result = ~rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->reg[rD] = result;
    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\tNOT %d = %d,   Register[%d] updated from %d to %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rBdata, result, rD, oldrD, cpu->reg[rD], oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void bchk(struct GISA *cpu, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t result = rAdata & rBdata;

    printf("IMEM[%d]\t%d AND %d = %d,   Compare result isn't saved.\n", cpu->pc, rAdata, rBdata, result);
    cpu->pc = cpu->pc + 4;
}

void bchki(struct GISA *cpu, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t result = rAdata & rBdata;

    printf("IMEM[%d]\t%d AND %d = %d,   Compare result isn't saved.\n", cpu->pc, rAdata, rBdata, result);
    cpu->pc = cpu->pc + 4;
}

void bchks(struct GISA *cpu, int rA, int rB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = cpu->reg[rB];
    uint32_t result = rAdata & rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d AND %d = %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}

void bchkis(struct GISA *cpu, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t result = rAdata & rBdata;

    bool oldnzcv[4];
    oldnzcv[0] = cpu->nzcv[0];
    oldnzcv[1] = cpu->nzcv[1];
    oldnzcv[2] = cpu->nzcv[2];
    oldnzcv[3] = cpu->nzcv[3];

    cpu->nzcv[0] = (result >> 31);
    cpu->nzcv[1] = (result == 0);
    cpu->nzcv[2] = 0;
    cpu->nzcv[3] = 0;

    printf("IMEM[%d]\t%d AND %d = %d,   NZCV updated from [%d, %d, %d, %d] to [%d, %d, %d, %d].\n", cpu->pc, rAdata, rBdata, result, oldnzcv[0], oldnzcv[1], oldnzcv[2], oldnzcv[3], cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);
    cpu->pc = cpu->pc + 4;
}



void ldr(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rAdata + rBdata;
    uint32_t result = (cpu->DMEM[addr + 3] << 24) | (cpu->DMEM[addr + 2] << 16) | (cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 3, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldri(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rBdata;
    uint32_t result = (cpu->DMEM[addr + 3] << 24) | (cpu->DMEM[addr + 2] << 16) | (cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 3, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrr(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = cpu->pc + rBdata;
    uint32_t result = (cpu->DMEM[addr + 3] << 24) | (cpu->DMEM[addr + 2] << 16) | (cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 3, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}



void ldrb(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rAdata + rBdata;
    uint32_t result = (uint32_t)(cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrbi(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rBdata;
    uint32_t result = (uint32_t)(cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrbr(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = cpu->pc + rBdata;
    uint32_t result = (uint32_t)(cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d],   Register[%d] updated from %d to %d.\n", cpu->pc, result, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}



void ldrsb(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rAdata + rBdata;
    uint32_t result = (uint32_t)(int32_t)(int8_t)(cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrsbi(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rBdata;
    uint32_t result = (uint32_t)(int32_t)(int8_t)(cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrsbr(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = cpu->pc + rBdata;
    uint32_t result = (uint32_t)(int32_t)(int8_t)(cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}



void ldrh(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rAdata + rBdata;
    uint32_t result = (uint32_t)(cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 1, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrhi(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rBdata;
    uint32_t result = (uint32_t)(cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 1, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrhr(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = cpu->pc + rBdata;
    uint32_t result = (uint32_t)(cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 1, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}



void ldrsh(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rAdata + rBdata;
    uint32_t result = (uint32_t)(int32_t)(int16_t)(cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 1, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrshi(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = rBdata;
    uint32_t result = (uint32_t)(int32_t)(int16_t)(cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 1, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}

void ldrshr(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t oldrD = cpu->reg[rD];
    uint32_t addr = cpu->pc + rBdata;
    uint32_t result = (uint32_t)(int32_t)(int16_t)(cpu->DMEM[addr + 1] << 8) | (cpu->DMEM[addr]);
    cpu->reg[rD] = result;

    printf("IMEM[%d]\tLoad from DMEM[%d~%d],   Register[%d] updated from %d to %d.\n", cpu->pc, addr, addr + 1, rD, oldrD, cpu->reg[rD]);
    cpu->pc = cpu->pc + 4;
}



void str(struct GISA *cpu, int rB, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t result = rAdata + rBdata;
    uint32_t oldDMEM = (cpu->DMEM[result + 3] << 24) | (cpu->DMEM[result + 2] << 16) | (cpu->DMEM[result + 1] << 8) | (cpu->DMEM[result]);
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);
    cpu->DMEM[result + 1] = (uint8_t)(cpu->reg[rB] >> 8);
    cpu->DMEM[result + 2] = (uint8_t)(cpu->reg[rB] >> 16);
    cpu->DMEM[result + 3] = (uint8_t)(cpu->reg[rB] >> 24);

    printf("IMEM[%d]\tDMEM[%d~%d] updated from %d to %d.\n", cpu->pc, result, result + 3, oldDMEM, cpu->reg[rB]);
    cpu->pc = cpu->pc + 4;
}

void stri(struct GISA *cpu, int rB, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t result = rBdata;
    uint32_t oldDMEM = (cpu->DMEM[result + 3] << 24) | (cpu->DMEM[result + 2] << 16) | (cpu->DMEM[result + 1] << 8) | (cpu->DMEM[result]);
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);
    cpu->DMEM[result + 1] = (uint8_t)(cpu->reg[rB] >> 8);
    cpu->DMEM[result + 2] = (uint8_t)(cpu->reg[rB] >> 16);
    cpu->DMEM[result + 3] = (uint8_t)(cpu->reg[rB] >> 24);

    printf("IMEM[%d]\tDMEM[%d~%d] updated from %d to %d.\n", cpu->pc, result, result + 3, oldDMEM, cpu->reg[rB]);
    cpu->pc = cpu->pc + 4;
}

void strr(struct GISA *cpu, int rB, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t result = cpu->pc + rBdata;
    uint32_t oldDMEM = (cpu->DMEM[result + 3] << 24) | (cpu->DMEM[result + 2] << 16) | (cpu->DMEM[result + 1] << 8) | (cpu->DMEM[result]);
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);
    cpu->DMEM[result + 1] = (uint8_t)(cpu->reg[rB] >> 8);
    cpu->DMEM[result + 2] = (uint8_t)(cpu->reg[rB] >> 16);
    cpu->DMEM[result + 3] = (uint8_t)(cpu->reg[rB] >> 24);

    printf("IMEM[%d]\tDMEM[%d~%d] updated from %d to %d.\n", cpu->pc, result, result + 3, oldDMEM, cpu->reg[rB]);
    cpu->pc = cpu->pc + 4;
}



void strb(struct GISA *cpu, int rB, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t result = rAdata + rBdata;
    uint32_t oldDMEM = cpu->DMEM[result];
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);

    printf("IMEM[%d]\tDMEM[%d] updated from %d to %d.\n", cpu->pc, result, oldDMEM, (uint8_t)(cpu->reg[rB]));
    cpu->pc = cpu->pc + 4;
}

void strbi(struct GISA *cpu, int rB, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t result = rBdata;
    uint32_t oldDMEM = cpu->DMEM[result];
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);

    printf("IMEM[%d]\tDMEM[%d] updated from %d to %d.\n", cpu->pc, result, oldDMEM, (uint8_t)(cpu->reg[rB]));
    cpu->pc = cpu->pc + 4;
}

void strbr(struct GISA *cpu, int rB, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t result = cpu->pc + rBdata;
    uint32_t oldDMEM = cpu->DMEM[result];
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);

    printf("IMEM[%d]\tDMEM[%d] updated from %d to %d.\n", cpu->pc, result, oldDMEM, (uint8_t)(cpu->reg[rB]));
    cpu->pc = cpu->pc + 4;
}



void strh(struct GISA *cpu, int rB, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t result = rAdata + rBdata;
    uint32_t oldDMEM = (cpu->DMEM[result + 1] << 8) | (cpu->DMEM[result]);
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);
    cpu->DMEM[result + 1] = (uint8_t)(cpu->reg[rB] >> 8);

    printf("IMEM[%d]\tDMEM[%d~%d] updated from %d to %d.\n", cpu->pc, result, result + 1, oldDMEM, (uint16_t)(cpu->reg[rB]));
    cpu->pc = cpu->pc + 4;
}

void strhi(struct GISA *cpu, int rB, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t result = rBdata;
    uint32_t oldDMEM = (cpu->DMEM[result + 1] << 8) | (cpu->DMEM[result]);
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);
    cpu->DMEM[result + 1] = (uint8_t)(cpu->reg[rB] >> 8);

    printf("IMEM[%d]\tDMEM[%d~%d] updated from %d to %d.\n", cpu->pc, result, result + 1, oldDMEM, (uint16_t)(cpu->reg[rB]));
    cpu->pc = cpu->pc + 4;
}

void strhr(struct GISA *cpu, int rB, int immB)
{
    uint32_t rBdata = (uint32_t)((immB << 16) >> 16);
    uint32_t result = cpu->pc + rBdata;
    uint32_t oldDMEM = (cpu->DMEM[result + 1] << 8) | (cpu->DMEM[result]);
    cpu->DMEM[result] = (uint8_t)(cpu->reg[rB]);
    cpu->DMEM[result + 1] = (uint8_t)(cpu->reg[rB] >> 8);

    printf("IMEM[%d]\tDMEM[%d~%d] updated from %d to %d.\n", cpu->pc, result, result + 1, oldDMEM, (uint16_t)(cpu->reg[rB]));
    cpu->pc = cpu->pc + 4;
}



void b(struct GISA *cpu, int cond, int immB)
{
    int32_t rBdata = (int32_t)((immB << 10) >> 10);
    uint32_t oldpc = cpu->pc;
    uint32_t result = (uint32_t)(rBdata + (int32_t)cpu->pc);
    //printf("How much Branch?: %d\n", immB);

    bool N = cpu->nzcv[0];
    bool Z = cpu->nzcv[1];
    bool C = cpu->nzcv[2];
    bool V = cpu->nzcv[3];

    if ((cond == AL) |
        ((cond == EQ) & Z) |
        ((cond == NE) & !Z) |
        ((cond == HS) & C) |
        ((cond == LO) & !C) |
        ((cond == MI) & N) |
        ((cond == PL) & !N) |
        ((cond == VS) & V) |
        ((cond == VC) & !V) |
        ((cond == HI) & (C & !Z)) |
        ((cond == LS) & (!C | Z)) |
        ((cond == GE) & (!N ^ V)) |
        ((cond == LT) & (N ^ V)) |
        ((cond == GT) & ((!N ^ V) & !Z)) |
        ((cond == LE) & ((N ^ V) | Z))){
        cpu->pc = result;

        printf("IMEM[%d]\tBranch from %d to %d.\n", oldpc, oldpc, result);
    } else {
        printf("IMEM[%d]\tBranch not worked.\n", cpu->pc);
        cpu->pc = cpu->pc + 4;
    }
}



void jmp(struct GISA *cpu, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t result = rAdata | rBdata;
    uint32_t oldpc = cpu->pc;
    cpu->pc = result;

    printf("IMEM[%d]\tJump from %d to %d.\n", oldpc, oldpc, result);
}

void jmpi(struct GISA *cpu,int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t result = rBdata;
    uint32_t oldpc = cpu->pc;
    cpu->pc = result;

    printf("IMEM[%d]\tJump from %d to %d.\n", oldpc, oldpc, result);
}

void jmpl(struct GISA *cpu, int rD, int rA, int immB)
{
    uint32_t rAdata = cpu->reg[rA];
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t oldpc = cpu->pc;
    uint32_t result = rAdata | rBdata;

    cpu->reg[rD] = cpu->pc + 4;
    cpu->pc = result;

    printf("IMEM[%d]\tPC saved to Register[%d]. Jump from %d to %d.\n", oldpc, rD, oldpc, result);
}

void jmpil(struct GISA *cpu, int rD, int immB)
{
    uint32_t rBdata = (uint32_t)immB;
    uint32_t oldrD = cpu->reg[rD];
    uint32_t oldpc = cpu->pc;
    uint32_t result = rBdata;

    cpu->reg[rD] = cpu->pc + 4;
    cpu->pc = result;

    printf("IMEM[%d]\tPC saved to Register[%d]. Jump from %d to %d.\n", oldpc, rD, oldpc, result);
}



/* ---------- Execute Part Start ---------- */





/* ---------- Decode Part Start ---------- */

int opcode(uint32_t inst){
    return ((inst >> 26) & 0x3f);
}

int modebit_1(uint32_t inst){
    return ((inst >> 25) & 0x1);
}

int modebit_2(uint32_t inst){
    return ((inst >> 24) & 0x1);
}

int rD(uint32_t inst){
    return ((inst >> 20) & 0xf);
}

int rA(uint32_t inst){
    return ((inst >> 16) & 0xf);
}

int rB(uint32_t inst){
    return ((inst >> 12) & 0xf);
}

int immB_16(uint32_t inst){
    return (inst & 0xffff);
}

int immB_20(uint32_t inst){
    return (inst & 0xfffff);
}

int immB_16_store(uint32_t inst){
    return ((inst & 0xfff) | ((inst >> 8) & 0xf000));
}

int immB_20_store(uint32_t inst){
    return ((inst & 0xfff) | ((inst >> 4) & 0xff000));
}

int immB_22_branch(uint32_t inst){
    //printf("brnach: %d\n", (inst >> 4));
    return ((inst >> 4) & 0x3fffff);
}

int immB_24_jump(uint32_t inst){
    return (inst & 0xffffff);
}

int cond(uint32_t inst){
    return (inst & 0xf);
}

/* ---------- Decode Part End ---------- */






void start_setting(struct GISA *cpu)
{
    for (int i = 0; i < 16; i++){
        cpu->reg[i] = 0;
        printf("R[%d] = %d\n", i, cpu->reg[i]);
    }

    printf("Register File initialization completed.\n\n");
    
    cpu->pc = 0;
    printf("Program Counter initialization completed: PC = %d\n", cpu->pc);
    cpu->nzcv[0] = false;
    cpu->nzcv[1] = false;
    cpu->nzcv[2] = false;
    cpu->nzcv[3] = false;
    printf("NZCV Register initialization completed: N = %d, Z = %d, C = %d, V = %d\n\n", cpu->nzcv[0], cpu->nzcv[1], cpu->nzcv[2], cpu->nzcv[3]);

    for (int i = 0; i < DMEM_SIZE; i = i + 4){
        cpu->DMEM[i] = 0xc9;  // 0xB10580C9 로 초기화.
        cpu->DMEM[i + 1] = 0x80;  // 0xB10580C9 로 초기화.
        cpu->DMEM[i + 2] = 0x05;  // 0xB10580C9 로 초기화.
        cpu->DMEM[i + 3] = 0xb1;  // 0xB10580C9 로 초기화.
    }
    printf("DMEM initial setup complete:\nDMEM[0] = %x\nDMEM[DMEM_SIZE-1] = %x\n\n", cpu->DMEM[0], cpu->DMEM[DMEM_SIZE-1]);


    printf("Loading IMEM...\n");
    FILE  *binfp = fopen("binary.bin", "r");
    char binbuf[64];
    int imemcount = 0;
    char *imemcheck;

    if (binfp == NULL) {
        printf("Failed to read binary file.\n");
        exit(1);
    }

    while (fgets(binbuf, sizeof(binbuf), binfp) != NULL && imemcount < IMEM_SIZE) {
        int i, j;
        for (i = 0, j = 0; i < strlen(binbuf); i++){
            if (binbuf[i] != '_' && binbuf[i] != '\n') {
                binbuf[j] = binbuf[i];
                j++;
            }
        }
        binbuf[j] = '\0';
        //printf("binbuf\t%s\n", binbuf);
        uint32_t inst = strtoul(binbuf, &imemcheck, 2);

        if (*imemcheck != '\0') {
            printf("IMEM load failed: Not a binary code. '%c'\n", *imemcheck);
            exit(1);
        }
        
        cpu->IMEM[imemcount] = inst;
        cpu->IMEM[imemcount + 1] = (inst >> 8);
        cpu->IMEM[imemcount + 2] = (inst >> 16);
        cpu->IMEM[imemcount + 3] = (inst >> 24);

        printf("imem[%d]\t%x\n", imemcount, cpu->IMEM[imemcount]);
        printf("imem[%d]\t%x\n", imemcount + 1, cpu->IMEM[imemcount + 1]);
        printf("imem[%d]\t%x\n", imemcount + 2, cpu->IMEM[imemcount + 2]);
        printf("imem[%d]\t%x\n", imemcount + 3, cpu->IMEM[imemcount + 3]);
        imemcount = imemcount + 4;

    }

    printf("Number of instruction lines: %d\n", (imemcount / 4));
    printf("IMEM load complete.\n");

    fclose(binfp);
}


bool execute(struct GISA *cpu, uint32_t inst)
{
    int op = opcode(inst);
    int mode_1 = modebit_1(inst);   // inst[25]
    int mode_2 = modebit_2(inst);   // inst[24]
    bool is_finished = false;
    //printf("명령어 종류: %x\n", op);

    switch (op)
    {
    case MOV:
        if (mode_2 == 0){
            if (mode_1 == 0){
                mov(cpu, rD(inst), rB(inst));
            } else {
                movi(cpu, rD(inst), immB_20(inst));
            }
        } else {
            moviz(cpu, rD(inst), immB_20(inst));       
        }
        break;

    case ADD:
        if (mode_2 == 0){
            if (mode_1 == 0){
                add(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                addi(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                adds(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                addis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case SUB:
        if (mode_2 == 0){
            if (mode_1 == 0){
                sub(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                subi(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                subs(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                subis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case MOVH:
        if (mode_2 == 0){
            if (mode_1 == 0){
                movh(cpu, rD(inst), immB_16(inst));
            } else {
                movhl(cpu, rD(inst), immB_20(inst));
            }
        } else {
            if (mode_1 == 0){
                movhk(cpu, rD(inst), immB_16(inst));
            } else {
                movhlk(cpu, rD(inst), immB_20(inst));
            }        
        }
        break;

    case CMP:
        if (mode_2 == 0){
            if (mode_1 == 0){
                cmp(cpu, rA(inst), rB(inst));
            } else {
                cmpi(cpu, rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                cmps(cpu, rA(inst), rB(inst));
            } else {
                cmpis(cpu, rA(inst), immB_16(inst));
            }        
        }
        break;

    case MUL:
        if (mode_2 == 0){
            if (mode_1 == 0){
                mul(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                muli(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                muls(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                mulis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case MULH:
        if (mode_2 == 0){
            if (mode_1 == 0){
                mulh(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                mulhi(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                mulhs(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                mulhis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case MULHU:
        if (mode_2 == 0){
            if (mode_1 == 0){
                mulhu(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                mulhui(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                mulhus(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                mulhuis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case MULFX:
        if (mode_2 == 0){
            if (mode_1 == 0){
                mulfx(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                mulfxi(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                mulfxs(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                mulfxis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case DIV:
        if (mode_2 == 0){
            if (mode_1 == 0){
                div_op(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                divi(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                divs(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                divis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case DIVU:
        if (mode_2 == 0){
            if (mode_1 == 0){
                divu(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                divui(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                divus(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                divuis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case MOD:
        if (mode_2 == 0){
            if (mode_1 == 0){
                mod(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                modi(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                mods(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                modis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case MODU:
        if (mode_2 == 0){
            if (mode_1 == 0){
                modu(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                modui(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                modus(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                moduis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case SHL:
        if (mode_2 == 0){
            if (mode_1 == 0){
                shl(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                shli(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                shls(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                shlis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case ASR:
        if (mode_2 == 0){
            if (mode_1 == 0){
                asr(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                asri(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                asrs(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                asris(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case LSR:
        if (mode_2 == 0){
            if (mode_1 == 0){
                lsr(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                lsri(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                lsrs(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                lsris(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case ROR:
        if (mode_2 == 0){
            if (mode_1 == 0){
                ror(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                rori(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                rors(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                roris(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case ROL:
        if (mode_2 == 0){
            if (mode_1 == 0){
                rol(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                roli(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                rols(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                rolis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case AND:
        if (mode_2 == 0){
            if (mode_1 == 0){
                and(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                andi(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                ands(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                andis(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case OR:
        if (mode_2 == 0){
            if (mode_1 == 0){
                or(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                ori(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                ors(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                oris(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case XOR:
        if (mode_2 == 0){
            if (mode_1 == 0){
                xor(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                xori(cpu, rD(inst), rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                xors(cpu, rD(inst), rA(inst), rB(inst));
            } else {
                xoris(cpu, rD(inst), rA(inst), immB_16(inst));
            }        
        }
        break;

    case NOT:
        if (mode_2 == 0){
            if (mode_1 == 0){
                not(cpu, rD(inst), rB(inst));
            } else {
                noti(cpu, rD(inst), immB_20(inst));
            }
        } else {
            if (mode_1 == 0){
                nots(cpu, rD(inst), rB(inst));
            } else {
                notis(cpu, rD(inst), immB_20(inst));
            }        
        }
        break;

    case BCHK:
        if (mode_2 == 0){
            if (mode_1 == 0){
                bchk(cpu, rA(inst), rB(inst));
            } else {
                bchki(cpu, rA(inst), immB_16(inst));
            }
        } else {
            if (mode_1 == 0){
                bchks(cpu, rA(inst), rB(inst));
            } else {
                bchkis(cpu, rA(inst), immB_16(inst));
            }        
        }
        break;

    case LDR:
        if (mode_2 == 0){
            if (mode_1 == 0){
                ldr(cpu, rD(inst), rA(inst), immB_16(inst));
            } else {
                ldri(cpu, rD(inst), immB_20(inst));
            }
        } else {
            ldrr(cpu, rD(inst), immB_20(inst));
        }
        break;

    case LDRB:
        if (mode_2 == 0){
            if (mode_1 == 0){
                ldrb(cpu, rD(inst), rA(inst), immB_16(inst));
            } else {
                ldrbi(cpu, rD(inst), immB_20(inst));
            }
        } else {
            ldrbr(cpu, rD(inst), immB_20(inst));
        }
        break;

    case LDRSB:
        if (mode_2 == 0){
            if (mode_1 == 0){
                ldrsb(cpu, rD(inst), rA(inst), immB_16(inst));
            } else {
                ldrsbi(cpu, rD(inst), immB_20(inst));
            }
        } else {
            ldrsbr(cpu, rD(inst), immB_20(inst));
        }
        break;

    case LDRH:
        if (mode_2 == 0){
            if (mode_1 == 0){
                ldrh(cpu, rD(inst), rA(inst), immB_16(inst));
            } else {
                ldrhi(cpu, rD(inst), immB_20(inst));
            }
        } else {
            ldrhr(cpu, rD(inst), immB_20(inst));
        }
        break;

    case LDRSH:
        if (mode_2 == 0){
            if (mode_1 == 0){
                ldrsh(cpu, rD(inst), rA(inst), immB_16(inst));
            } else {
                ldrshi(cpu, rD(inst), immB_20(inst));
            }
        } else {
            ldrshr(cpu, rD(inst), immB_20(inst));
        }
        break;

    case STR:
        if (mode_2 == 0){
            if (mode_1 == 0){
                str(cpu, rB(inst), rA(inst), immB_16_store(inst));
            } else {
                stri(cpu, rB(inst), immB_20_store(inst));
            }
        } else {
            strr(cpu, rB(inst), immB_20_store(inst));
        }
        break;

    case STRB:
        if (mode_2 == 0){
            if (mode_1 == 0){
                strb(cpu, rB(inst), rA(inst), immB_16_store(inst));
            } else {
                strbi(cpu, rB(inst), immB_20_store(inst));
            }
        } else {
            strbr(cpu, rB(inst), immB_20_store(inst));
        }
        break;

    case STRH:
        if (mode_2 == 0){
            if (mode_1 == 0){
                strh(cpu, rB(inst), rA(inst), immB_16_store(inst));
            } else {
                strhi(cpu, rB(inst), immB_20_store(inst));
            }
        } else {
            strhr(cpu, rB(inst), immB_20_store(inst));
        }
        break;

    case B:
        b(cpu, cond(inst), immB_22_branch(inst));
        break;

    case JMP:
        if (mode_2 == 0){
            if (mode_1 == 0){
                jmp(cpu, rA(inst), immB_16(inst));
            } else {
                jmpi(cpu, immB_24_jump(inst));
            }
        } else {
            if (mode_1 == 0){
                jmpl(cpu, rD(inst), rA(inst), immB_16(inst));
            } else {
                jmpil(cpu, rD(inst), immB_20(inst));
            }        
        }
        break;

    case KILL:
        printf("KILL instruction detected. Halting simulator.\n");
        is_finished = true;
        break;
    
    default:
        printf("Undefined instruction: IMEM[%d]\t%x\n", cpu->pc, inst);
        exit(1);
        break;
    }
    //cpu->pc = cpu->pc + 4;  // 명령처리 함수들 완성하면 지울것, pc 변화는 함수 안에서 이뤄지게 만들 것!
    return is_finished;
}





int main()
{
    static struct GISA cpu;
    uint32_t inst;
    bool is_finished = false;

    start_setting(&cpu);
    printf("\n--- Initial setup complete. ---\n\n");

    while (!is_finished && cpu.pc < IMEM_SIZE){
        inst = (cpu.IMEM[cpu.pc + 3] << 24) | (cpu.IMEM[cpu.pc + 2] << 16) | (cpu.IMEM[cpu.pc + 1] << 8) | (cpu.IMEM[cpu.pc]);
        is_finished = execute(&cpu, inst);
        //printf("current inst:\t%x\n", inst);
    }

    if (is_finished) {
        printf("Finished by KILL.\n");
    }
    printf("\n--- All instruction finished. ---\n\n");

    return 0;
}



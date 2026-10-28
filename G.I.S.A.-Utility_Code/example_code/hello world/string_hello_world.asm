.global string_printer
.global main
	MOVI R1 #72
.data DATA_ID_4 1 72
	MOVI R1 #101
.data - 1 101
	MOVI R1 #108
.data - 1 108
	MOVI R1 #108
.data - 1 108
	MOVI R1 #111
.data - 1 111
	MOVI R1 #32
.data - 1 32
	MOVI R1 #87
.data - 1 87
	MOVI R1 #111
.data - 1 111
	MOVI R1 #114
.data - 1 114
	MOVI R1 #108
.data - 1 108
	MOVI R1 #100
.data - 1 100
	MOVI R1 #33
.data - 1 33
	MOVI R1 #0
.data - 1 0


JMP CPU_START

string_printer:
LABEL_1:
	STR R15 R14 #-48
	STR R13 R14 #-52
	MOV R13 R14
	ADDI R14 R13 #-56

	MOVH R7 #65535

string_printer_print_loop:
string_printer_wait_until_tdx_free:
	LDR R0 R7 #4
	CMPIS R0 #1
	BEQ string_printer_wait_until_tdx_free

	LDRB R5 R4 #0

	CMPIS R5 #0
	BEQ string_printer_print_loop_end

	STRB R5 R7 #0
	ADDI R4 R4 #1
	BAL string_printer_print_loop

string_printer_print_loop_end:
	MOVI R0 #0
	MOV R14 R13
	LDR R13 R14 #-52
	LDR R15 R14 #-48
	JMP R15

main:
LABEL_2:
	STR R15 R14 #-32
	STR R13 R14 #-36
	MOV R13 R14
	ADDI R14 R13 #-40
	MOVI R1 DATA_ID_4
	STR R1 R13 #-16
	LDR R3 R13 #-16
	MOV R4 R3
	JMPL R15 LABEL_1
	MOV R1 R0
	STR R1 R13 #-20
	MOVI R1 #0
	STR R1 R13 #-24
	LDR R3 R13 #-24
	MOV R0 R3
	MOV R14 R13
	LDR R13 R14 #-36
	LDR R15 R14 #-32
	JMP R15
	MOVI R1 #0
	STR R1 R13 #-28
	LDR R3 R13 #-28
	MOV R0 R3
	MOV R14 R13
	LDR R13 R14 #-36
	LDR R15 R14 #-32
	JMP R15














CPU_START:
    MOVI R0 #0
    MOVI R1 #0
    MOVI R2 #0
    MOVI R3 #0
    MOVI R4 #0
    MOVI R5 #0
    MOVI R6 #0
    MOVI R7 #0
    MOVI R8 #0
    MOVI R9 #0
    MOVI R10 #0
    MOVI R11 #0
    MOVI R12 #0
    MOVI R13 #4096
    MOVI R14 #4096
    MOVI R15 #0
    JMPL R15 main
    MOVI R10 #253
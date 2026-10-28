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
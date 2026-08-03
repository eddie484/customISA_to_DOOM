print_uart:
LABEL_1:
	STR R15 R14 #-48
	STR R13 R14 #-52
	MOV R13 R14
	ADDI R14 R13 #-56

	MOVH R7 #65535
wait_until_tdx_free:
	LDR R0 R7 #4
	CMPIS R0 #1
	BEQ wait_until_tdx_free

	STR R4 R7 #0

	MOVI R0 #0
	MOV R14 R13
	LDR R13 R14 #-52
	LDR R15 R14 #-48
	JMP R15
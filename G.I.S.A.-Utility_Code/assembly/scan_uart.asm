scan_uart:
LABEL_1:
	STR R15 R14 #-40
	STR R13 R14 #-44
	MOV R13 R14
	ADDI R14 R13 #-48

	MOVI R0 #256
    MOVH R7 #65535
wait_until_receive_uart:
    LDR R0 R7 #0
	CMPIS R0 #256
	BEQ wait_until_receive_uart

	MOV R14 R13
	LDR R13 R14 #-44
	LDR R15 R14 #-40
	JMP R15
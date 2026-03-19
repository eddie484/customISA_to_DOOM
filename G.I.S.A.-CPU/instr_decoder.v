`include "defines.v"

module instr_decoder (instr_in, icode_out, aluop_out, imode_out, rmode_out, lkmode_out, setcc_out, signed_out, unary_out, jump_out, branch_out, load_out, mulsel_out, rD_out, rA_out, rB_out, immB_out, cond_out, dmen_out, dmrw_out, dmsize_out, dmsext_out, wben_out);

	input [31:0] instr_in;
	
	output [5:0] icode_out;
	output reg [3:0] aluop_out;
	output imode_out, rmode_out, lkmode_out, setcc_out, signed_out, unary_out, jump_out, branch_out, load_out;
	output reg [1:0] mulsel_out;
	output [3:0] rD_out, rA_out, rB_out;
	output reg [31:0] immB_out;
	output [3:0] cond_out;
	output dmen_out, dmrw_out;
	output reg [1:0] dmsize_out;
	output dmsext_out;
	output wben_out;
	
	wire [5:0] icode;
	wire mode_A, mode_B;
	wire zmode, lgmode;
	wire store;
	wire jump, branch, load;
	
	
	assign icode = instr_in[31:26];
	assign mode_A = instr_in[25];
	assign mode_B = instr_in[24];
	assign zmode = (icode == `MOV) && mode_A && mode_B;
	assign lgmode = (icode == `MOVH) && mode_A;
	assign store = (icode == `STR || icode == `STRB || icode == `STRH);
	assign jump = (icode == `JMP);
	assign branch = (icode == `B);
	assign load = (icode == `LDR || icode == `LDRB || icode == `LDRSB || icode == `LDRH || icode == `LDRSH);
	
	assign icode_out = icode;
	assign imode_out = mode_A || load || branch || (jump && mode_A) || store || (icode == `MOVH);
	assign rmode_out = ((load || store) && mode_B) || branch;
	assign lkmode_out = jump && mode_B;
	assign setcc_out = mode_B;
	assign signed_out = (icode == `MULH || icode == `MULFX || icode == `DIV || icode == `MOD || icode == `ASR);
	assign unary_out = (icode == `MOV || icode == `MOVH || icode == `NOT) || ((load || store) && mode_A) || (jump && mode_B);
	assign jump_out = jump;
	assign branch_out = branch;
	assign load_out = load;
	assign rD_out = instr_in[23:20];
	assign rA_out = instr_in[19:16];
	assign rB_out = instr_in[15:12];
	assign cond_out = instr_in[3:0];
	assign dmen_out = load || store;
	assign dmrw_out = store;
	assign dmsext_out =  (icode == `LDRSB || icode == `LDRSH);
	assign wben_out =(icode != `CMP && icode != `BCHK) && !store && !branch && !(jump && !mode_B);
	

	
	
	wire [31:0] immB_u16;
	wire [31:0] immB_s16;
	wire [31:0] immB_u20;
	wire [31:0] immB_s20;
	wire [31:0] immB_h16;
	wire [31:0] immB_h20;
	wire [31:0] immB_strs16;
	wire [31:0] immB_stru20;
	wire [31:0] immB_strs20;
	wire [31:0] immB_b22;
	wire [31:0] immB_j24;

	
	assign immB_u16 = {16'b0, instr_in[15:0]};
	assign immB_s16 = {{16{instr_in[15]}}, instr_in[15:0]};
	assign immB_u20 = {12'b0, instr_in[19:0]};
	assign immB_s20 = {{12{instr_in[19]}}, instr_in[19:0]};
	assign immB_h16 = {instr_in[15:0], 16'b0};
	assign immB_h20 = {instr_in[19:0], 12'b0};
	assign immB_strs16 = {{16{instr_in[23]}}, instr_in[23:20], instr_in[11:0]};
	assign immB_stru20 = {12'b0, instr_in[23:16], instr_in[11:0]};
	assign immB_strs20 = {{12{instr_in[23]}}, instr_in[23:16], instr_in[11:0]};
	assign immB_b22 = {{10{instr_in[25]}}, instr_in[25:4]};
	assign immB_j24 = {8'b0, instr_in[23:0]};
	
	
	always @(*) begin
		case (icode)
			`MOV, `ADD, `MOVH, `LDR, `LDRB, `LDRSB, `LDRH, `LDRSH, `STR, `STRB, `STRH, `B, `JMP: aluop_out = `add;
			`SUB, `CMP: aluop_out = `sub;
			`MUL, `MULH, `MULHU, `MULFX: aluop_out = `mul;
			`DIV, `DIVU: aluop_out = `div;
			`MOD, `MODU: aluop_out = `mod;
			`SHL: aluop_out = `shl;
			`ASR, `LSR: aluop_out = `shr;
			`ROL: aluop_out = `rol;
			`ROR: aluop_out = `ror;
			`AND, `BCHK: aluop_out = `and;
			`OR: aluop_out = `or;
			`XOR: aluop_out = `xor;
			`NOT: aluop_out = `not;
			default: aluop_out = 4'b0;
		endcase
	
		case (icode)
			`MUL: mulsel_out = 2'b00;
			`MULH, `MULHU: mulsel_out = 2'b01;
			`MULFX: mulsel_out = 2'b10;
			default: mulsel_out = 2'b00;
		endcase
		
		case (icode)
			`MOV: immB_out = zmode ? immB_u20 : immB_s20;
			`ADD, `SUB, `CMP, `MUL, `MULH, `MULFX, `DIV, `MOD: immB_out = immB_s16;
			`MOVH: immB_out = lgmode ? immB_h20 : immB_h16;
			`MULHU, `DIVU, `MODU, `SHL, `ASR, `LSR, `ROL, `ROR, `AND, `OR, `XOR, `BCHK: immB_out = immB_u16;
			`NOT: immB_out = immB_u20;
			`LDR, `LDRB, `LDRSB, `LDRH, `LDRSH: begin
				if (mode_A) immB_out = immB_u20;
				else if (mode_B) immB_out = immB_s20;
				else immB_out = immB_s16;
			end
			`STR, `STRB, `STRH: begin
				if (mode_A) immB_out = immB_stru20;
				else if (mode_B) immB_out = immB_strs20;
				else immB_out = immB_strs16;
			end
			`B: immB_out = immB_b22;
			`JMP: immB_out = mode_B ? immB_u20 : immB_j24;
			default: immB_out = 32'b0;
		endcase
		
		case(icode)
			`LDR, `STR: dmsize_out = 2'b00;
			`LDRB, `LDRSB, `STRB: dmsize_out = 2'b01;
			`LDRH, `LDRSH, `STRH: dmsize_out = 2'b00;
			default: dmsize_out = 2'b00;
		endcase
	end
	
endmodule
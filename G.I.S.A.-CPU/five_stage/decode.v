(* keep_hierarchy *) module decode (clk, nRESET, instr_in, pc_in, icode_out, load_out, dmen_out, dmrw_out, aluop_out, sign_out, mulsel_out, lk_out, valA_out, valB_out, wdata_out, dmsize_out, dmsext_out, setcc_out, cond_out, branch_out, rD_out, wben_out, jump_out, rA_out, rB_out, wben_W, dst_W, valD_W, fwd_E, fwd_M, rD_E, rD_M, wben_E, wben_M, reg_0, reg_1, reg_2, reg_3, reg_4, reg_5, reg_6, reg_7, reg_8, reg_9, reg_10, reg_11, reg_12, reg_13, reg_14, reg_15);

	input clk, nRESET;
	input [31:0] instr_in, pc_in;
	
	// reg write를 위한 input
	input [31:0] valD_W;
	input [3:0] dst_W;
	input wben_W;
	
	// forwarding을 위한 input
	input [31:0] fwd_E, fwd_M;
	input [3:0] rD_E, rD_M;
	input wben_E, wben_M;

	(* keep *) output [31:0] valA_out, valB_out, wdata_out;
	(* keep *) output [5:0] icode_out;
	(* keep *) output [3:0] aluop_out, cond_out, rD_out, rA_out, rB_out;
	(* keep *) output [1:0] mulsel_out, dmsize_out;
	(* keep *) output load_out, dmen_out, dmrw_out, sign_out, lk_out, dmsext_out, setcc_out, branch_out, wben_out, jump_out;
	output [31:0] reg_0, reg_1, reg_2, reg_3, reg_4, reg_5, reg_6, reg_7, reg_8, reg_9, reg_10, reg_11, reg_12, reg_13, reg_14, reg_15;
	
	wire [31:0] immB;
	wire [3:0] rA, rB;
	wire unary, rmode, imode;
	wire [31:0] valA, valB;
	wire [31:0] fwdA, fwdB;
	
	
	instr_decoder instr_decoder_module (instr_in, icode_out, aluop_out, imode, rmode, lk_out, setcc_out, sign_out, unary, jump_out, branch_out, load_out, mulsel_out, rD_out, rA, rB, immB, cond_out, dmen_out, dmrw_out, dmsize_out, dmsext_out, wben_out);
	reg_file reg_file_module (clk, nRESET, rA, rB, wben_W, dst_W, valD_W, valA, valB, reg_0, reg_1, reg_2, reg_3, reg_4, reg_5, reg_6, reg_7, reg_8, reg_9, reg_10, reg_11, reg_12, reg_13, reg_14, reg_15);
	
	assign fwdA = 
		((rA == rD_E) && wben_E) ? fwd_E :
		((rA == rD_M) && wben_M) ? fwd_M :
		((rA == dst_W) && wben_W) ? valD_W :
		valA;
		
	assign fwdB = 
		((rB == rD_E) && wben_E) ? fwd_E :
		((rB == rD_M) && wben_M) ? fwd_M :
		((rB == dst_W) && wben_W) ? valD_W :
		valB;
	
	assign valA_out = (unary ? 32'b0 : (rmode ? pc_in : fwdA));
	assign valB_out = (imode ? immB : fwdB);
	assign wdata_out = fwdB;
	assign rA_out = rA;
	assign rB_out = rB;
	
endmodule
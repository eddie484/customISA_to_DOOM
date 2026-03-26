module decode (clk, nRESET, instr_in, pc_in, icode_out, load_out, dmen_out, dmrw_out, aluop_out, sign_out, mulsel_out, lk_out, valA_out, valB_out, wdata_out, dmsize_out, dmsext_out, setcc_out, cond_out, branch_out, rD_out, wben_out, jump_out, wben_W, dst_W, valD_W);

	input clk, nRESET;
	input [31:0] instr_in, pc_in;
	input [31:0] valD_W;
	input [3:0] dst_W;
	input wben_W;

	output [31:0] valA_out, valB_out, wdata_out;
	output [5:0] icode_out;
	output [3:0] aluop_out, cond_out, rD_out;
	output [1:0] mulsel_out, dmsize_out;
	output load_out, dmen_out, dmrw_out, sign_out, lk_out, dmsext_out, setcc_out, branch_out, wben_out, jump_out;
	
	wire [31:0] immB;
	wire [3:0] rA, rB;
	wire unary, rmode, imode;
	wire [31:0] valA, valB;
	
	
	instr_decoder instr_decoder_module (instr_in, icode_out, aluop_out, imode, rmode, lk_out, setcc_out, sign_out, unary, jump_out, branch_out, load_out, mulsel_out, rD_out, rA, rB, immB, cond_out, dmen_out, dmrw_out, dmsize_out, dmsext_out, wben_out);
	reg_file reg_file_module (clk, nRESET, rA, rB, wben_W, dst_W, valD_W, valA, valB);
	
	assign valA_out = (unary ? 32'b0 : (rmode ? pc_in : valA));
	assign valB_out = (imode ? immB : valB);
	assign wdata_out = valB;
	
endmodule
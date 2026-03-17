`include "defines.v"

module GISA_CPU (clk, nRESET, result);
	
	input clk, nRESET;
	
	output [31:0] result;
	
	reg [31:0] pc_reg;
	
	wire [31:0] pc_F, nextpc_F, pcplus4_F, instr_F;
	
	wire [31:0] valA_D, valB_D, wdata_D, pcplus4_D;
	wire [5:0] icode_D;
	wire [3:0] aluop_D, cond_D, rD_D;
	wire [1:0] mulsel_D, dmsize_D;
	wire load_D, dmen_D, dmrw_D, sign_D, lk_D, dmsext_D, setcc_D, branch_D, wben_D, jump_D;

	wire [31:0] valE_E, wdata_E, pcplus4_E;
	wire [5:0] icode_E;
	wire [3:0] rD_E;
	wire [1:0] dmsize_E;
	wire load_E, dmen_E, dmrw_E, lk_E, dmsext_E, wben_E, taken_E;
	
	wire [31:0] valE_M, valM_M, pcplus4_M;
	wire [5:0] icode_M;
	wire [3:0] rD_M;
	wire load_M, lk_M, wben_M;
	
	wire [31:0] valD_W;
	wire [3:0] rD_W;
	wire wben_W;
	
	assign result = valE_E;
	
	fetch F (pc_reg, pc_F, nextpc_F, pcplus4_F, instr_F);
	
	decode D (clk, nRESET, instr_F, pc_F, pcplus4_F, icode_D, load_D, dmen_D, dmrw_D, aluop_D, sign_D, mulsel_D, lk_D, valA_D, valB_D, wdata_D, dmsize_D, dmsext_D, setcc_D, cond_D, branch_D, pcplus4_D, rD_D, wben_D, jump_D, wben_W, rD_W, valD_W);

	execute E (clk, nRESET, icode_D, load_D, dmen_D, dmrw_D, aluop_D, sign_D, mulsel_D, lk_D, valA_D, valB_D, wdata_D, dmsize_D, dmsext_D, setcc_D, cond_D, branch_D, pcplus4_D, rD_D, wben_D, icode_E, load_E, dmen_E, dmrw_E, lk_E, valE_E, wdata_E, dmsize_E, dmsext_E, pcplus4_E, rD_E, wben_E, taken_E);

	memory M (clk, nRESET, icode_E, load_E, dmen_E, dmrw_E, lk_E, valE_E, wdata_E, dmsize_E, dmsext_E, pcplus4_E, rD_E, wben_E, icode_M, load_M, lk_M, valE_M, valM_M, pcplus4_M, rD_M, wben_M);

	write_back W (icode_M, load_M, lk_M, valE_M, valM_M, pcplus4_M, rD_M, wben_M, valD_W, rD_W, wben_W);
	
	always @(posedge clk) begin
	if (!nRESET)
		pc_reg <= 32'b0;
	else 
		pc_reg <= taken_E ? valE_E : (jump_D ? valB_D : nextpc_F);
	end

endmodule
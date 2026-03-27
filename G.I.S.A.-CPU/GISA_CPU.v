`include "defines.v"

module GISA_CPU (clk, nRESET);
	
	input clk, nRESET;
	
	
	wire [31:0] pc_computed;
	
	

	// ----- Pipeline Controller -----
	
	wire stall_F, stall_D;
	wire bubble_E;
	
	wire [5:0] icode_E;
	wire [3:0] rA_D, rB_D, rD_E;
	
	
	pipeline_controller pipecon_module (icode_E, rA_D, rB_D, rD_E, stall_F, stall_D, bubble_E);
	
	
	
	// ----- Fetch Level -----
	
	wire [31:0] pc_F, nextpc_F, pcplus4_F, instr_F;
	
	
	pipereg_F PR_F (clk, nRESET, stall_F, pc_computed, pc_F);
	
	fetch F (pc_F, nextpc_F, pcplus4_F, instr_F);
	
	
	
	// ----- Decode Level -----
		
	wire [31:0] valA_D, valB_D, wdata_D, pcplus4_D, pc_D, instr_D;
	wire [5:0] icode_D;
	wire [3:0] aluop_D, cond_D, rD_D;
	wire [1:0] mulsel_D, dmsize_D;
	wire load_D, dmen_D, dmrw_D, sign_D, lk_D, dmsext_D, setcc_D, branch_D, wben_D, jump_D;
	
	// reg write를 위한 와이어
	wire [31:0] valD_W;
	wire [3:0] rD_W;
	wire wben_W;
	
	// data forwarding을 위한 와이어
	wire [31:0] fwd_E, fwd_M;
	wire [3:0] rD_M;
	wire wben_E, wben_M;

	
	pipereg_D PR_D (clk, nRESET, stall_D, pc_F, instr_F, PCplus4_F, pc_D, instr_D, PCplus4_D);
	
	decode D (clk, nRESET, instr_D, pc_D, icode_D, load_D, dmen_D, dmrw_D, aluop_D, sign_D, mulsel_D, lk_D, valA_D, valB_D, wdata_D, dmsize_D, dmsext_D, setcc_D, cond_D, branch_D, rD_D, wben_D, jump_D, rA_D, rB_D, wben_W, rD_W, valD_W, fwd_E, fwd_M, rD_E, rD_M, wben_E, wben_M);

	
	
	// ----- Execute Level -----
	
	wire [31:0] valA_E, valB_E, valE_E, wdata_E, pcplus4_E;
	wire [3:0] aluop_E, cond_E;
	wire [1:0] mulsel_E, dmsize_E;
	wire load_E, dmen_E, dmrw_E, sign_E, lk_E, dmsext_E, setcc_E, branch_E, taken_E;
	
	
	pipereg_E PR_E (clk, nRESET, bubble_E, icode_D, load_D, dmen_D, dmrw_D, aluop_D, sign_D, mulsel_D, lk_D, valA_D, valB_D, wdata_D, dmsize_D, dmsext_D, setcc_D, cond_D, branch_D, PCplus4_D, rD_D, wben_D, icode_E, load_E, dmen_E, dmrw_E, aluop_E, sign_E, mulsel_E, lk_E, valA_E, valB_E, wdata_E, dmsize_E, dmsext_E, setcc_E, cond_E, branch_E, PCplus4_E, rD_E, wben_E);
	
	execute E (clk, nRESET, aluop_E, sign_E, mulsel_E, lk_E, valA_E, valB_E, setcc_E, cond_E, branch_E, pcplus4_E, valE_E, taken_E, fwd_E);

	
	
	// ----- Memory Level -----
	
	wire [31:0] valE_M, valM_M, wdata_M, pcplus4_M;
	wire [5:0] icode_M;
	wire [1:0] dmsize_M;
	wire load_M, dmen_M, dmrw_M, lk_M, dmsext_M;
	
	
	pipereg_M PR_M (clk, nRESET, icode_E, load_E, dmen_E, dmrw_E, lk_E, valE_E, wdata_E, dmsize_E, dmsext_E, PCplus4_E, rD_E, wben_E, icode_M, load_M, dmen_M, dmrw_M, lk_M, valE_M, wdata_M, dmsize_M, dmsext_M, PCplus4_M, rD_M, wben_M);

	memory M (clk, nRESET, load_M, dmen_M, dmrw_M, lk_M, valE_M, wdata_M, dmsize_M, dmsext_M, pcplus4_M, valM_M, fwd_M);

	
	
	// ----- Write Level -----	
	
	wire [31:0] valE_W, valM_W, PCplus4_W;
	wire [5:0] icode_W;
	wire load_W, lk_W;
	
	pipereg_W PR_W (clk, nRESET, icode_M, load_M, lk_M, valE_M, valM_M, PCplus4_M, rD_M, wben_M, icode_W, load_W, lk_W, valE_W, valM_W, PCplus4_W, rD_W, wben_W);

	write_back W (load_W, lk_W, valE_W, valM_W, pcplus4_W, valD_W);
	
	
	
	// ----- PC control -----
	
	assign pc_computed = taken_E ? valE_E : (jump_D ? valB_D : nextpc_F);
	

endmodule
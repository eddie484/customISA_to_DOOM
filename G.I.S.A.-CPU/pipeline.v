`include "defines.v"


(* keep_hierarchy *) module pipeline_controller (icode_D, icode_E, rA_D, rB_D, rD_E, mispred_E, mul_finished_E, stall_F, stall_D, stall_E, bubble_D, bubble_E, bubble_M);

	input [5:0] icode_D, icode_E;
	input [3:0] rA_D, rB_D, rD_E;
	input mispred_E, mul_finished_E;
	
	(* keep *) output stall_F, stall_D, stall_E;
	(* keep *) output bubble_D, bubble_E, bubble_M;
	
	
	assign stall_F = ((icode_E == `LDR || icode_E == `LDRB || icode_E == `LDRSB || icode_E == `LDRH || icode_E == `LDRSH) && (rD_E == rA_D || rD_E == rB_D)) || ((icode_E == `MUL || icode_E == `MULH || icode_E == `MULHU || icode_E == `MULFX) && (!mul_finished_E));
	assign stall_D = ((icode_E == `LDR || icode_E == `LDRB || icode_E == `LDRSB || icode_E == `LDRH || icode_E == `LDRSH) && (rD_E == rA_D || rD_E == rB_D)) || ((icode_E == `MUL || icode_E == `MULH || icode_E == `MULHU || icode_E == `MULFX) && (!mul_finished_E));
	assign stall_E = ((icode_E == `MUL || icode_E == `MULH || icode_E == `MULHU || icode_E == `MULFX) && (!mul_finished_E));
	assign bubble_D = (mispred_E || (icode_D == `JMP)) && (!stall_D);
	assign bubble_E = (((icode_E == `LDR || icode_E == `LDRB || icode_E == `LDRSB || icode_E == `LDRH || icode_E == `LDRSH) && (rD_E == rA_D || rD_E == rB_D)) || mispred_E) && (!stall_E);
	assign bubble_M = ((icode_E == `MUL || icode_E == `MULH || icode_E == `MULHU || icode_E == `MULFX) && (!mul_finished_E));
endmodule



(* keep_hierarchy *) module pipereg_F (clk, nRESET, stall_F, nextPC, PC_F);

	input clk, nRESET, stall_F;
	input [31:0] nextPC;
	
	(* keep *) output reg [31:0] PC_F;
	
	
	always @(posedge clk)
		if (!nRESET) 
			PC_F <= 32'b0;
		else if (!stall_F) 
			PC_F <= nextPC;
		
endmodule



(* keep_hierarchy *) module pipereg_D (clk, nRESET, stall_D, bubble_D, PC_F, IR_F, PCplus4_F, PC_D, IR_D, PCplus4_D);

	input clk, nRESET, stall_D, bubble_D;
	input [31:0] PC_F, IR_F, PCplus4_F;
	
	(* keep *) output reg [31:0] PC_D, PCplus4_D;
	(* keep *) output [31:0] IR_D;
	
	
	always @(posedge clk)
		if (!nRESET || bubble_D) begin
			PC_D <= 32'b0;
			PCplus4_D <= 32'b0;
		end else if (!stall_D) begin
			PC_D <= PC_F;
			PCplus4_D <= PCplus4_F;
		end
		
	assign IR_D = IR_F;
		
endmodule



(* keep_hierarchy *) module pipereg_E (clk, nRESET, stall_E, bubble_E, icode_D, load_D, dmen_D, dmrw_D, aluop_D, sign_D, mulsel_D, lk_D, valA_D, valB_D, wdata_D, dmsize_D, dmsext_D, setcc_D, cond_D, branch_D, PCplus4_D, rD_D, wben_D, icode_E, load_E, dmen_E, dmrw_E, aluop_E, sign_E, mulsel_E, lk_E, valA_E, valB_E, wdata_E, dmsize_E, dmsext_E, setcc_E, cond_E, branch_E, PCplus4_E, rD_E, wben_E);

	input clk, nRESET, stall_E, bubble_E;

	input [31:0] valA_D, valB_D, wdata_D, PCplus4_D;
	input [5:0] icode_D;
	input [3:0] aluop_D, cond_D, rD_D;
	input [1:0] mulsel_D, dmsize_D;
	input load_D, dmen_D, dmrw_D, sign_D, lk_D, dmsext_D, setcc_D, branch_D, wben_D;

	(* keep *) output reg [31:0] valA_E, valB_E, wdata_E, PCplus4_E;
	(* keep *) output reg [5:0] icode_E;
	(* keep *) output reg [3:0] aluop_E, cond_E, rD_E;
	(* keep *) output reg [1:0] mulsel_E, dmsize_E;
	(* keep *) output reg load_E, dmen_E, dmrw_E, sign_E, lk_E, dmsext_E, setcc_E, branch_E, wben_E;

	
	always @(posedge clk)
		if (!nRESET || bubble_E) begin
			valA_E <= 32'b0;
			valB_E <= 32'b0;
			wdata_E <= 32'b0;
			PCplus4_E <= 32'b0;
			icode_E <= 6'b0;
			aluop_E <= 4'b0;
			cond_E <= 4'b0;
			rD_E <= 4'b0;
			mulsel_E <= 2'b0;
			dmsize_E <= 2'b0;
			load_E <= 1'b0;
			dmen_E <= 1'b0;
			dmrw_E <= 1'b0;
			sign_E <= 1'b0;
			lk_E <= 1'b0;
			dmsext_E <= 1'b0;
			setcc_E <= 1'b0;
			branch_E <= 1'b0;
			wben_E <= 1'b0;
		end else if (!stall_E) begin
			valA_E <= valA_D;
			valB_E <= valB_D;
			wdata_E <= wdata_D;
			PCplus4_E <= PCplus4_D;
			icode_E <= icode_D;
			aluop_E <= aluop_D;
			cond_E <= cond_D;
			rD_E <= rD_D;
			mulsel_E <= mulsel_D;
			dmsize_E <= dmsize_D;
			load_E <= load_D;
			dmen_E <= dmen_D;
			dmrw_E <= dmrw_D;
			sign_E <= sign_D;
			lk_E <= lk_D;
			dmsext_E <= dmsext_D;
			setcc_E <= setcc_D;
			branch_E <= branch_D;
			wben_E <= wben_D;
		end
		
endmodule



(* keep_hierarchy *) module pipereg_M (clk, nRESET, bubble_M, icode_E, load_E, dmen_E, dmrw_E, lk_E, valE_E, wdata_E, dmsize_E, dmsext_E, PCplus4_E, rD_E, wben_E, icode_M, load_M, dmen_M, dmrw_M, lk_M, valE_M, wdata_M, dmsize_M, dmsext_M, PCplus4_M, rD_M, wben_M);

	input clk, nRESET, bubble_M;
	
	input [31:0] valE_E, wdata_E, PCplus4_E;
	input [5:0] icode_E;
	input [3:0] rD_E;
	input [1:0] dmsize_E;
	input load_E, dmen_E, dmrw_E, lk_E, dmsext_E, wben_E;
	
	(* keep *) output reg [31:0] valE_M, wdata_M, PCplus4_M;
	(* keep *) output reg [5:0] icode_M;
	(* keep *) output reg [3:0] rD_M;
	(* keep *) output reg [1:0] dmsize_M;
	(* keep *) output reg load_M, dmen_M, dmrw_M, lk_M, dmsext_M, wben_M;

	
	always @(posedge clk)
		if (!nRESET || bubble_M) begin
			valE_M <= 32'b0;
			wdata_M <= 32'b0;
			PCplus4_M <= 32'b0;
			icode_M <= 6'b0;
			rD_M <= 4'b0;
			dmsize_M <= 2'b0;
			load_M <= 1'b0;
			dmen_M <= 1'b0;
			dmrw_M <= 1'b0;
			lk_M <= 1'b0;
			dmsext_M <= 1'b0;
			wben_M <= 1'b0;
		end else begin
			valE_M <= valE_E;
			wdata_M <= wdata_E;
			PCplus4_M <= PCplus4_E;
			icode_M <= icode_E;
			rD_M <= rD_E;
			dmsize_M <= dmsize_E;
			load_M <= load_E;
			dmen_M <= dmen_E;
			dmrw_M <= dmrw_E;
			lk_M <= lk_E;
			dmsext_M <= dmsext_E;
			wben_M <= wben_E;
		end
		
endmodule



(* keep_hierarchy *) module pipereg_W (clk, nRESET, icode_M, load_M, lk_M, valE_M, valM_M, PCplus4_M, rD_M, wben_M, icode_W, load_W, lk_W, valE_W, valM_W, PCplus4_W, rD_W, wben_W);

	input clk, nRESET;
	
	input [31:0] valE_M, valM_M, PCplus4_M;
	input [5:0] icode_M;
	input [3:0] rD_M;
	input load_M, lk_M, wben_M;
	
	(* keep *) output [31:0] valM_W;
	
	(* keep *) output reg [31:0] valE_W, PCplus4_W;
	(* keep *) output reg [5:0] icode_W;
	(* keep *) output reg [3:0] rD_W;
	(* keep *) output reg load_W, lk_W, wben_W;

	
	always @(posedge clk)
		if (!nRESET) begin
			valE_W <= 32'b0;
			PCplus4_W <= 32'b0;
			icode_W <= 6'b0;
			rD_W <= 4'b0;
			load_W <= 1'b0;
			lk_W <= 1'b0;
			wben_W <= 1'b0;
		end else begin
			valE_W <= valE_M;
			PCplus4_W <= PCplus4_M;
			icode_W <= icode_M;
			rD_W <= rD_M;
			load_W <= load_M;
			lk_W <= lk_M;
			wben_W <= wben_M;
		end
		
	assign valM_W = valM_M;
		
endmodule
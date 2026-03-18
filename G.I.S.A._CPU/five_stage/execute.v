`include "v/defines.v"

module execute (clk, nRESET, icode_in, load_in, dmen_in, dmrw_in, aluop_in, sign_in, mulsel_in, lk_in, valA_in, valB_in, wdata_in, dmsize_in, dmsext_in, setcc_in, cond_in, branch_in, pcplus4_in, rD_in, wben_in, icode_out, load_out, dmen_out, dmrw_out, lk_out, valE_out, wdata_out, dmsize_out, dmsext_out, pcplus4_out, rD_out, wben_out, taken_out);

	input clk, nRESET;
	input [31:0] valA_in, valB_in, wdata_in, pcplus4_in;
	input [5:0] icode_in;
	input [3:0] aluop_in, cond_in, rD_in;
	input [1:0] mulsel_in, dmsize_in;
	input load_in, dmen_in, dmrw_in, sign_in, lk_in, dmsext_in, setcc_in, branch_in, wben_in;
	
	output [31:0] valE_out, wdata_out, pcplus4_out;
	output [5:0] icode_out;
	output [3:0] rD_out;
	output [1:0] dmsize_out;
	output load_out, dmen_out, dmrw_out, lk_out, dmsext_out, wben_out;
	output reg taken_out;
	
	wire [3:0] alucc, nzcv;
	
	
	ALU ALU_module (valA_in, valB_in, sign_in, aluop_in, mulsel_in, valE_out, alucc);
	nzcv_reg nzcv_module (clk, nRESET, alucc, setcc_in, nzcv);
		
	
	assign icode_out = icode_in;
	assign load_out = load_in;
	assign dmen_out = dmen_in;
	assign dmrw_out = dmrw_in;
	assign lk_out = lk_in;
	assign wdata_out = wdata_in;
	assign dmsize_out = dmsize_in;
	assign dmsext_out = dmsext_in;
	assign pcplus4_out = pcplus4_in;
	assign rD_out = rD_in;
	assign wben_out = wben_in;
	
	
	always @(*) begin
		if (branch_in) begin
			case (cond_in)
				`EQ: taken_out = (nzcv[2] == 1'b1);
				`NE: taken_out = (nzcv[2] == 1'b0);
				`CS, `HS: taken_out = (nzcv[1] == 1'b1);
				`CC, `LO: taken_out = (nzcv[1] == 1'b0);
				`MI: taken_out = (nzcv[3] == 1'b1);
				`PL: taken_out = (nzcv[3] == 1'b0);
				`VS: taken_out = (nzcv[0] == 1'b1);
				`VC: taken_out = (nzcv[0] == 1'b0);
				`HI: taken_out = ((nzcv[1] == 1'b1) && (nzcv[2] == 1'b0));
				`LS: taken_out = ((nzcv[1] == 1'b0) || (nzcv[2] == 1'b1));
				`GE: taken_out = (nzcv[3] == nzcv[0]);
				`LT: taken_out = (nzcv[3] != nzcv[0]);
				`GT: taken_out = ((nzcv[2] == 1'b0) && (nzcv[3] == nzcv[0]));
				`LE: taken_out = ((nzcv[2] == 1'b1) || (nzcv[3] != nzcv[0]));
				`AL: taken_out = 1'b1;
				default: taken_out = 1'b0;
			endcase		
		end else
			taken_out = 1'b0;
	end

endmodule
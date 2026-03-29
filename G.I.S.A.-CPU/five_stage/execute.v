`include "v/defines.v"

module execute (clk, nRESET, aluop_in, sign_in, mulsel_in, lk_in, valA_in, valB_in, setcc_in, cond_in, branch_in, PCplus4_in, valE_out, mispred_out, fwd_out);

	input clk, nRESET;
	input [31:0] valA_in, valB_in, PCplus4_in;
	input [3:0] aluop_in, cond_in;
	input [1:0] mulsel_in;
	input sign_in, lk_in, setcc_in, branch_in;
	
	output [31:0] valE_out, fwd_out;
	output reg mispred_out;
	
	wire [3:0] alucc, nzcv;
	reg taken;
	
	
	ALU ALU_module (valA_in, valB_in, sign_in, aluop_in, mulsel_in, valE_out, alucc);
	nzcv_reg nzcv_module (clk, nRESET, alucc, setcc_in, nzcv);
	
	assign fwd_out = lk_in ? PCplus4_in : valE_out;
	//assign mispred_out = (!taken && branch_in);
	
	
	
	always @(*) begin
		if (branch_in) begin
			case (cond_in)
				`EQ: taken = (nzcv[2] == 1'b1);
				`NE: taken = (nzcv[2] == 1'b0);
				`CS, `HS: taken = (nzcv[1] == 1'b1);
				`CC, `LO: taken = (nzcv[1] == 1'b0);
				`MI: taken = (nzcv[3] == 1'b1);
				`PL: taken = (nzcv[3] == 1'b0);
				`VS: taken = (nzcv[0] == 1'b1);
				`VC: taken = (nzcv[0] == 1'b0);
				`HI: taken = ((nzcv[1] == 1'b1) && (nzcv[2] == 1'b0));
				`LS: taken = ((nzcv[1] == 1'b0) || (nzcv[2] == 1'b1));
				`GE: taken = (nzcv[3] == nzcv[0]);
				`LT: taken = (nzcv[3] != nzcv[0]);
				`GT: taken = ((nzcv[2] == 1'b0) && (nzcv[3] == nzcv[0]));
				`LE: taken = ((nzcv[2] == 1'b1) || (nzcv[3] != nzcv[0]));
				`AL: taken = 1'b1;
				default: taken = 1'b0;
			endcase		
			mispred_out = (!taken && branch_in);
		end else begin
			taken = 1'b0;
			mispred_out = 1'b0;
		end
	end

endmodule
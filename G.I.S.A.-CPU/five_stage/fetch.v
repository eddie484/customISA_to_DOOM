module fetch (pc_in, predPC_out, PCplus4_out, instr_out);
	
	input [31:0] pc_in;
	
	output [31:0] predPC_out, PCplus4_out, instr_out;
	
	wire [31:0] instr, PCplus4, predPC;
	
	
	imem imem_module (pc_in, instr);
	branch_predictor bp_module (pc_in, instr[31:26], {{10{instr[25]}}, instr[25:4]}, PCplus4, predPC);
	
	assign predPC_out = predPC;
	assign PCplus4_out = PCplus4;
	assign instr_out = instr;
	
endmodule
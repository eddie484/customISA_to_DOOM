`include "defines.v"

module branch_predictor (pc_in, icode_in, immB_in, PCplus4_out, predPC_out);

	input [31:0] pc_in, immB_in;
	input [5:0] icode_in;
	
	output [31:0] PCplus4_out, predPC_out;
	
	wire [31:0] PCplus4, PCplusImmB;
	
	
	adder_subtractor pc_4_adder (pc_in, 32'd4, 1'b0, , PCplus4);
	adder_subtractor pc_immB_adder (pc_in, immB_in, 1'b0, , PCplusImmB);
	
	assign PCplus4_out = PCplus4;
	assign predPC_out = (icode_in == `B) ? PCplusImmB : PCplus4;

endmodule
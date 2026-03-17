module fetch (pc_in, pc_out, nextpc_out, pcplus4_out, instr_out);
	
	input [31:0] pc_in;
	
	output [31:0] pc_out, nextpc_out, pcplus4_out, instr_out;
	
	wire [31:0] instr, pcplus4;
	
	
	imem imem_module (pc_in, instr);
	adder_subtractor pc_adder (pc_in, 32'd4, 1'b0, , pcplus4);
	
	assign pc_out = pc_in;
	assign nextpc_out = pcplus4;
	assign pcplus4_out = pcplus4;
	assign instr_out = instr;
	
endmodule
(* keep_hierarchy *) module imem (clk, nRESET, stall_F, pc_in, instr_out);

	input clk, nRESET, stall_F;
	
	input [31:0] pc_in;
	
	(* keep *) output [31:0] instr_out;
	
	//reg [7:0] imem [0:1023];
	wire [31:0] imem_read;
	
	
	imem_ram_1port imem_ram_1port_module (.data(32'b0), .address(pc_in[12:2]), .wren(1'b0), .clock(clk), .clken(!stall_F), .q(imem_read));

	assign instr_out = imem_read;


endmodule
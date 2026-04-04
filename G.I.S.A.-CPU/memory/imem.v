(* keep_hierarchy *) module imem (clk, nRESET, stall_D, bubble_D, pc_in, instr_out);

	input clk, nRESET, stall_D, bubble_D;
	
	input [31:0] pc_in;
	
	(* keep *) output [31:0] instr_out;
	
	//reg [7:0] imem [0:1023];
	wire [31:0] imem_read;
	reg bubble_D_next;
	
	
	imem_ram_1port imem_ram_1port_module (.data(32'b0), .address(pc_in[12:2]), .wren(1'b0), .clock(clk), .clken(!stall_D), .q(imem_read));
/*
	always @(posedge clk) begin
		if (!nRESET || bubble_D)
			instr_out <= 32'b0;
		else if (!stall_D)
			instr_out = {imem[pc_in + 3], imem[pc_in + 2], imem[pc_in + 1], imem[pc_in]};
	end*/
	
	assign instr_out = (!nRESET || bubble_D_next) ? 32'b0 : imem_read;
	
	always @(posedge clk)
		if (!nRESET) bubble_D_next <= 1'b0;
		else if (!stall_D) bubble_D_next <= bubble_D;
	

endmodule
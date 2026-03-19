module imem (pc_in, instr_out);

	input [31:0] pc_in;
	
	output reg [31:0] instr_out;
	
	reg [7:0] imem [0:1023];
	
	
	always @(*) begin
		instr_out = {imem[pc_in + 3], imem[pc_in + 2], imem[pc_in + 1], imem[pc_in]};
	end

endmodule
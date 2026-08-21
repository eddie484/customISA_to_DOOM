module msb_shifter (in, out, amount);

	input [31:0] in;
	(* keep *) output [31:0] out;
	(* keep *) output [4:0] amount;
	
	wire [31:0] s16 = (in[31:16] == 16'b0) ? {in[15:0], 16'b0} : in;
	wire [31:0] s8 = (s16[31:24] == 8'b0) ? {s16[23:0], 8'b0} : s16;
	wire [31:0] s4 = (s8[31:28] == 4'b0) ? {s8[27:0], 4'b0} : s8;
	wire [31:0] s2 = (s4[31:30] == 2'b0) ? {s4[29:0], 2'b0} : s4;
	wire [31:0] s1 = (s2[31:31] == 1'b0) ? {s2[30:0], 1'b0} : s2;
	
	assign out = s1;
	assign amount = {(in[31:16] == 0), (s16[31:24] == 0), (s8[31:28] == 0), (s4[31:30] == 0), (s2[31] == 0)};

endmodule
module counter_32b (in, out);

	input [4:0] in;
	output [4:0] out;
	
	wire [4:0] carry;
	
	half_adder ha_0 (in[0], 1'b1, carry[0], out[0]);
	half_adder ha_1 (in[1], carry[0], carry[1], out[1]);
	half_adder ha_2 (in[2], carry[1], carry[2], out[2]);
	half_adder ha_3 (in[3], carry[2], carry[3], out[3]);
	half_adder ha_4 (in[4], carry[3], carry[4], out[4]);
endmodule

module half_adder (a, b, carry, sum);
	
	input a, b;
	output carry, sum;
	
	assign carry = a & b;
	assign sum = a ^ b;
	
endmodule

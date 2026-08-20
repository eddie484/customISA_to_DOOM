module rca_8b (a, b, c0, cout, sum);
	
	input [7:0]a, b;
	input c0;
	
	output cout;
	output [7:0]sum;
	
	wire [8:1] carry;
	
	full_adder adder0 (a[0], b[0], c0, carry[1], sum[0]);
	full_adder adder1 (a[1], b[1], carry[1], carry[2], sum[1]);
	full_adder adder2 (a[2], b[2], carry[2], carry[3], sum[2]);
	full_adder adder3 (a[3], b[3], carry[3], carry[4], sum[3]);
	full_adder adder4 (a[4], b[4], carry[4], carry[5], sum[4]);
	full_adder adder5 (a[5], b[5], carry[5], carry[6], sum[5]);
	full_adder adder6 (a[6], b[6], carry[6], carry[7], sum[6]);
	full_adder adder7 (a[7], b[7], carry[7], carry[8], sum[7]);
	
	assign cout = carry[8];
	
	
endmodule


module full_adder (a, b, cin, cout, sum);
	
	input a, b, cin;
	output cout, sum;
	
	assign cout = (a & b) | (cin & (a ^ b));
	assign sum = a ^ b ^ cin;
	
endmodule


module half_adder (a, b, carry, sum);
	
	input a, b;
	output carry, sum;
	
	assign carry = a & b;
	assign sum = a ^ b;
	
endmodule


module carry_look_ahead_adder_subtractor_4b (a, b, s, cout, sum);
	
	input [3:0]a, b;
	input s;
	
	output cout;
	output [3:0]sum;
	
	wire [3:1] c;
	wire [2:0] p, g; 
	
	wire [3:0] b_cal;
	assign b_cal = b ^ {4{s}};
	
	pg_cal pg0 (a[0], b_cal[0], p[0], g[0]);
	pg_cal pg1 (a[1], b_cal[1], p[1], g[1]);
	pg_cal pg2 (a[2], b_cal[2], p[2], g[2]);
	
	assign c[1] = g[0] | (s & p[0]);
	assign c[2] = g[1] | (g[0] & p[1]) | (s & p[0] & p[1]);	// g[1] | (`(g[0] | s & p[0])` &p[1])와 동일하다.
	assign c[3] = g[2] | (g[1] & p[2]) | (g[0] & p[1] & p[2]) | (s & p[0] & p[1] & p[2]);	// g[2] | `((g[1] | ((g[0] | s & p[0]) &p[1]))` & p[2])와 동일하다.
	
	middle_adder adder0 (a[0], b_cal[0], s, sum[0]);
	middle_adder adder1 (a[1], b_cal[1], c[1], sum[1]);
	middle_adder adder2 (a[2], b_cal[2], c[2], sum[2]);
	full_adder adder3 (a[3], b_cal[3], c[3], cout, sum[3]);
	
endmodule


module middle_adder (a, b, cin, sum);
	
	input a, b, cin;
	output sum;
	
	assign sum = a ^ b ^ cin;
	
endmodule


module full_adder (a, b, cin, cout, sum);
	
	input a, b, cin;
	output cout, sum;
	
	assign cout = (a & b) | (cin & (a ^ b));
	assign sum = a ^ b ^ cin;
	
endmodule


module pg_cal (a, b, p, g);	// half adder와 똑같은 형태!

	input a, b;
	output p, g;
	
	assign p = a ^ b;
	assign g = a & b;
	
endmodule
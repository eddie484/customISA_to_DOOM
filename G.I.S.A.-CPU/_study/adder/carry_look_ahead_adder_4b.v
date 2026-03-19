module carry_look_ahead_adder_4b (a, b, cin, cout, sum);
	
	input [3:0]a, b;
	input cin;
	
	output cout;
	output [3:0]sum;
	
	wire [3:1] c;
	wire [2:0] p, g; 
	
	pg_cal pg0 (a[0], b[0], p[0], g[0]);
	pg_cal pg1 (a[1], b[1], p[1], g[1]);
	pg_cal pg2 (a[2], b[2], p[2], g[2]);
	
	assign c[1] = g[0] | (cin & p[0]);
	assign c[2] = g[1] | (g[0] & p[1]) | (cin & p[0] & p[1]);	// g[1] | (`(g[0] | cin & p[0])` &p[1])와 동일하다.
	assign c[3] = g[2] | (g[1] & p[2]) | (g[0] & p[1] & p[2]) | (cin & p[0] & p[1] & p[2]);	// g[2] | `((g[1] | ((g[0] | cin & p[0]) &p [1]))` & p[2])와 동일하다.
	
	middle_adder adder0 (a[0], b[0], cin, sum[0]);
	middle_adder adder1 (a[1], b[1], c[1], sum[1]);
	middle_adder adder2 (a[2], b[2], c[2], sum[2]);
	full_adder adder3 (a[3], b[3], c[3], cout, sum[3]);
	
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
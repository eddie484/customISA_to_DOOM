module carry_look_ahead_adder_2level_16b (a, b, cin, cout, sum);
	
	input [15:0]a, b;
	input cin;
	
	output cout;
	output [15:0]sum;
	
	wire [3:1] c;
	wire [14:0] p, g; 
	wire [2:0] ps, gs;
	
	pg_cal pg0 (a[0], b[0], p[0], g[0]);
	pg_cal pg1 (a[1], b[1], p[1], g[1]);
	pg_cal pg2 (a[2], b[2], p[2], g[2]);
	pg_cal pg3 (a[3], b[3], p[3], g[3]);
	pg_cal pg4 (a[4], b[4], p[4], g[4]);
	pg_cal pg5 (a[5], b[5], p[5], g[5]);
	pg_cal pg6 (a[6], b[6], p[6], g[6]);
	pg_cal pg7 (a[7], b[7], p[7], g[7]);
	pg_cal pg8 (a[8], b[8], p[8], g[8]);
	pg_cal pg9 (a[9], b[9], p[9], g[9]);
	pg_cal pg10 (a[10], b[10], p[10], g[10]);
	pg_cal pg11 (a[11], b[11], p[11], g[11]);
	pg_cal pg12 (a[12], b[12], p[12], g[12]);
	pg_cal pg13 (a[13], b[13], p[13], g[13]);
	pg_cal pg14 (a[14], b[14], p[14], g[14]);
	
	assign ps[0] = p[0] & p[1] & p[2] & p[3];
	assign gs[0] = g[3] | (g[2] & p[3]) | (g[1] & p[2] & p[3]) | (g[0] & p[1] & p[2] & p[3]);
	assign ps[1] = p[4] & p[5] & p[6] & p[7];
	assign gs[1] = g[7] | (g[6] & p[7]) | (g[5] & p[6] & p[7]) | (g[4] & p[5] & p[6] & p[7]);
	assign ps[2] = p[8] & p[9] & p[10] & p[11];
	assign gs[2] = g[11] | (g[10] & p[11]) | (g[9] & p[10] & p[11]) | (g[8] & p[9] & p[10] & p[11]);
	
	assign c[1] = gs[0] | (cin & ps[0]);
	assign c[2] = gs[1] | (gs[0] & ps[1]) | (cin & ps[0] & ps[1]);
	assign c[3] = gs[2] | (gs[1] & ps[2]) | (gs[0] & ps[1] & ps[2]) | (cin & ps[0] & ps[1] & ps[2]);
	
	cla_group group0_cla (a[3:0], b[3:0], cin, p[2:0], g[2:0], , sum[3:0]);
	cla_group group1_cla (a[7:4], b[7:4], c[1], p[6:4], g[6:4], , sum[7:4]);
	cla_group group2_cla (a[11:8], b[11:8], c[2], p[10:8], g[10:8], , sum[11:8]);
	cla_group group3_cla (a[15:12], b[15:12], c[3], p[14:12], g[14:12], cout, sum[15:12]);
	
endmodule


module cla_group (a, b, cin, p, g, cout, sum);
	
	input [3:0]a, b;
	input cin;
	input [2:0]p, g;
	
	output cout;
	output [3:0]sum;
	
	wire [3:1] c;
	
	assign c[1] = g[0] | (cin & p[0]);
	assign c[2] = g[1] | (g[0] & p[1]) | (cin & p[0] & p[1]);	// g[1] | (`(g[0] | cin & p[0])` &p[1])와 동일하다.
	assign c[3] = g[2] | (g[1] & p[2]) | (g[0] & p[1] & p[2]) | (cin & p[0] & p[1] & p[2]);	// g[2] | `((g[1] | ((g[0] | cin & p[0]) &p [1]))` & p[2])와 동일하다.
	
	full_adder adder0 (a[0], b[0], cin, , sum[0]);
	full_adder adder1 (a[1], b[1], c[1], , sum[1]);
	full_adder adder2 (a[2], b[2], c[2], , sum[2]);
	full_adder adder3 (a[3], b[3], c[3], cout, sum[3]);
	
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
module carry_look_ahead_adder_subtractor_2level_16b (a, b, s, cout, sum);
	
	input [15:0]a, b;	// 입력으로 받은 두 16비트 피연산값 a, b
	input s;	// 덧셈, 뺄셈을 결정하는 입력. cin으로 쓰인다. 0일 경우 덧셈, 1일 경우 뺄셈.
	
	output cout;	// 최상위 비트에서 발생하는 carry out.
	output [15:0]sum;	// 출력으로 나오는 16비트 덧셈 결과값
	
	wire [3:1] c;	// 각 group cla 모듈의 cin으로 사용할 carry.
	wire [14:0] p, g; 	// propagated carry, generated carry. 
	wire [2:0] ps, gs;	// Group-propagated carry, Group-generated carry
	
	wire [15:0] b_cal;	// 연산에 사용될 b 값.
	assign b_cal = b ^ {16{s}};	// 뺄셈의 경우 입력으로 받은 b의 비트 반전값을 사용한다.

	
	// carry lookahead를 위해 p, g를 연산하는 모듈.
	pg_cal pg0 (a[0], b_cal[0], p[0], g[0]);
	pg_cal pg1 (a[1], b_cal[1], p[1], g[1]);
	pg_cal pg2 (a[2], b_cal[2], p[2], g[2]);
	pg_cal pg3 (a[3], b_cal[3], p[3], g[3]);
	pg_cal pg4 (a[4], b_cal[4], p[4], g[4]);
	pg_cal pg5 (a[5], b_cal[5], p[5], g[5]);
	pg_cal pg6 (a[6], b_cal[6], p[6], g[6]);
	pg_cal pg7 (a[7], b_cal[7], p[7], g[7]);
	pg_cal pg8 (a[8], b_cal[8], p[8], g[8]);
	pg_cal pg9 (a[9], b_cal[9], p[9], g[9]);
	pg_cal pg10 (a[10], b_cal[10], p[10], g[10]);
	pg_cal pg11 (a[11], b_cal[11], p[11], g[11]);
	pg_cal pg12 (a[12], b_cal[12], p[12], g[12]);
	pg_cal pg13 (a[13], b_cal[13], p[13], g[13]);
	pg_cal pg14 (a[14], b_cal[14], p[14], g[14]);
	
	// group p, g를 구한다.
	assign ps[0] = p[0] & p[1] & p[2] & p[3];
	assign gs[0] = g[3] | (g[2] & p[3]) | (g[1] & p[2] & p[3]) | (g[0] & p[1] & p[2] & p[3]);
	assign ps[1] = p[4] & p[5] & p[6] & p[7];
	assign gs[1] = g[7] | (g[6] & p[7]) | (g[5] & p[6] & p[7]) | (g[4] & p[5] & p[6] & p[7]);
	assign ps[2] = p[8] & p[9] & p[10] & p[11];
	assign gs[2] = g[11] | (g[10] & p[11]) | (g[9] & p[10] & p[11]) | (g[8] & p[9] & p[10] & p[11]);
	
	// group p, g를 이용해 각 group cla 모듈의 cin으로 사용할 값을 구한다.
	assign c[1] = gs[0] | (s & ps[0]);
	assign c[2] = gs[1] | (gs[0] & ps[1]) | (s & ps[0] & ps[1]);
	assign c[3] = gs[2] | (gs[1] & ps[2]) | (gs[0] & ps[1] & ps[2]) | (s & ps[0] & ps[1] & ps[2]);
	
	// group cla 모듈들을 이용해 연산을 진행한다.
	cla_group group0_cla (a[3:0], b_cal[3:0], s, p[2:0], g[2:0], , sum[3:0]);
	cla_group group1_cla (a[7:4], b_cal[7:4], c[1], p[6:4], g[6:4], , sum[7:4]);
	cla_group group2_cla (a[11:8], b_cal[11:8], c[2], p[10:8], g[10:8], , sum[11:8]);
	cla_group group3_cla (a[15:12], b_cal[15:12], c[3], p[14:12], g[14:12], cout, sum[15:12]);
	
endmodule


module cla_group (a, b, cin, p, g, cout, sum);	// 4비트 cla를 수행하는 모듈.
	
	input [3:0]a, b;
	input cin;
	input [2:0]p, g;
	
	output cout;
	output [3:0]sum;
	
	wire [3:1] c;
	
	// carry lookahead 연산
	assign c[1] = g[0] | (cin & p[0]);
	assign c[2] = g[1] | (g[0] & p[1]) | (cin & p[0] & p[1]);	// g[1] | (`(g[0] | cin & p[0])` &p[1])와 동일하다.
	assign c[3] = g[2] | (g[1] & p[2]) | (g[0] & p[1] & p[2]) | (cin & p[0] & p[1] & p[2]);	// g[2] | `((g[1] | ((g[0] | cin & p[0]) &p [1]))` & p[2])와 동일하다.
	
	// 예측된 carry값과 피연산값들을 full adder로 연산한다. 이미 carry lookahead로 계산된 cout는 받지 않는다.
	full_adder adder0 (a[0], b[0], cin, , sum[0]);
	full_adder adder1 (a[1], b[1], c[1], , sum[1]);
	full_adder adder2 (a[2], b[2], c[2], , sum[2]);
	full_adder adder3 (a[3], b[3], c[3], cout, sum[3]);
	
endmodule


module full_adder (a, b, cin, cout, sum);	// cin과 두 피연산값을 논리연산을 이용해 덧셈한다.
	
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
module carry_look_ahead_adder_3level_32b (a, b, cin, cout, sum);
	
	input [31:0]a, b;	// 입력으로 받은 두 32비트 피연산값 a, b
	input cin;	// 입력 carry
	
	output cout;	// 최상위 비트에서 발생하는 carry out.
	output [31:0]sum;	// 출력으로 나오는 32비트 덧셈 결과값
	
	wire c_2lv;	// 2레벨 group cla 모듈의 cin으로 사용할 carry
	wire [30:0] p, g; 	// propagated carry, generated carry. 
	wire [6:0] ps, gs;	// Group-propagated carry, Group-generated carry
	wire pss, gss;	// Group-group-propagated carry, Group-group-generated carry
	

	// carry lookahead를 위해 p, g를 연산하는 모듈.
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
	pg_cal pg15 (a[15], b[15], p[15], g[15]);
	pg_cal pg16 (a[16], b[16], p[16], g[16]);
	pg_cal pg17 (a[17], b[17], p[17], g[17]);
	pg_cal pg18 (a[18], b[18], p[18], g[18]);
	pg_cal pg19 (a[19], b[19], p[19], g[19]);
	pg_cal pg20 (a[20], b[20], p[20], g[20]);
	pg_cal pg21 (a[21], b[21], p[21], g[21]);
	pg_cal pg22 (a[22], b[22], p[22], g[22]);
	pg_cal pg23 (a[23], b[23], p[23], g[23]);
	pg_cal pg24 (a[24], b[24], p[24], g[24]);
	pg_cal pg25 (a[25], b[25], p[25], g[25]);
	pg_cal pg26 (a[26], b[26], p[26], g[26]);
	pg_cal pg27 (a[27], b[27], p[27], g[27]);
	pg_cal pg28 (a[28], b[28], p[28], g[28]);
	pg_cal pg29 (a[29], b[29], p[29], g[29]);
	pg_cal pg30 (a[30], b[30], p[30], g[30]);
	
	// group p, g를 구한다.
	assign ps[0] = p[0] & p[1] & p[2] & p[3];
	assign gs[0] = g[3] | (g[2] & p[3]) | (g[1] & p[2] & p[3]) | (g[0] & p[1] & p[2] & p[3]);
	assign ps[1] = p[4] & p[5] & p[6] & p[7];
	assign gs[1] = g[7] | (g[6] & p[7]) | (g[5] & p[6] & p[7]) | (g[4] & p[5] & p[6] & p[7]);
	assign ps[2] = p[8] & p[9] & p[10] & p[11];
	assign gs[2] = g[11] | (g[10] & p[11]) | (g[9] & p[10] & p[11]) | (g[8] & p[9] & p[10] & p[11]);
	assign ps[3] = p[12] & p[13] & p[14] & p[15];
	assign gs[3] = g[15] | (g[14] & p[15]) | (g[13] & p[14] & p[15]) | (g[12] & p[13] & p[14] & p[15]);
	assign ps[4] = p[16] & p[17] & p[18] & p[19];
	assign gs[4] = g[19] | (g[18] & p[19]) | (g[17] & p[18] & p[19]) | (g[16] & p[17] & p[18] & p[19]);
	assign ps[5] = p[20] & p[21] & p[22] & p[23];
	assign gs[5] = g[23] | (g[22] & p[23]) | (g[21] & p[22] & p[23]) | (g[20] & p[21] & p[22] & p[23]);
	assign ps[6] = p[24] & p[25] & p[26] & p[27];
	assign gs[6] = g[27] | (g[26] & p[27]) | (g[25] & p[26] & p[27]) | (g[24] & p[25] & p[26] & p[27]);
	
	// group-group p, g를 구한다.
	assign pss = ps[0] & ps[1] & ps[2] & ps[3];
	assign gss = gs[3] | (gs[2] & ps[3]) | (gs[1] & ps[2] & ps[3]) | (gs[0] & ps[1] & ps[2] & ps[3]);
	
	// group-group p, g를 이용해 2레벨 group cla 모듈의 cin으로 사용할 값을 구한다.
	assign c_2lv = gss | (cin & pss);
	
	// 2레벨 group cla 모듈들을 이용해 연산을 진행한다.
	cla_group_2level group0_cla (a[15:0], b[15:0], cin, p[14:0], g[14:0], ps[2:0], gs[2:0], , sum[15:0]);
	cla_group_2level group1_cla (a[31:16], b[31:16], c_2lv, p[30:16], g[30:16], ps[6:4], gs[6:4], cout, sum[31:16]);
	
endmodule


module cla_group_2level (a, b, cin, p, g, ps, gs, cout, sum);	// 2레벨 16비트 cla를 수행하는 모듈.
	
	input [15:0]a, b;	// 입력으로 받은 두 16비트 피연산값 a, b
	input cin;	// 입력 carry
	input [14:0] p, g; 	// propagated carry, generated carry. 
	input [2:0] ps, gs;	// Group-propagated carry, Group-generated carry
	
	output cout;	// 최상위 비트에서 발생하는 carry out.
	output [15:0]sum;	// 출력으로 나오는 16비트 덧셈 결과값
	
	wire [3:1] c;	// 각 group cla 모듈의 cin으로 사용할 carry.
	
	// group p, g를 이용해 각 group cla 모듈의 cin으로 사용할 값을 구한다.
	assign c[1] = gs[0] | (cin & ps[0]);
	assign c[2] = gs[1] | (gs[0] & ps[1]) | (cin & ps[0] & ps[1]);
	assign c[3] = gs[2] | (gs[1] & ps[2]) | (gs[0] & ps[1] & ps[2]) | (cin & ps[0] & ps[1] & ps[2]);
	
	// group cla 모듈들을 이용해 연산을 진행한다. 이미 carry lookahead로 계산된 cout는 받지 않는다.
	cla_group_1level group0_cla (a[3:0], b[3:0], cin, p[2:0], g[2:0], , sum[3:0]);
	cla_group_1level group1_cla (a[7:4], b[7:4], c[1], p[6:4], g[6:4], , sum[7:4]);
	cla_group_1level group2_cla (a[11:8], b[11:8], c[2], p[10:8], g[10:8], , sum[11:8]);
	cla_group_1level group3_cla (a[15:12], b[15:12], c[3], p[14:12], g[14:12], cout, sum[15:12]);
	
endmodule


module cla_group_1level (a, b, cin, p, g, cout, sum);	// 4비트 cla를 수행하는 모듈.
	
	input [3:0]a, b;
	input cin;
	input [2:0]p, g;
	
	output cout;
	output [3:0]sum;
	
	wire [3:1] c;
	
	// carry lookahead 연산
	assign c[1] = g[0] | (cin & p[0]);
	assign c[2] = g[1] | (g[0] & p[1]) | (cin & p[0] & p[1]);	// g[1] | (`(g[0] | cin & p[0])` &p[1])와 동일하다.
	assign c[3] = g[2] | (g[1] & p[2]) | (g[0] & p[1] & p[2]) | (cin & p[0] & p[1] & p[2]);	// g[2] | `((g[1] | ((g[0] | cin & p[0]) &p[1]))` & p[2])와 동일하다.
	
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
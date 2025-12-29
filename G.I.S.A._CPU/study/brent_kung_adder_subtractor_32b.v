module brent_kung_adder_subtractor_32b (a, b, s, cout, sum);
	
	input [31:0]a, b;	// 입력으로 받은 두 32비트 피연산값 a, b
	input s;	// 덧셈, 뺄셈을 결정하는 입력. cin으로 쓰인다. 0일 경우 덧셈, 1일 경우 뺄셈.
	
	output cout;	// 최상위 비트에서 발생하는 carry out.
	output [31:0]sum;	// 출력으로 나오는 32비트 덧셈 결과값
	
	wire [31:0] p_0, g_0;   // input cell의 연산 결과. 두 피연산값을 이용해 만든 p, g이다.
	wire [31:0] p_1, g_1;   // 1단계 black cell의 연산 결과. [i:i-1] 범위 연결.
	wire [31:0] p_2, g_2;   // 2단계 black cell의 연산 결과. [i:i-2] 범위 연결.
	wire [31:0] p_3, g_3;   // 3단계 black cell의 연산 결과. [i:i-4] 범위 연결.
	wire [31:0] p_4, g_4;   // 4단계 black cell의 연산 결과. [i:i-8] 범위 연결.
	wire [31:0] p_5, g_5;   // 5단계 black cell의 연산 결과. [i:i-16] 의 연결.
	wire [31:0] p_6, g_6;   // 6단계 black cell의 연산 결과. [i:i-8] 의 연결.
	wire [31:0] p_7, g_7;   // 7단계 black cell의 연산 결과. [i:i-4] 의 연결.
	wire [31:0] p_8, g_8;   // 8단계 black cell의 연산 결과. [i:i-2] 의 연결.
	wire [31:0] p_9, g_9;   // 9단계 black cell의 연산 결과. [i:i-1] 의 연결.
    wire [31:1] c;  // carry cell의 연산 결과. 각 자리의 prefix carry 값.

    wire [31:0] b_cal;	// 연산에 사용될 b 값.
	assign b_cal = b ^ {32{s}};	// 뺄셈의 경우 입력으로 받은 b의 비트 반전값을 사용한다.

    genvar i;

    // input cell 모듈들
    generate
        for (i = 0; i < 32; i = i + 1) begin: input_level
            input_cell input_level (a[i], b_cal[i], p_0[i], g_0[i]);
        end
    endgenerate


    // black cell level 1 모듈들
    generate
        for (i = 0; i < 32; i = i + 2) begin: level_1
            black_cell level_1 (p_0[i], g_0[i], p_0[i+1], g_0[i+1], p_1[i+1], g_1[i+1]);
        end
    endgenerate


    // black cell level 2 모듈들
    generate
        for (i = 1; i < 32; i = i + 4) begin: level_2
            black_cell level_2 (p_1[i], g_1[i], p_1[i+2], g_1[i+2], p_2[i+2], g_2[i+2]);
        end
    endgenerate


    // black cell level 3 모듈들
    generate
        for (i = 3; i < 32; i = i + 8) begin: level_3
            black_cell level_3 (p_2[i], g_2[i], p_2[i+4], g_2[i+4], p_3[i+4], g_3[i+4]);
        end
    endgenerate


    // black cell level 4 모듈들
    black_cell level_4_1 (p_3[7], g_3[7], p_3[15], g_3[15], p_4[15], g_4[15]);
    black_cell level_4_2 (p_3[23], g_3[23], p_3[31], g_3[31], p_4[31], g_4[31]);


    // black cell level 5 모듈들
    black_cell level_5_1 (p_4[15], g_4[15], p_4[31], g_4[31], p_5[31], g_5[31]);


    // black cell level 6 모듈들
    black_cell level_6_1 (p_4[15], g_4[15], p_3[23], g_3[23], p_6[23], g_6[23]);


    // black cell level 7 모듈들
    black_cell level_7_1 (p_3[7], g_3[7], p_2[11], g_2[11], p_7[11], g_7[11]);
    black_cell level_7_2 (p_4[15], g_4[15], p_2[19], g_2[19], p_7[19], g_7[19]);
    black_cell level_7_3 (p_6[23], g_6[23], p_2[27], g_2[27], p_7[27], g_7[27]);


    // black cell level 8 모듈들
    black_cell level_8_1 (p_2[3], g_2[3], p_1[5], g_1[5], p_8[5], g_8[5]);
    black_cell level_8_2 (p_3[7], g_3[7], p_1[9], g_1[9], p_8[9], g_8[9]);
    black_cell level_8_3 (p_7[11], g_7[11], p_1[13], g_1[13], p_8[13], g_8[13]);
    black_cell level_8_4 (p_4[15], g_4[15], p_1[17], g_1[17], p_8[17], g_8[17]);
    black_cell level_8_5 (p_7[19], g_7[19], p_1[21], g_1[21], p_8[21], g_8[21]);
    black_cell level_8_6 (p_6[23], g_6[23], p_1[25], g_1[25], p_8[25], g_8[25]);
    black_cell level_8_7 (p_7[27], g_7[27], p_1[29], g_1[29], p_8[29], g_8[29]);


    // black cell level 9 모듈들
    black_cell level_9_1 (p_1[1], g_1[1], p_0[2], g_0[2], p_9[2], g_9[2]);
    black_cell level_9_2 (p_2[3], g_2[3], p_0[4], g_0[4], p_9[4], g_9[4]);
    black_cell level_9_3 (p_8[5], g_8[5], p_0[6], g_0[6], p_9[6], g_9[6]);
    black_cell level_9_4 (p_3[7], g_3[7], p_0[8], g_0[8], p_9[8], g_9[8]);
    black_cell level_9_5 (p_8[9], g_8[9], p_0[10], g_0[10], p_9[10], g_9[10]);
    black_cell level_9_6 (p_7[11], g_7[11], p_0[12], g_0[12], p_9[12], g_9[12]);
    black_cell level_9_7 (p_8[13], g_8[13], p_0[14], g_0[14], p_9[14], g_9[14]);
    black_cell level_9_8 (p_4[15], g_4[15], p_0[16], g_0[16], p_9[16], g_9[16]);
    black_cell level_9_9 (p_8[17], g_8[17], p_0[18], g_0[18], p_9[18], g_9[18]);
    black_cell level_9_10 (p_7[19], g_7[19], p_0[20], g_0[20], p_9[20], g_9[20]);
    black_cell level_9_11 (p_8[21], g_8[21], p_0[22], g_0[22], p_9[22], g_9[22]);
    black_cell level_9_12 (p_6[23], g_6[23], p_0[24], g_0[24], p_9[24], g_9[24]);
    black_cell level_9_13 (p_8[25], g_8[25], p_0[26], g_0[26], p_9[26], g_9[26]);
    black_cell level_9_14 (p_7[27], g_7[27], p_0[28], g_0[28], p_9[28], g_9[28]);
    black_cell level_9_15 (p_8[29], g_8[29], p_0[30], g_0[30], p_9[30], g_9[30]);


    // carry cell 모듈들
    carry_cell carry_0 (p_0[0], g_0[0], s, c[1]);
    carry_cell carry_1 (p_1[1], g_1[1], s, c[2]);
    carry_cell carry_2 (p_2[3], g_2[3], s, c[4]);
    carry_cell carry_3 (p_8[5], g_8[5], s, c[6]);
    carry_cell carry_4 (p_3[7], g_3[7], s, c[8]);
    carry_cell carry_5 (p_8[9], g_8[9], s, c[10]);
    carry_cell carry_6 (p_7[11], g_7[11], s, c[12]);
    carry_cell carry_7 (p_8[13], g_8[13], s, c[14]);
    carry_cell carry_8 (p_4[15], g_4[15], s, c[16]);
    carry_cell carry_9 (p_8[17], g_8[17], s, c[18]);
    carry_cell carry_10 (p_7[19], g_7[19], s, c[20]);
    carry_cell carry_11 (p_8[21], g_8[21], s, c[22]);
    carry_cell carry_12 (p_6[23], g_6[23], s, c[24]);
    carry_cell carry_13 (p_8[25], g_8[25], s, c[26]);
    carry_cell carry_14 (p_7[27], g_7[27], s, c[28]);
    carry_cell carry_15 (p_8[29], g_8[29], s, c[30]);
    carry_cell carry_16 (p_5[31], g_5[31], s, cout);
    generate
        for (i = 2; i < 32; i = i + 2) begin: carry_level
            carry_cell carry (p_9[i], g_9[i], s, c[i+1]);
        end
    endgenerate


    // sum cell 모듈들
    sum_cell sum_0 (p_0[0], s, sum[0]);
    generate
        for (i = 1; i < 32; i = i + 1) begin: sum_level
            sum_cell sum_level (p_0[i], c[i], sum[i]);
        end
    endgenerate


endmodule


module input_cell (a, b, p, g);

    input a, b;
    output p, g;

    assign p = a ^ b;
    assign g = a & b;

endmodule

module black_cell (pl, gl, ph, gh, pout, gout); // p/g low, p/g high.

    input ph, gh;
    input pl, gl;
    output pout, gout;

    assign pout = ph & pl;
    assign gout = gh | (ph & gl);

endmodule

module carry_cell (p, g, cz, cout);

    input p, g, cz;
    output cout;

    assign cout = g | (p & cz);

endmodule

module sum_cell (p, c, sum);

    input p, c;
    output sum;

    assign sum = p ^ c;

endmodule
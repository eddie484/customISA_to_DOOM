module han_carlson_adder_subtractor_64b (a, b, s, cout, sum);
	
	input [63:0]a, b;	// 입력으로 받은 두 64비트 피연산값 a, b
	input s;	// 덧셈, 뺄셈을 결정하는 입력. cin으로 쓰인다. 0일 경우 덧셈, 1일 경우 뺄셈.
	
	output cout;	// 최상위 비트에서 발생하는 carry out.
	output [63:0]sum;	// 출력으로 나오는 64비트 덧셈 결과값
	
	wire [63:0] p_0, g_0;   // input cell의 연산 결과. 두 피연산값을 이용해 만든 p, g이다.
	wire [63:0] p_1, g_1;   // 1단계 black cell의 연산 결과. [i:i-1] 범위 연결.
	wire [63:0] p_2, g_2;   // 2단계 black cell의 연산 결과. [i:i-2] 범위 연결.
	wire [63:0] p_3, g_3;   // 3단계 black cell의 연산 결과. [i:i-4] 범위 연결.
	wire [63:0] p_4, g_4;   // 4단계 black cell의 연산 결과. [i:i-8] 범위 연결.
	wire [63:0] p_5, g_5;   // 5단계 black cell의 연산 결과. [i:i-16] 의 연결.
	wire [63:0] p_6, g_6;   // 6단계 black cell의 연산 결과. [i:i-32] 의 연결.
	wire [63:0] p_7, g_7;   // 7단계 black cell의 연산 결과. [i:i-1] 의 연결.
    wire [63:1] c;  // carry cell의 연산 결과. 각 자리의 prefix carry 값.

    wire [63:0] b_cal;	// 연산에 사용될 b 값.
	assign b_cal = b ^ {64{s}};	// 뺄셈의 경우 입력으로 받은 b의 비트 반전값을 사용한다.

    genvar i;

    // input cell 모듈들
    generate
        for (i = 0; i < 64; i = i + 1) begin: input_level
            input_cell input_level (a[i], b_cal[i], p_0[i], g_0[i]);
        end
    endgenerate


    // black cell level 1 모듈들
    generate
        for (i = 0; i < 64; i = i + 2) begin: level_1
            black_cell level_1 (p_0[i], g_0[i], p_0[i+1], g_0[i+1], p_1[i+1], g_1[i+1]);
        end
    endgenerate


    // black cell level 2 모듈들
    generate
        for (i = 1; i < 62; i = i + 2) begin: level_2
            black_cell level_2 (p_1[i], g_1[i], p_1[i+2], g_1[i+2], p_2[i+2], g_2[i+2]);
        end
    endgenerate


    // black cell level 3 모듈들
    black_cell level_3_1 (p_1[1], g_1[1], p_2[5], g_2[5], p_3[5], g_3[5]);
    generate
        for (i = 3; i < 60; i = i + 2) begin: level_3
            black_cell level_3 (p_2[i], g_2[i], p_2[i+4], g_2[i+4], p_3[i+4], g_3[i+4]);
        end
    endgenerate


    // black cell level 4 모듈들
    black_cell level_4_1 (p_1[1], g_1[1], p_3[9], g_3[9], p_4[9], g_4[9]);
    black_cell level_4_2 (p_2[3], g_2[3], p_3[11], g_3[11], p_4[11], g_4[11]);
    generate
        for (i = 5; i < 56; i = i + 2) begin: level_4
            black_cell level_4 (p_3[i], g_3[i], p_3[i+8], g_3[i+8], p_4[i+8], g_4[i+8]);
        end
    endgenerate


    // black cell level 5 모듈들
    black_cell level_5_1 (p_1[1], g_1[1], p_4[17], g_4[17], p_5[17], g_5[17]);
    black_cell level_5_2 (p_2[3], g_2[3], p_4[19], g_4[19], p_5[19], g_5[19]);
    black_cell level_5_3 (p_3[5], g_3[5], p_4[21], g_4[21], p_5[21], g_5[21]);
    black_cell level_5_4 (p_3[7], g_3[7], p_4[23], g_4[23], p_5[23], g_5[23]);
    generate
        for (i = 9; i < 48; i = i + 2) begin: level_5
            black_cell level_5 (p_4[i], g_4[i], p_4[i+16], g_4[i+16], p_5[i+16], g_5[i+16]);
        end
    endgenerate


    // black cell level 6 모듈들
    black_cell level_6_1 (p_1[1], g_1[1], p_5[33], g_5[33], p_6[33], g_6[33]);
    black_cell level_6_2 (p_2[3], g_2[3], p_5[35], g_5[35], p_6[35], g_6[35]);
    black_cell level_6_3 (p_3[5], g_3[5], p_5[37], g_5[37], p_6[37], g_6[37]);
    black_cell level_6_4 (p_3[7], g_3[7], p_5[39], g_5[39], p_6[39], g_6[39]);
    black_cell level_6_5 (p_4[9], g_4[9], p_5[41], g_5[41], p_6[41], g_6[41]);
    black_cell level_6_6 (p_4[11], g_4[11], p_5[43], g_5[43], p_6[43], g_6[43]);
    black_cell level_6_7 (p_4[13], g_4[13], p_5[45], g_5[45], p_6[45], g_6[45]);
    black_cell level_6_8 (p_4[15], g_4[15], p_5[47], g_5[47], p_6[47], g_6[47]);
    generate
        for (i = 17; i < 32; i = i + 2) begin: level_6
            black_cell level_6 (p_5[i], g_5[i], p_5[i+32], g_5[i+32], p_6[i+32], g_6[i+32]);
        end
    endgenerate


    // black cell level 7 모듈들
    black_cell level_7_1 (p_1[1], g_1[1], p_0[2], g_0[2], p_7[2], g_7[2]);
    black_cell level_7_2 (p_2[3], g_2[3], p_0[4], g_0[4], p_7[4], g_7[4]);
    black_cell level_7_3 (p_3[5], g_3[5], p_0[6], g_0[6], p_7[6], g_7[6]);
    black_cell level_7_4 (p_3[7], g_3[7], p_0[8], g_0[8], p_7[8], g_7[8]);
    black_cell level_7_5 (p_4[9], g_4[9], p_0[10], g_0[10], p_7[10], g_7[10]);
    black_cell level_7_6 (p_4[11], g_4[11], p_0[12], g_0[12], p_7[12], g_7[12]);
    black_cell level_7_7 (p_4[13], g_4[13], p_0[14], g_0[14], p_7[14], g_7[14]);
    black_cell level_7_8 (p_4[15], g_4[15], p_0[16], g_0[16], p_7[16], g_7[16]);
    black_cell level_7_9 (p_5[17], g_5[17], p_0[18], g_0[18], p_7[18], g_7[18]);
    black_cell level_7_10 (p_5[19], g_5[19], p_0[20], g_0[20], p_7[20], g_7[20]);
    black_cell level_7_11 (p_5[21], g_5[21], p_0[22], g_0[22], p_7[22], g_7[22]);
    black_cell level_7_12 (p_5[23], g_5[23], p_0[24], g_0[24], p_7[24], g_7[24]);
    black_cell level_7_13 (p_5[25], g_5[25], p_0[26], g_0[26], p_7[26], g_7[26]);
    black_cell level_7_14 (p_5[27], g_5[27], p_0[28], g_0[28], p_7[28], g_7[28]);
    black_cell level_7_15 (p_5[29], g_5[29], p_0[30], g_0[30], p_7[30], g_7[30]);
    black_cell level_7_16 (p_5[31], g_5[31], p_0[32], g_0[32], p_7[32], g_7[32]);
    generate
        for (i = 33; i < 62; i = i + 2) begin: level_7
            black_cell level_7 (p_6[i], g_6[i], p_0[i+1], g_0[i+1], p_7[i+1], g_7[i+1]);
        end
    endgenerate


    // carry cell 모듈들
    carry_cell carry_0 (p_0[0], g_0[0], s, c[1]);
    carry_cell carry_1 (p_1[1], g_1[1], s, c[2]);
    carry_cell carry_2 (p_2[3], g_2[3], s, c[4]);
    carry_cell carry_3 (p_3[5], g_3[5], s, c[6]);
    carry_cell carry_4 (p_3[7], g_3[7], s, c[8]);
    carry_cell carry_5 (p_4[9], g_4[9], s, c[10]);
    carry_cell carry_6 (p_4[11], g_4[11], s, c[12]);
    carry_cell carry_7 (p_4[13], g_4[13], s, c[14]);
    carry_cell carry_8 (p_4[15], g_4[15], s, c[16]);
    carry_cell carry_9 (p_5[17], g_5[17], s, c[18]);
    carry_cell carry_10 (p_5[19], g_5[19], s, c[20]);
    carry_cell carry_11 (p_5[21], g_5[21], s, c[22]);
    carry_cell carry_12 (p_5[23], g_5[23], s, c[24]);
    carry_cell carry_13 (p_5[25], g_5[25], s, c[26]);
    carry_cell carry_14 (p_5[27], g_5[27], s, c[28]);
    carry_cell carry_15 (p_5[29], g_5[29], s, c[30]);
    carry_cell carry_16 (p_5[31], g_5[31], s, c[32]);
    carry_cell carry_32 (p_6[63], g_6[63], s, cout);
    generate
        for (i = 33; i < 62; i = i + 2) begin: carry_level_1
            carry_cell carry_level_1 (p_6[i], g_6[i], s, c[i+1]);
        end
        for (i = 2; i < 64; i = i + 2) begin: carry_level_2
            carry_cell carry_level_2 (p_7[i], g_7[i], s, c[i+1]);
        end
    endgenerate


    // sum cell 모듈들
    sum_cell sum_0 (p_0[0], s, sum[0]);
    generate
        for (i = 1; i < 64; i = i + 1) begin: sum_level
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
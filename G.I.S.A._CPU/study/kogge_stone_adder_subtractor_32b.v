module kogge_stone_adder_subtractor_32b (a, b, s, cout, sum);
	
	input [31:0]a, b;	// 입력으로 받은 두 32비트 피연산값 a, b
	input s;	// 덧셈, 뺄셈을 결정하는 입력. cin으로 쓰인다. 0일 경우 덧셈, 1일 경우 뺄셈.
	
	output cout;	// 최상위 비트에서 발생하는 carry out.
	output [31:0]sum;	// 출력으로 나오는 32비트 덧셈 결과값
	
	wire [31:0] p_0, g_0;   // input cell의 연산 결과. 두 피연산값을 이용해 만든 p, g이다.
	wire [31:1] p_1, g_1;   // 1단계 black cell의 연산 결과. [i:i-1] 범위 연결.
	wire [31:2] p_2, g_2;   // 2단계 black cell의 연산 결과. [i:i-2] 범위 연결.
	wire [31:4] p_3, g_3;   // 3단계 black cell의 연산 결과. [i:i-4] 범위 연결.
	wire [31:8] p_4, g_4;   // 4단계 black cell의 연산 결과. [i:i-8] 범위 연결.
	wire [31:16] p_5, g_5;   // 5단계 black cell의 연산 결과. [i:i-16] 의 연결.
    wire [31:1] c;  // carry cell의 연산 결과. 각 자리의 prefix carry 값.

    wire [31:0] b_cal;	// 연산에 사용될 b 값.
	assign b_cal = b ^ {32{s}};	// 뺄셈의 경우 입력으로 받은 b의 비트 반전값을 사용한다.

    genvar i;

    // input cell 모듈들
    generate
        for (i = 0; i < 32; i = i + 1) begin: input_cell
            input_cell input_level (a[i], b_cal[i], p_0[i], g_0[i]);
        end
    endgenerate


    // black cell level 1 모듈들
    generate
        for (i = 1; i < 32; i = i + 1) begin: level_1
            black_cell level_1 (p_0[i-1], g_0[i-1], p_0[i], g_0[i], p_1[i], g_1[i]);
        end
    endgenerate


    // black cell level 2 모듈들
    black_cell level_2_2 (p_0[0], g_0[0], p_1[2], g_1[2], p_2[2], g_2[2]);
    generate
        for (i = 3; i < 32; i = i + 1) begin: level_2
            black_cell level_2 (p_1[i-2], g_1[i-2], p_1[i], g_1[i], p_2[i], g_2[i]);
        end
    endgenerate


    // black cell level 3 모듈들
    black_cell level_3_4 (p_0[0], g_0[0], p_2[4], g_2[4], p_3[4], g_3[4]);
    black_cell level_3_5 (p_1[1], g_1[1], p_2[5], g_2[5], p_3[5], g_3[5]);
    generate
        for (i = 6; i < 32; i = i + 1) begin: level_3
            black_cell level_3 (p_2[i-4], g_2[i-4], p_2[i], g_2[i], p_3[i], g_3[i]);
        end
    endgenerate


    // black cell level 4 모듈들
    black_cell level_4_8 (p_0[0], g_0[0], p_3[8], g_3[8], p_4[8], g_4[8]);
    black_cell level_4_9 (p_1[1], g_1[1], p_3[9], g_3[9], p_4[9], g_4[9]);
    black_cell level_4_10 (p_2[2], g_2[2], p_3[10], g_3[10], p_4[10], g_4[10]);
    black_cell level_4_11 (p_2[3], g_2[3], p_3[11], g_3[11], p_4[11], g_4[11]);
    generate
        for (i = 12; i < 32; i = i + 1) begin: level_4
            black_cell level_4 (p_3[i-8], g_3[i-8], p_3[i], g_3[i], p_4[i], g_4[i]);
        end
    endgenerate


    // black cell level 5 모듈들
    black_cell level_5_16 (p_0[0], g_0[0], p_4[16], g_4[16], p_5[16], g_5[16]);
    black_cell level_5_17 (p_1[1], g_1[1], p_4[17], g_4[17], p_5[17], g_5[17]);
    black_cell level_5_18 (p_2[2], g_2[2], p_4[18], g_4[18], p_5[18], g_5[18]);
    black_cell level_5_19 (p_2[3], g_2[3], p_4[19], g_4[19], p_5[19], g_5[19]);
    black_cell level_5_20 (p_3[4], g_3[4], p_4[20], g_4[20], p_5[20], g_5[20]);
    black_cell level_5_21 (p_3[5], g_3[5], p_4[21], g_4[21], p_5[21], g_5[21]);
    black_cell level_5_22 (p_3[6], g_3[6], p_4[22], g_4[22], p_5[22], g_5[22]);
    black_cell level_5_23 (p_3[7], g_3[7], p_4[23], g_4[23], p_5[23], g_5[23]);
    generate
        for (i = 24; i < 32; i = i + 1) begin: level_5
            black_cell level_5 (p_4[i-16], g_4[i-16], p_4[i], g_4[i], p_5[i], g_5[i]);
        end
    endgenerate


    // carry cell 모듈들
    carry_cell carry_0 (p_0[0], g_0[0], s, c[1]);
    carry_cell carry_1 (p_1[1], g_1[1], s, c[2]);
    carry_cell carry_2 (p_2[2], g_2[2], s, c[3]);
    carry_cell carry_3 (p_2[3], g_2[3], s, c[4]);
    carry_cell carry_4 (p_3[4], g_3[4], s, c[5]);
    carry_cell carry_5 (p_3[5], g_3[5], s, c[6]);
    carry_cell carry_6 (p_3[6], g_3[6], s, c[7]);
    carry_cell carry_7 (p_3[7], g_3[7], s, c[8]);
    generate
        for (i = 8; i < 16; i = i + 1) begin: carry_p4
            carry_cell carry_p4 (p_4[i], g_4[i], s, c[i+1]);
        end
        for (i = 16; i < 31; i = i + 1) begin: carry_p5
            carry_cell carry_p5 (p_5[i], g_5[i], s, c[i+1]);
        end
    endgenerate
    carry_cell carry_31 (p_5[31], g_5[31], c[31], cout);


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
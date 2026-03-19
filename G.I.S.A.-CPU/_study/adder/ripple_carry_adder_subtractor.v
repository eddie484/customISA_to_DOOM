module ripple_carry_adder_subtractor (a, b, s, cout, sum);
	
	input [31:0]a, b;
	input s;
	
	output cout;
	output [31:0]sum;
	
	wire [32:1] carry;
	wire [31:0] b_cal;
	
	assign b_cal = b ^ {32{s}};
	
	full_adder adder0 (a[0], b_cal[0], s, carry[1], sum[0]);
	full_adder adder1 (a[1], b_cal[1], carry[1], carry[2], sum[1]);
	full_adder adder2 (a[2], b_cal[2], carry[2], carry[3], sum[2]);
	full_adder adder3 (a[3], b_cal[3], carry[3], carry[4], sum[3]);
	full_adder adder4 (a[4], b_cal[4], carry[4], carry[5], sum[4]);
	full_adder adder5 (a[5], b_cal[5], carry[5], carry[6], sum[5]);
	full_adder adder6 (a[6], b_cal[6], carry[6], carry[7], sum[6]);
	full_adder adder7 (a[7], b_cal[7], carry[7], carry[8], sum[7]);
	full_adder adder8 (a[8], b_cal[8], carry[8], carry[9], sum[8]);
	full_adder adder9 (a[9], b_cal[9], carry[9], carry[10], sum[9]);
	full_adder adder10 (a[10], b_cal[10], carry[10], carry[11], sum[10]);
	full_adder adder11 (a[11], b_cal[11], carry[11], carry[12], sum[11]);
	full_adder adder12 (a[12], b_cal[12], carry[12], carry[13], sum[12]);
	full_adder adder13 (a[13], b_cal[13], carry[13], carry[14], sum[13]);
	full_adder adder14 (a[14], b_cal[14], carry[14], carry[15], sum[14]);
	full_adder adder15 (a[15], b_cal[15], carry[15], carry[16], sum[15]);
	full_adder adder16 (a[16], b_cal[16], carry[16], carry[17], sum[16]);
	full_adder adder17 (a[17], b_cal[17], carry[17], carry[18], sum[17]);
	full_adder adder18 (a[18], b_cal[18], carry[18], carry[19], sum[18]);
	full_adder adder19 (a[19], b_cal[19], carry[19], carry[20], sum[19]);
	full_adder adder20 (a[20], b_cal[20], carry[20], carry[21], sum[20]);
	full_adder adder21 (a[21], b_cal[21], carry[21], carry[22], sum[21]);
	full_adder adder22 (a[22], b_cal[22], carry[22], carry[23], sum[22]);
	full_adder adder23 (a[23], b_cal[23], carry[23], carry[24], sum[23]);
	full_adder adder24 (a[24], b_cal[24], carry[24], carry[25], sum[24]);
	full_adder adder25 (a[25], b_cal[25], carry[25], carry[26], sum[25]);
	full_adder adder26 (a[26], b_cal[26], carry[26], carry[27], sum[26]);
	full_adder adder27 (a[27], b_cal[27], carry[27], carry[28], sum[27]);
	full_adder adder28 (a[28], b_cal[28], carry[28], carry[29], sum[28]);
	full_adder adder29 (a[29], b_cal[29], carry[29], carry[30], sum[29]);
	full_adder adder30 (a[30], b_cal[30], carry[30], carry[31], sum[30]);
	full_adder adder31 (a[31], b_cal[31], carry[31], carry[32], sum[31]);
	
	assign cout = carry[32];
	
	
endmodule
`define RADIX_M2 3'd1
`define RADIX_M1 3'd2
`define RADIX_0 3'd3
`define RADIX_P1 3'd4
`define RADIX_P2 3'd5

module radix4_signed_srt_divider(clk, nRESET, dividend_in, divisor_in, signed_sig_in, div_on_in, quotient_out, remainder_out, fin_out, busy_out);

	input clk, nRESET;
	input [31:0] dividend_in, divisor_in;
	input signed_sig_in, div_on_in;
	
	output reg [31:0] quotient_out;
	output [31:0] remainder_out;
	output reg fin_out, busy_out;
	
	wire [63:0] dividend_shifted;
	wire [34:0] using_rc, using_rs, using_rc_csa, result_rc, result_rs, multiplied_d, cpa_result;
	wire [31:0] q_neg_sign, q_minus, q_sum, q_result, cpa_result_restored, remainder_minus, remainder_aftershift;
	wire [31:0] divisor_shifted, dividend_minus, divisor_minus, dividend_absolute, divisor_absolute;
	wire [7:0] apr_result;
	wire [4:0] divisor_shift_amount_wire, next_m;
	wire [2:0] cpa_carry;
	wire [2:0] pla_result;
	wire is_dividend_minus, is_divisor_minus, is_quotient_minus;
	reg [34:0] r_carry, r_sum;
	reg [31:0] remainder_shifted;
	reg [31:0] q_pos, q_neg, d, pr_low;
	reg [4:0] divisor_shift_amount_reg;
	reg [4:0] m;
	reg [1:0] div_state;

	// 다음단 연산에 사용하기 위해 shift한 carry/sum
	assign using_rc = (m == 5'b0) ? r_carry : {r_carry[32:0], 2'b0};
	assign using_rs = (m == 5'b0) ? r_sum : {r_sum[32:0], pr_low[31:30]};
	assign using_rc_csa = (pla_result == `RADIX_P1 || pla_result == `RADIX_P2) ? {using_rc[34:1], 1'b1} : using_rc;
	
	// p에서 뺄 q*d 값
	assign multiplied_d = (pla_result == `RADIX_0) ? 35'b0 : ((pla_result == `RADIX_M2) ? {2'b0, d[31:0], 1'b0} : ((pla_result == `RADIX_P2) ? ~{2'b0, d[31:0], 1'b0} : ((pla_result == `RADIX_M1) ? {3'b0, d} : ~{3'b0, d})));
	
	// remainder를 구하는 cpa
	csa csa_module (multiplied_d, using_rs, using_rc_csa, result_rs, result_rc);
	
	// 연산결과로 나온 carry와 sum을 더해 최종 remainder를 구한다.
	adder_subtractor cpa_module (r_carry, r_sum, 1'b0, cpa_carry[0], cpa_result[31:0]);
	full_adder fa_32 (r_carry[32], r_sum[32], cpa_carry[0], cpa_carry[1], cpa_result[32]);
	full_adder fa_33 (r_carry[33], r_sum[33], cpa_carry[1], cpa_carry[2], cpa_result[33]);
	full_adder fa_34 (r_carry[34], r_sum[34], cpa_carry[2], , cpa_result[34]);
	
	// PLA에서 quotient bit 정하기
	rca_8b apr_adder (using_rc[34:27], using_rs[34:27], 1'b0, , apr_result);
	pla pla_module (apr_result[7:1], d[30:27], pla_result);
	
	// q값 구하기
	adder_subtractor q_cal (q_pos, q_neg_sign, !cpa_result[34], ,q_sum);
	
	// cpa_result가 음수일 경우 사용할 복구값 계산
	adder_subtractor cpa_result_restoring (cpa_result[31:0], d, 1'b0, , cpa_result_restored);
	assign q_neg_sign = (cpa_result[34] == 1'b1) ? ~q_neg : q_neg;

	
	// 입력 절댓값 처리
	assign is_dividend_minus = signed_sig_in && dividend_in[31];
	assign is_divisor_minus = signed_sig_in && divisor_in[31];
	adder_subtractor dividend_minus_maker (32'b0, dividend_in, 1'b1, , dividend_minus);
	adder_subtractor divisor_minus_maker (32'b0, divisor_in, 1'b1, , divisor_minus);
	assign dividend_absolute = is_dividend_minus ? dividend_minus : dividend_in;
	assign divisor_absolute = is_divisor_minus ? divisor_minus : divisor_in;
	
	// 입력 전처리 shift
	msb_shifter msb_divisor (divisor_absolute, divisor_shifted, divisor_shift_amount_wire);
	left_shifter_64b dividend_shifter ({32'b0, dividend_absolute}, {1'b0, divisor_shift_amount_wire}, dividend_shifted);	// 64b shifter 만들어 바꾸기

	// remainder의 후처리 shift
	assign is_quotient_minus = is_dividend_minus ^ is_divisor_minus;
	right_shifter_32b remainder_right_shifter (remainder_shifted, divisor_shift_amount_reg, 1'b0, remainder_aftershift);
	adder_subtractor remainder_minus_maker (32'b0, remainder_aftershift, 1'b1, , remainder_minus);
	adder_subtractor q_minus_maker (32'b0, q_sum, 1'b1, , q_minus);
	assign remainder_out = is_dividend_minus ? remainder_minus : remainder_aftershift;
	assign q_result = is_quotient_minus ? q_minus : q_sum;
	
	counter_32b counter (m, next_m);
	
	
	localparam IDLE = 2'b00;
	localparam CAL = 2'b01;
	localparam END = 2'b10;
	
	
	
	
	always @(posedge clk) begin
		if (!nRESET) begin
			fin_out <= 1'b0;
			m <= 5'b0;
			div_state <= IDLE;
			busy_out <= 1'b0;

		end else begin
			case (div_state)
				IDLE: begin
					fin_out <= 1'b0;
					busy_out <= 1'b0;
					if (div_on_in == 1) begin
						if (divisor_in == 32'b0) begin
							fin_out <= 1'b1;
							quotient_out <= 32'b0;
							remainder_shifted <= 32'b0;
						end else begin
							div_state <= CAL;
							busy_out <= 1'b1;
							r_carry <= 35'b0;
							r_sum <= {3'b0, dividend_shifted[63:32]};
							pr_low <= dividend_shifted[31:0];
							d <= divisor_shifted;
							divisor_shift_amount_reg <= divisor_shift_amount_wire;
							q_pos <= 32'b0;
							q_neg <= 32'b0;
							m <= 5'b0;
						end
					end
				end

				CAL: begin
					m <= next_m;
					r_carry <= result_rc;
					r_sum <= result_rs;
					
					// pla 값에 따른 q 세팅
					case (pla_result)
						`RADIX_M2: begin
							q_pos <= {q_pos[29:0], 2'b00};
							q_neg <= {q_neg[29:0], 2'b10};
						end
					
						`RADIX_M1: begin
							q_pos <= {q_pos[29:0], 2'b00};
							q_neg <= {q_neg[29:0], 2'b01};
						end
					
						`RADIX_0: begin
							q_pos <= {q_pos[29:0], 2'b00};
							q_neg <= {q_neg[29:0], 2'b00};
						end
					
						`RADIX_P1: begin
							q_pos <= {q_pos[29:0], 2'b01};
							q_neg <= {q_neg[29:0], 2'b00};
						end
					
						`RADIX_P2: begin
							q_pos <= {q_pos[29:0], 2'b10};
							q_neg <= {q_neg[29:0], 2'b00};
						end
					
					endcase
					
					if (m != 5'b0) begin
						pr_low <= {pr_low[29:0], 2'b0};
					end
					
					
					if (m == 5'b1_0000) begin
						div_state <= END;
					end
				end
				
				END: begin
					fin_out <= 1'b1;
					if (cpa_result[34] == 1) begin
						remainder_shifted <= cpa_result_restored[31:0];
					end else begin
						remainder_shifted <= cpa_result[31:0];
					end
					quotient_out <= q_result;
					div_state <= IDLE;
				end
				
				default: div_state <= IDLE;
			endcase
		end
	end
endmodule
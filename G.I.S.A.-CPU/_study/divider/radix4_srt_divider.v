`define RADIX_M2 3'd1
`define RADIX_M1 3'd2
`define RADIX_0 3'd3
`define RADIX_P1 3'd4
`define RADIX_P2 3'd5

module radix4_srt_divider(clk, nRESET, dividend_in, divisor_in, div_on_in, quotient_out, remainder_out, fin_out);

	input clk, nRESET;
	input [31:0] dividend_in, divisor_in;
	input div_on_in;
	
	(* keep *) output reg [31:0] quotient_out;
	(* keep *) output [31:0] remainder_out;
	(* keep *) output reg fin_out;
	
	wire [63:0] dividend_shifted;
	wire [31:0] q_neg_sign, q_result;
	wire [34:0] using_rc, using_rs, using_rc_csa, result_rc, result_rs, multiplied_d, cpa_result, cpa_result_restored;
	wire [31:0] divisor_shifted;
	wire [7:0] apr_result;
	wire [4:0] divisor_shift_amount_wire;
	wire [2:0] cpa_carry;
	wire [2:0] pla_result;
	reg [31:0] q_pos, q_neg, d, pr_low;
	reg [34:0] r_carry, r_sum;
	reg [4:0] divisor_shift_amount_reg;
	(* keep *) reg [4:0] m;
	(* keep *) reg [1:0] div_state;
	(* keep *) reg busy;
	(* keep *) reg [31:0] remainder_shifted;

	assign using_rc = (m == 5'b0) ? r_carry : {r_carry[32:0], 2'b0};
	assign using_rs = (m == 5'b0) ? r_sum : {r_sum[32:0], pr_low[31:30]};
	assign using_rc_csa = (pla_result == `RADIX_P1 || pla_result == `RADIX_P2) ? {using_rc[34:1], 1'b1} : using_rc;
	assign multiplied_d = (pla_result == `RADIX_0) ? 35'b0 : ((pla_result == `RADIX_M2) ? {2'b0, d[31:0], 1'b0} : ((pla_result == `RADIX_P2) ? ~{2'b0, d[31:0], 1'b0} : ((pla_result == `RADIX_M1) ? {3'b0, d} : ~{3'b0, d})));
	assign q_neg_sign = (cpa_result[34] == 1'b1) ? ~q_neg : q_neg;
	assign dividend_shifted = dividend_in << divisor_shift_amount_wire;	// 64b shifter 만들어 바꾸기
	
	csa csa_module (multiplied_d, using_rs, using_rc_csa, result_rs, result_rc);
	adder_subtractor cpa_module (r_carry, r_sum, 1'b0, cpa_carry[0], cpa_result[31:0]);
	full_adder fa_32 (r_carry[32], r_sum[32], cpa_carry[0], cpa_carry[1], cpa_result[32]);
	full_adder fa_33 (r_carry[33], r_sum[33], cpa_carry[1], cpa_carry[2], cpa_result[33]);
	full_adder fa_34 (r_carry[34], r_sum[34], cpa_carry[2], , cpa_result[34]);
	rca_8b apr_adder (using_rc[34:27], using_rs[34:27], 1'b0, , apr_result);
	adder_subtractor q_cal (q_pos, q_neg_sign, !cpa_result[34], ,q_result);
	pla pla_module (apr_result[7:1], d[30:27], pla_result);
	adder_subtractor cpa_result_restoring (cpa_result, d, 1'b0, , cpa_result_restored);
	
	msb_shifter msb_divisor (divisor_in, divisor_shifted, divisor_shift_amount_wire);
	
	//shifter_rotator msb_dividend (dividend_in, divisor_shift_amount_wire, 2'b01, 0, dividend_shifted,);
	shifter_rotator remainder_right_shifter (remainder_shifted, divisor_shift_amount_reg, 2'b10, 0, remainder_out,);
	
	
	
	localparam IDLE = 2'b00;
	localparam CAL = 2'b01;
	localparam END = 2'b10;
	
	
	
	
	always @(posedge clk) begin
		if (!nRESET) begin
			fin_out <= 1'b0;
			m <= 5'b0;
			div_state <= IDLE;
			busy <= 1'b0;

		end else begin
			case (div_state)
				IDLE: begin
					fin_out <= 1'b0;
					busy <= 1'b0;
					if (div_on_in == 1) begin
						if (divisor_in == 32'b0) begin
							fin_out <= 1'b1;
							quotient_out <= 32'b0;
							remainder_shifted <= 32'b0;
						end else begin
							div_state <= CAL;
							busy <= 1'b1;
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
					m <= m + 5'b1;
					r_carry <= result_rc;
					r_sum <= result_rs;
					
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
						remainder_shifted <= cpa_result_restored;
					end else begin
						remainder_shifted <= cpa_result;
					end
					quotient_out <= q_result;
					div_state <= IDLE;
				end
				
				default: div_state <= IDLE;
			endcase
		end
	end
endmodule
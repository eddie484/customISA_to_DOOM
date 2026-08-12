module non_restoring_divider(clk, nRESET, dividend_in, divisor_in, div_on_in, quotient_out, remainder_out, fin_out);

	input clk, nRESET;
	input [31:0] dividend_in, divisor_in;
	input div_on_in;
	
	output [31:0] quotient_out;
	output reg [31:0] remainder_out;
	output reg fin_out;
	
	wire [64:0] next_p;
	wire [4:0] next_m;
	wire [31:0] next_p_high_cal, restored_remainder, restored_q, unsigned_q;
	wire next_p_subtractor_cout;
	
	reg [64:0] p;
	reg [31:0] d, q, signed_q;
	reg [4:0] m;
	reg [1:0] div_state;
	reg busy;
	
	
	localparam IDLE = 2'b00;
	localparam CAL = 2'b01;
	localparam END = 2'b10;
	
	
	adder_subtractor next_p_subtractor (p[62:31], d, !p[64], next_p_subtractor_cout, next_p_high_cal);
	adder_subtractor remainder_restoring (p[63:32], d, 1'b0, , restored_remainder);
	adder_subtractor quotient_restoring (unsigned_q, 32'b1, 1'b1, , restored_q);
	counter_32b counter (m, next_m);
	adder_subtractor cpa_q (signed_q, ~signed_q, 1'b1, , unsigned_q);
	
	assign quotient_out = (p[64] ? restored_q : unsigned_q);

	
	assign next_p = {p[63] ^ (next_p_subtractor_cout ^ !p[64]), next_p_high_cal, p[30:0], 1'b0};
	
	always @(posedge clk) begin
		if (!nRESET) begin
			signed_q <= 32'b0;
			remainder_out <= 32'b0;
			fin_out <= 1'b0;
			p <= 65'b0;
			d <= 32'b0;
			q <= 32'b0;
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
							signed_q <= 32'b0;
							remainder_out <= 32'b0;
						end else begin
							div_state <= CAL;
							busy <= 1'b1;
							p <= {33'b0, dividend_in};
							d <= divisor_in;
							q <= 32'b0;
							m <= 5'b0;
						end
					end
				end
				
				CAL: begin
					m <= next_m;
					q <= {q[30:0], !p[64]};
					
					
					p <= next_p;
					
					
					if (m == 5'b1_1111) begin
						div_state <= END;
					end
				end
				
				END: begin
					fin_out <= 1'b1;
					signed_q <= q;
					if (p[64] == 1) begin
						remainder_out <= restored_remainder;
					end else begin
						remainder_out <= p[63:32];
					end
					div_state <= IDLE;
				end
				
				default: div_state <= IDLE;
			endcase
		end
	end
endmodule
module restoring_divider(clk, nRESET, dividend_in, divisor_in, div_on_in, quotient_out, remainder_out, fin_out);

	input clk, nRESET;
	input [31:0] dividend_in, divisor_in;
	input div_on_in;
	
	output reg [31:0] quotient_out, remainder_out;
	output reg fin_out;
	
	wire [64:0] next_p;
	wire [5:0] next_m;
	
	reg [64:0] p;
	reg [31:0] d, q;
	reg [5:0] m;
	reg [2:0] div_state;
	reg busy;
	
	
	localparam IDLE = 3'b000;
	localparam START = 3'b001;
	localparam CAL = 3'b010;
	localparam RESTORE = 3'b011;
	localparam END = 3'b100;
	
	
	assign next_p = {p[63:31] - d, p[30:0], 1'b0};
	assign next_m = m + 1;
	
	always @(posedge clk) begin
		if (!nRESET) begin
			div_state <= IDLE;
			busy <= 1'b0;

		end else begin
			case (div_state)
				IDLE: begin
					fin_out <= 1'b0;
					busy <= 1'b0;
					if (div_on_in == 1) begin
						div_state <= START;
					end
				end
				
				START: begin
					busy <= 1'b1;
					p <= {33'b0, dividend_in};
					d <= divisor_in;
					q <= 32'b0;
					m <= 5'b0;
					div_state <= CAL;
				end
				
				CAL: begin
					p <= next_p;
					m <= next_m;
					q <= {q[30:0], 1'b1};
					
					if (next_p[64] == 1) begin
						div_state <= RESTORE;
					end else if (next_m == 6'b10_0000) begin
						div_state <= END;
					end
				end
				
				RESTORE: begin
					p <= {p[64:32] + d, p[31:0]};
					q[0] <= 1'b0;
					
					if (m == 6'b10_0000) begin
						div_state <= END;
					end else begin
						div_state <= CAL;
					end
				end
				
				END: begin
					fin_out <= 1'b1;
					quotient_out <= q;
					remainder_out <= p[63:32];
					div_state <= IDLE;
				end
			endcase
		end
	end
endmodule
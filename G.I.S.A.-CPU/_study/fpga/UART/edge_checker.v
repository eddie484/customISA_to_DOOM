module edge_checker (clk, nRESET, signal, pos_sig, neg_sig);

	input clk, nRESET, signal;
	output reg pos_sig, neg_sig;
	reg [2:0] state;
	
	localparam OFF = 3'b000;
	localparam OFF_BOUNCE_CHECK = 3'b001;
	localparam POSEDGE = 3'b010;
	localparam ON = 3'b011;
	localparam ON_BOUNCE_CHECK = 3'b100;
	localparam NEGEDGE = 3'b101;
	
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) begin
			state <= OFF;
			pos_sig <= 1'b0;
			neg_sig <= 1'b0;
		end else begin
			case (state)
				OFF: begin
					pos_sig <= 1'b0;
					neg_sig <= 1'b0;
					if (signal == 1'b1) state <= OFF_BOUNCE_CHECK;
				end
				
				OFF_BOUNCE_CHECK: begin
					pos_sig <= 1'b0;
					neg_sig <= 1'b0;
					if (signal == 1'b1) state <= POSEDGE;
					else state <= OFF;
				end
				
				POSEDGE: begin
					pos_sig <= 1'b1;
					neg_sig <= 1'b0;
					state <= ON;
				end
				
				ON: begin
					pos_sig <= 1'b0;
					neg_sig <= 1'b0;
					if (signal == 1'b0) state <= ON_BOUNCE_CHECK;
				end
				
				ON_BOUNCE_CHECK: begin
					pos_sig <= 1'b0;
					neg_sig <= 1'b0;
					if (signal == 1'b0) state <= NEGEDGE;
					else state <= ON;
				end
				
				NEGEDGE: begin
					pos_sig <= 1'b0;
					neg_sig <= 1'b1;
					state <= OFF;
				end
				
				default: begin
					pos_sig <= 1'b0;
					neg_sig <= 1'b0;
					state <= OFF;
				end
			endcase
		end
	end
endmodule
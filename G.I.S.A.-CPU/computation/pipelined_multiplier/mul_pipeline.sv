module cal_checker (clk, nRESET, mul_start_in, mul_finished_out);

	input clk, nRESET;
	input mul_start_in;
	
	output reg mul_finished_out;
	
	reg [1:0] mul_state;
	
	
	localparam IDLE = 2'b00;
	localparam START = 2'b01;
	localparam FINISH = 2'b11;
	
	
	always @(posedge clk) begin
		if (!nRESET) begin
			mul_state <= IDLE;
			mul_finished_out <= 1'b0;
		end else begin
			case (mul_state)
				IDLE: begin
					if (mul_start_in) mul_state <= START;
					mul_finished_out <= 1'b0;
				end
				
				START: begin
					mul_state <= FINISH;
					mul_finished_out <= 1'b1;
				end
				
				FINISH: begin
					mul_state <= IDLE;
					mul_finished_out <= 1'b0;
				end
			endcase
		end
	end
	
	
endmodule



module pipereg_PGtoPA (clk, nRESET, pp_PG, sign_PG, signed_sig_in, a_in, b_in, pp_PA, sign_PA, signed_sig_PA, a_PA, b_PA);
	
	input clk, nRESET;
	input [33:0] pp_PG [15:0];
	input [31:0] a_in;
	input [15:0] sign_PG;
	input signed_sig_in, b_in;
	
	output reg [33:0] pp_PA [15:0];
	output reg [31:0] a_PA;
	output reg [15:0] sign_PA;
	output reg signed_sig_PA, b_PA;
	
	always @(posedge clk) begin
		if (!nRESET) begin
			pp_PA <= '{default: '0};
			a_PA <= 32'b0;
			sign_PA <= 16'b0;
			signed_sig_PA <= 1'b0;
			b_PA <= 1'b0;
		end else begin
			pp_PA <= pp_PG;
			a_PA <= a_in;
			sign_PA <= sign_PG;
			signed_sig_PA <= signed_sig_in;
			b_PA <= b_in;
		end
	end
endmodule



module pipereg_PAtoFA (clk, nRESET, tree_result_PA, tree_result_FA);

	input clk, nRESET;
	input [63:0] tree_result_PA [1:0];
	
	output reg [63:0] tree_result_FA [1:0];
	
	always @(posedge clk) begin
		if (!nRESET)
			tree_result_FA <= '{default: '0};
		else
			tree_result_FA <= tree_result_PA;
	end
endmodule
module edge_checker_onboard (clk, nRESET, BUTTON, LED);

	input clk, nRESET;
	input BUTTON;
	
	output [2:0] LED;
	
	wire button_sig, button_pos, button_neg;
	
	reg [1:0] pos_counter;
	
	
	edge_checker button_edge (clk, nRESET, button_sig, button_pos, button_neg);
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) pos_counter = 2'b0;
		else if (button_pos) pos_counter = pos_counter + 1;
	end
	
	assign button_sig = ~BUTTON;
	assign LED = ~{button_sig, pos_counter};

endmodule
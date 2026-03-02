module simple_counter(clk, counter_out);

	input clk;
	output reg [31:0] counter_out;
	
	always @(posedge clk) begin
		counter_out <= #1 counter_out + 1;
	end
	
endmodule
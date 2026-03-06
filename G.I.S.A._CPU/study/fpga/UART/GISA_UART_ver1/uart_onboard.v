module uart_onboard (clk, nRESET, rdx, tdx, LED);
	
	input clk, nRESET;
	input rdx;
	
	output reg [7:0] LED;
	output tdx;
	
	wire [7:0] uart_receive;
	wire uart_outen;
	
	uart uart_core (clk, nRESET, rdx, uart_receive, uart_outen);
	assign tdx = rdx;
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) LED <= 8'b0;
		else if (uart_outen) LED <= ~uart_receive;
	end
	
endmodule
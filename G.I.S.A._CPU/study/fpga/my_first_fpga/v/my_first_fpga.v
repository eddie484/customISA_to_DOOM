module my_first_fpga(CLOCK0_50, KEY, LED);

	input CLOCK0_50;
	input [1:0] KEY;
	
	output [7:0] LED;
	
	wire clk_50;
	wire [31:0] counter;
	wire [7:0] mux_result;
	
	pll u_pll(CLOCK0_50, ~KEY[1], clk_50);
	
	simple_counter u_simple_counter(clk_50, counter);
	
	counter_bus_mux u_counter_bus_mux(counter[24:17], counter[26:19], KEY[0], mux_result);
	
	assign LED = ~mux_result;

endmodule
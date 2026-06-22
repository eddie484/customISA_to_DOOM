(* noprune *) module GISA_CPU_onboard (CLOCK0_50, switch, button, led);
	
	input CLOCK0_50;
	input [3:0] switch;
	input [1:0] button;
	
	output [7:0] led;
	
	wire [31:0] reg_0, reg_1, reg_2, reg_3, reg_4, reg_5, reg_6, reg_7, reg_8, reg_9, reg_10, reg_11, reg_12, reg_13, reg_14, reg_15;

	wire [1:0] button_not;
	assign button_not = ~button[1:0];
	
	wire clk, nRESET, locked;
	assign pll_RESET = !((switch == 4'b1111) && (button_not == 2'b11));
	assign nRESET = (!((switch == 4'b1111) && (button_not == 2'b11))) && locked;
	
	pll pll_module(.refclk(CLOCK0_50), .rst(!pll_RESET), .locked(locked), .outclk_0(clk));
	
	GISA_CPU GISA_CPU_module (clk, nRESET, reg_0, reg_1, reg_2, reg_3, reg_4, reg_5, reg_6, reg_7, reg_8, reg_9, reg_10, reg_11, reg_12, reg_13, reg_14, reg_15);
	
	// ----- OnBoard Logic -----
	
	wire [31:0] selected_reg;
	wire [7:0] selected_bit;
	
	assign selected_reg = (switch == 4'b0000) ? reg_0 :
								 (switch == 4'b0001) ? reg_1 :
								 (switch == 4'b0010) ? reg_2 :
								 (switch == 4'b0011) ? reg_3 :
								 (switch == 4'b0100) ? reg_4 :
								 (switch == 4'b0101) ? reg_5 :
								 (switch == 4'b0110) ? reg_6 :
								 (switch == 4'b0111) ? reg_7 :
								 (switch == 4'b1000) ? reg_8 :
								 (switch == 4'b1001) ? reg_9 :
								 (switch == 4'b1010) ? reg_10 :
								 (switch == 4'b1011) ? reg_11 :
								 (switch == 4'b1100) ? reg_12 :
								 (switch == 4'b1101) ? reg_13 :
								 (switch == 4'b1110) ? reg_14 : reg_15;
								 

	assign selected_bit = (selected_reg >> (button_not * 8));
	assign led = ~selected_bit;
endmodule	
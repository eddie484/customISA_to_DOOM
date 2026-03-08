module adder_subtractor_onboard (CLK, SWITCH, BUTTON_A, BUTTON_B, LED);

	input CLK;
	input [3:0] SWITCH;
	input BUTTON_A;
	input BUTTON_B;
	
	output [7:0] LED;
	
	wire [31:0] result;
	wire cout;
	wire sig_A, sig_B;
	
	reg sign = 1'b0;
	reg [31:0] valA = 32'b0;
	reg [31:0] valB = 32'b0; 
	
	
	assign sig_A = ~BUTTON_A;
	assign sig_B = ~BUTTON_B;
	
	
	always @ (posedge CLK) begin
		if (sig_A && sig_B && (SWITCH == 4'b0101)) begin
			sign <= 1'b0;
		end else if (sig_A && sig_B && (SWITCH == 4'b1010)) begin
			sign <= 1'b1;
		end else if (sig_A) begin
			valA <= {28'b0, SWITCH};
		end else if (sig_B) begin
			valB <= {28'b0, SWITCH};
		end
	end
	
	
	adder add_sub_module (valA, valB, sign, cout, result);

	
	assign LED = ~{sign, sig_A, sig_B, cout, result[3:0]};
	
endmodule
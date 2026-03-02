module adder_subtractor_onboard (CLK, SWITCH, BUTTON_A, BUTTON_B, LED);

	input CLK;
	input [3:0] SWITCH;
	input BUTTON_A;
	input BUTTON_B;
	
	output [7:0] LED;
	
	reg [31:0] valA = 32'b0;
	reg [31:0] valB = 32'b0; 
	wire [31:0] result;
	wire cout;
	reg sign = 1'b0;
	
	
	
	
	
	always @ (posedge CLK) begin
		if (~BUTTON_A && ~BUTTON_B && (SWITCH == 4'b0101)) begin
			sign <= 1'b0;
		end else if (~BUTTON_A && ~BUTTON_B && (SWITCH == 4'b1010)) begin
			sign <= 1'b1;
		end else if (~BUTTON_A) begin
			valA <= {28'b0, SWITCH};
		end else if (~BUTTON_B) begin
			valB <= {28'b0, SWITCH};
		end
	end
	
	
	adder add_sub_module (valA, valB, sign, cout, result);

	
	assign LED = ~{sign, ~BUTTON_A, ~BUTTON_B, cout, result[3:0]};
	
endmodule
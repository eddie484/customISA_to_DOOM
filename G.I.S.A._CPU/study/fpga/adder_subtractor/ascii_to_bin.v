module ascii_bin_translator (ascii_in, direction_in, binary_out);

	input [7:0] ascii_in;
	input direction_in;	// 0: ascii to bin, 1: bin to ascii.
	
	output reg [3:0] binary_out;
	
	always @(*) begin
		case (ascii_in)
			8'b00110000: binary_out = 4'b0000;
			8'b00110001: binary_out = 4'b0001;
			8'b00110010: binary_out = 4'b0010;
			8'b00110011: binary_out = 4'b0011;
			8'b00110100: binary_out = 4'b0100;
			8'b00110101: binary_out = 4'b0101;
			8'b00110110: binary_out = 4'b0110;
			8'b00110111: binary_out = 4'b0111;
			8'b00111000: binary_out = 4'b1000;
			8'b00111001: binary_out = 4'b1001;
			8'b01100001: binary_out = 4'b1010;
			8'b01100010: binary_out = 4'b1011;
			8'b01100011: binary_out = 4'b1100;
			8'b01100100: binary_out = 4'b1101;
			8'b01100101: binary_out = 4'b1110;
			8'b01100110: binary_out = 4'b1111;
			8'b01000001: binary_out = 4'b1010;
			8'b01000010: binary_out = 4'b1011;
			8'b01000011: binary_out = 4'b1100;
			8'b01000100: binary_out = 4'b1101;
			8'b01000101: binary_out = 4'b1110;
			8'b01000110: binary_out = 4'b1111;
			default: binary_out = 4'b1111;
		endcase
	end
	
endmodule
	
module ascii_bin_translator (input_sig, direction_in, output_sig);

	input [7:0] input_sig;
	input direction_in;	// 0: ascii to bin, 1: bin to ascii.
	
	output reg [7:0] output_sig;
	
	always @(*) begin
		if (direction_in == 1'b0) begin
			case (input_sig)
				8'b00110000: output_sig = 8'b00000000;
				8'b00110001: output_sig = 8'b00000001;
				8'b00110010: output_sig = 8'b00000010;
				8'b00110011: output_sig = 8'b00000011;
				8'b00110100: output_sig = 8'b00000100;
				8'b00110101: output_sig = 8'b00000101;
				8'b00110110: output_sig = 8'b00000110;
				8'b00110111: output_sig = 8'b00000111;
				8'b00111000: output_sig = 8'b00001000;
				8'b00111001: output_sig = 8'b00001001;
				8'b01100001: output_sig = 8'b00001010;
				8'b01100010: output_sig = 8'b00001011;
				8'b01100011: output_sig = 8'b00001100;
				8'b01100100: output_sig = 8'b00001101;
				8'b01100101: output_sig = 8'b00001110;
				8'b01100110: output_sig = 8'b00001111;
				8'b01000001: output_sig = 8'b00001010;
				8'b01000010: output_sig = 8'b00001011;
				8'b01000011: output_sig = 8'b00001100;
				8'b01000100: output_sig = 8'b00001101;
				8'b01000101: output_sig = 8'b00001110;
				8'b01000110: output_sig = 8'b00001111;
				default: output_sig = input_sig;
			endcase
		end else begin
			case (input_sig)
				8'b00000000: output_sig = 8'b00110000;
				8'b00000001: output_sig = 8'b00110001;
				8'b00000010: output_sig = 8'b00110010;
				8'b00000011: output_sig = 8'b00110011;
				8'b00000100: output_sig = 8'b00110100;
				8'b00000101: output_sig = 8'b00110101;
				8'b00000110: output_sig = 8'b00110110;
				8'b00000111: output_sig = 8'b00110111;
				8'b00001000: output_sig = 8'b00111000;
				8'b00001001: output_sig = 8'b00111001;
				8'b00000001: output_sig = 8'b01101010;
				8'b00000010: output_sig = 8'b01101011;
				8'b00000011: output_sig = 8'b01101100;
				8'b00000100: output_sig = 8'b01101101;
				8'b00000101: output_sig = 8'b01101110;
				8'b00000110: output_sig = 8'b01101111;
				default: output_sig = input_sig;
			endcase
		end
	end
	
endmodule
	
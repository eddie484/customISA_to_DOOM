module ascii_bin_translator (input_sig, direction_in, output_sig);

	input [7:0] input_sig;
	input direction_in;	// 0: ascii to bin, 1: bin to ascii.
	
	output reg [7:0] output_sig;
	
	always @(*) begin
		if (direction_in == 1'b0) begin
			case (input_sig)
				8'b00110000: output_sig = 8'b00000000;
				8'b00110001: output_sig = 8'b00000001;
				default: output_sig = input_sig;
			endcase
		end else begin
			case (input_sig)
				8'b00000000: output_sig = 8'b00110000;
				8'b00000001: output_sig = 8'b00110001;
				default: output_sig = input_sig;
			endcase
		end
	end
	
endmodule
	
`define RADIX_M2 3'd1
`define RADIX_M1 3'd2
`define RADIX_0 3'd3
`define RADIX_P1 3'd4
`define RADIX_P2 3'd5

module pla (p, d, out);

	input [6:0] p;
	input [3:0] d;
	output reg [2:0] out;
	
	
	always @(*) begin
	case (d)
		4'b0000: begin
			if      (p <= 7'b000_0011) out = `RADIX_0;
			else if (p <= 7'b000_1011) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b111_0010) out = `RADIX_M2;
			else if (p <= 7'b111_1010) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b0001: begin
			if      (p <= 7'b000_0011) out = `RADIX_0;
			else if (p <= 7'b000_1100) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b111_0001) out = `RADIX_M2;
			else if (p <= 7'b111_1010) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b0010: begin
			if      (p <= 7'b000_0100) out = `RADIX_0;
			else if (p <= 7'b000_1101) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b111_0000) out = `RADIX_M2;
			else if (p <= 7'b111_1001) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b0011: begin
			if      (p <= 7'b000_0100) out = `RADIX_0;
			else if (p <= 7'b000_1101) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b111_0000) out = `RADIX_M2;
			else if (p <= 7'b111_1001) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b0100: begin
			if      (p <= 7'b000_0100) out = `RADIX_0;
			else if (p <= 7'b000_1110) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1111) out = `RADIX_M2;
			else if (p <= 7'b111_1001) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b0101: begin
			if      (p <= 7'b000_0101) out = `RADIX_0;
			else if (p <= 7'b000_1111) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1110) out = `RADIX_M2;
			else if (p <= 7'b111_1000) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b0110: begin
			if      (p <= 7'b000_0101) out = `RADIX_0;
			else if (p <= 7'b001_0000) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1101) out = `RADIX_M2;
			else if (p <= 7'b111_1000) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b0111: begin
			if      (p <= 7'b000_0101) out = `RADIX_0;
			else if (p <= 7'b001_0001) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1100) out = `RADIX_M2;
			else if (p <= 7'b111_1000) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b1000: begin
			if      (p <= 7'b000_0110) out = `RADIX_0;
			else if (p <= 7'b001_0010) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1011) out = `RADIX_M2;
			else if (p <= 7'b111_0111) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b1001: begin
			if      (p <= 7'b000_0110) out = `RADIX_0;
			else if (p <= 7'b001_0010) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1011) out = `RADIX_M2;
			else if (p <= 7'b111_0111) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b1010: begin
			if      (p <= 7'b000_0110) out = `RADIX_0;
			else if (p <= 7'b001_0011) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1010) out = `RADIX_M2;
			else if (p <= 7'b111_0111) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b1011: begin
			if      (p <= 7'b000_0111) out = `RADIX_0;
			else if (p <= 7'b001_0100) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1001) out = `RADIX_M2;
			else if (p <= 7'b111_0110) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b1100: begin
			if      (p <= 7'b000_0111) out = `RADIX_0;
			else if (p <= 7'b001_0101) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_1000) out = `RADIX_M2;
			else if (p <= 7'b111_0110) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b1101: begin
			if      (p <= 7'b000_0111) out = `RADIX_0;
			else if (p <= 7'b001_0110) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_0111) out = `RADIX_M2;
			else if (p <= 7'b111_0110) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b1110: begin
			if      (p <= 7'b000_0111) out = `RADIX_0;
			else if (p <= 7'b001_0111) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_0110) out = `RADIX_M2;
			else if (p <= 7'b111_0101) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		4'b1111: begin
			if      (p <= 7'b000_0111) out = `RADIX_0;
			else if (p <= 7'b001_0111) out = `RADIX_P1;
			else if (p <= 7'b010_1001) out = `RADIX_P2;
			else if (p <= 7'b110_0110) out = `RADIX_M2;
			else if (p <= 7'b111_0101) out = `RADIX_M1;
			else                       out = `RADIX_0;
		end

		default: out = `RADIX_0;
	endcase
end
	
endmodule
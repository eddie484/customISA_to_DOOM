module reg_file (clk, nRESET, srcA_in, srcB_in, wben_in, dst_in, valD_in, valA_out, valB_out);

	input clk, nRESET;
	input [3:0] srcA_in, srcB_in, dst_in;
	input wben_in;
	input [31:0] valD_in;
	
	output [31:0] valA_out, valB_out;
	
	reg [31:0] reg_0, reg_1, reg_2, reg_3, reg_4, reg_5, reg_6, reg_7, reg_8, reg_9, reg_10, reg_11, reg_12, reg_13, reg_14, reg_15;
	
	wire [15:0] enable_reg;
	
	
	assign enable_reg = (dst_in == 4'b0000) ? 16'b0000_0000_0000_0001 :
							(dst_in == 4'b0001) ? 16'b0000_0000_0000_0010 :
							(dst_in == 4'b0010) ? 16'b0000_0000_0000_0100 :
							(dst_in == 4'b0011) ? 16'b0000_0000_0000_1000 :
							(dst_in == 4'b0100) ? 16'b0000_0000_0001_0000 :
							(dst_in == 4'b0101) ? 16'b0000_0000_0010_0000 :
							(dst_in == 4'b0110) ? 16'b0000_0000_0100_0000 :
							(dst_in == 4'b0111) ? 16'b0000_0000_1000_0000 :
							(dst_in == 4'b1000) ? 16'b0000_0001_0000_0000 :
							(dst_in == 4'b1001) ? 16'b0000_0010_0000_0000 :
							(dst_in == 4'b1010) ? 16'b0000_0100_0000_0000 :
							(dst_in == 4'b1011) ? 16'b0000_1000_0000_0000 :
							(dst_in == 4'b1100) ? 16'b0001_0000_0000_0000 :
							(dst_in == 4'b1101) ? 16'b0010_0000_0000_0000 :
							(dst_in == 4'b1110) ? 16'b0100_0000_0000_0000 :
							(dst_in == 4'b1111) ? 16'b1000_0000_0000_0000 : 16'b0000_0000_0000_0000;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_0 <= 32'b0;
		else if (enable_reg[0] && wben_in)
			reg_0 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_1 <= 32'b0;
		else if (enable_reg[1] && wben_in)
			reg_1 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_2 <= 32'b0;
		else if (enable_reg[2] && wben_in)
			reg_2 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_3 <= 32'b0;
		else if (enable_reg[3] && wben_in)
			reg_3 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_4 <= 32'b0;
		else if (enable_reg[4] && wben_in)
			reg_4 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_5 <= 32'b0;
		else if (enable_reg[5] && wben_in)
			reg_5 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_6 <= 32'b0;
		else if (enable_reg[6] && wben_in)
			reg_6 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_7 <= 32'b0;
		else if (enable_reg[7] && wben_in)
			reg_7 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_8 <= 32'b0;
		else if (enable_reg[8] && wben_in)
			reg_8 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_9 <= 32'b0;
		else if (enable_reg[9] && wben_in)
			reg_9 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_10 <= 32'b0;
		else if (enable_reg[10] && wben_in)
			reg_10 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_11 <= 32'b0;
		else if (enable_reg[11] && wben_in)
			reg_11 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_12 <= 32'b0;
		else if (enable_reg[12] && wben_in)
			reg_12 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_13 <= 32'b0;
		else if (enable_reg[13] && wben_in)
			reg_13 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_14 <= 32'b0;
		else if (enable_reg[14] && wben_in)
			reg_14 <= valD_in;
							
	always @ (posedge clk) 
		if (!nRESET)
			reg_15 <= 32'b0;
		else if (enable_reg[15] && wben_in)
			reg_15 <= valD_in;
	/*
	
	always @(posedge clk) begin
		if (!nRESET) begin
			reg_0 <= 32'b0;
			reg_1 <= 32'b0;
			reg_2 <= 32'b0;
			reg_3 <= 32'b0;
			reg_4 <= 32'b0;
			reg_5 <= 32'b0;
			reg_6 <= 32'b0;
			reg_7 <= 32'b0;
			reg_8 <= 32'b0;
			reg_9 <= 32'b0;
			reg_10 <= 32'b0;
			reg_11 <= 32'b0;
			reg_12 <= 32'b0;
			reg_13 <= 32'b0;
			reg_14 <= 32'b0;
			reg_15 <= 32'b0;
		end else begin
			if (wben_in) begin
				case (dst_in)
					4'b0000: reg_0 <= valD_in;
					4'b0001: reg_1 <= valD_in;
					4'b0010: reg_2 <= valD_in;
					4'b0011: reg_3 <= valD_in;
					4'b0100: reg_4 <= valD_in;
					4'b0101: reg_5 <= valD_in;
					4'b0110: reg_6 <= valD_in;
					4'b0111: reg_7 <= valD_in;
					4'b1000: reg_8 <= valD_in;
					4'b1001: reg_9 <= valD_in;
					4'b1010: reg_10 <= valD_in;
					4'b1011: reg_11 <= valD_in;
					4'b1100: reg_12 <= valD_in;
					4'b1101: reg_13 <= valD_in;
					4'b1110: reg_14 <= valD_in;
					4'b1111: reg_15 <= valD_in;
					default: ;
				endcase
			end
		end
	end*/
	
	assign valA_out = (srcA_in == 4'b0000) ? reg_0 :
							(srcA_in == 4'b0001) ? reg_1 :
							(srcA_in == 4'b0010) ? reg_2 :
							(srcA_in == 4'b0011) ? reg_3 :
							(srcA_in == 4'b0100) ? reg_4 :
							(srcA_in == 4'b0101) ? reg_5 :
							(srcA_in == 4'b0110) ? reg_6 :
							(srcA_in == 4'b0111) ? reg_7 :
							(srcA_in == 4'b1000) ? reg_8 :
							(srcA_in == 4'b1001) ? reg_9 :
							(srcA_in == 4'b1010) ? reg_10 :
							(srcA_in == 4'b1011) ? reg_11 :
							(srcA_in == 4'b1100) ? reg_12 :
							(srcA_in == 4'b1101) ? reg_13 :
							(srcA_in == 4'b1110) ? reg_14 :
							(srcA_in == 4'b1111) ? reg_15 : 32'bx;
	
	assign valB_out = (srcB_in == 4'b0000) ? reg_0 :
							(srcB_in == 4'b0001) ? reg_1 :
							(srcB_in == 4'b0010) ? reg_2 :
							(srcB_in == 4'b0011) ? reg_3 :
							(srcB_in == 4'b0100) ? reg_4 :
							(srcB_in == 4'b0101) ? reg_5 :
							(srcB_in == 4'b0110) ? reg_6 :
							(srcB_in == 4'b0111) ? reg_7 :
							(srcB_in == 4'b1000) ? reg_8 :
							(srcB_in == 4'b1001) ? reg_9 :
							(srcB_in == 4'b1010) ? reg_10 :
							(srcB_in == 4'b1011) ? reg_11 :
							(srcB_in == 4'b1100) ? reg_12 :
							(srcB_in == 4'b1101) ? reg_13 :
							(srcB_in == 4'b1110) ? reg_14 :
							(srcB_in == 4'b1111) ? reg_15 : 32'bx;
	
endmodule


module nzcv_reg (clk, nRESET, alucc_in, setcc_in, nzcv_out);

	input clk, nRESET;
	input [3:0] alucc_in;
	input setcc_in;
	
	output reg [3:0] nzcv_out;
	
	
	always @(posedge clk) begin
		if (!nRESET)
			nzcv_out <= 4'b0;
		else if (setcc_in == 1'b1)
			nzcv_out <= alucc_in;
	end

endmodule
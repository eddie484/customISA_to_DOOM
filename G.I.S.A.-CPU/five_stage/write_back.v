module write_back (icode_in, load_in, lk_in, valE_in, valM_in, pcplus4_in, rD_in, wben_in, valD_out, rD_out, wben_out);
	
	input [31:0] valE_in, valM_in, pcplus4_in;
	input [5:0] icode_in;
	input [3:0] rD_in;
	input load_in, lk_in, wben_in;
	
	output [31:0] valD_out;
	output [3:0] rD_out;
	output wben_out;
	
	
	assign valD_out = lk_in ? pcplus4_in : (load_in ? valM_in : valE_in);
	
	assign rD_out = rD_in;
	assign wben_out = wben_in;
	
endmodule
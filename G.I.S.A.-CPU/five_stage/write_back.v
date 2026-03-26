module write_back (load_in, lk_in, valE_in, valM_in, pcplus4_in, valD_out);
	
	input [31:0] valE_in, valM_in, pcplus4_in;
	input load_in, lk_in;
	
	output [31:0] valD_out;
	
	
	assign valD_out = lk_in ? pcplus4_in : (load_in ? valM_in : valE_in);
	
	
endmodule
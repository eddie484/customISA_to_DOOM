module memory (clk, nRESET, dmen_in, dmrw_in, valE_in, wdata_in, dmsize_in, dmsext_in, valM_out);

	input clk, nRESET;
	input [31:0] valE_in, wdata_in;
	input [1:0] dmsize_in;
	input dmen_in, dmrw_in, dmsext_in;
	
	output [31:0] valM_out;
	
	
	dmem dmem_module (clk, nRESET, valE_in, dmen_in, dmrw_in, dmsize_in, dmsext_in, wdata_in, valM_out);
	
	
endmodule
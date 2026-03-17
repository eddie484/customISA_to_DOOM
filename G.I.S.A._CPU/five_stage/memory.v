module memory (clk, nRESET, icode_in, load_in, dmen_in, dmrw_in, lk_in, valE_in, wdata_in, dmsize_in, dmsext_in, pcplus4_in, rD_in, wben_in, icode_out, load_out, lk_out, valE_out, valM_out, pcplus4_out, rD_out, wben_out);

	input clk, nRESET;
	input [31:0] valE_in, wdata_in, pcplus4_in;
	input [5:0] icode_in;
	input [3:0] rD_in;
	input [1:0] dmsize_in;
	input load_in, dmen_in, dmrw_in, lk_in, dmsext_in, wben_in;
	
	output [31:0] valE_out, valM_out, pcplus4_out;
	output [5:0] icode_out;
	output [3:0] rD_out;
	output load_out, lk_out, wben_out;
	
	
	dmem dmem_module (clk, nRESET, valE_in, dmen_in, dmrw_in, dmsize_in, dmsext_in, wdata_in, valM_out);
	
	
	assign valE_out = valE_in;
	assign icode_out = icode_in;
	assign load_out = load_in;
	assign lk_out = lk_in;
	assign pcplus4_out = pcplus4_in;
	assign rD_out = rD_in;
	assign wben_out = wben_in;
	
endmodule
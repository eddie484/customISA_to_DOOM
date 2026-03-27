module memory (clk, nRESET, load_in, dmen_in, dmrw_in, lk_in, valE_in, wdata_in, dmsize_in, dmsext_in, PCplus4, valM_out, fwd_out);

	input clk, nRESET;
	input [31:0] valE_in, wdata_in, PCplus4;
	input [1:0] dmsize_in;
	input load_in, dmen_in, dmrw_in, dmsext_in, lk_in;
	
	output [31:0] valM_out, fwd_out;
	
	
	dmem dmem_module (clk, nRESET, valE_in, dmen_in, dmrw_in, dmsize_in, dmsext_in, wdata_in, valM_out);
	
	assign fwd_out = lk_in ? PCplus4 : (load_in ? valM_out : valE_in);
	
endmodule
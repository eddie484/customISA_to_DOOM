module counter_bus_mux(data0, data1, sel, result);
	
	input [7:0] data0, data1;
	input sel;
	
	output [7:0]result;
	
	
	assign result = (sel == 1'b1) ? data1 : data0;
	
endmodule
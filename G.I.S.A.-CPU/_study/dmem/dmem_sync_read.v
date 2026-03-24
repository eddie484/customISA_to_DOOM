module dmem (clk, nRESET, valE_in, dmen_in, dmrw_in, dmsize_in, dmsext_in, wdata_in, valM_out);

	input clk, nRESET;
	input [31:0] valE_in, wdata_in;
	input dmen_in, dmrw_in;
	input [1:0] dmsize_in;
	input dmsext_in;
	
	output reg [31:0] valM_out;
	
	reg [7:0] dmem [0:1023];
	
	integer i;
	
	
	always @(posedge clk) begin
		if (!nRESET) begin
			for (i = 0; i < 1024; i = i + 1) begin
				dmem[i] <= 8'b0;
			end
		end else if (dmen_in) begin
			if (dmrw_in) begin	// write
				case (dmsize_in)
					2'b00: begin
						dmem[valE_in + 3] <= wdata_in[31:24];
						dmem[valE_in + 2] <= wdata_in[23:16];
						dmem[valE_in + 1] <= wdata_in[15:8];
						dmem[valE_in] <= wdata_in[7:0];
					end
					2'b01: begin
						dmem[valE_in] <= wdata_in[7:0];
					end
					2'b10: begin
						dmem[valE_in + 1] <= wdata_in[15:8];
						dmem[valE_in] <= wdata_in[7:0];
					end
					default: ;
				endcase		
			end else begin		// read
				case (dmsize_in)
					2'b00: valM_out <= {dmem[valE_in + 3], dmem[valE_in + 2], dmem[valE_in + 1], dmem[valE_in]};
					2'b01: valM_out <= dmsext_in ? {{24{dmem[valE_in][7]}}, dmem[valE_in]} : {24'b0, dmem[valE_in]};
					2'b10: valM_out <= dmsext_in ? {{16{dmem[valE_in + 1][7]}}, dmem[valE_in + 1], dmem[valE_in]} : {16'b0, dmem[valE_in + 1], dmem[valE_in]};
					default: valM_out <= 32'b0;
				endcase
			end
		end
	end
endmodule
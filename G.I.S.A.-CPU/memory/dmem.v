(* keep_hierarchy *) module dmem (clk, nRESET, valE_in, dmen_in, dmrw_in, dmsize_in, dmsext_in, wdata_in, valM_out);

	input clk, nRESET;
	input [31:0] valE_in, wdata_in;
	input dmen_in, dmrw_in;
	input [1:0] dmsize_in;
	input dmsext_in;
	
	(* keep *) output reg [31:0] valM_out;
	
	wire [31:0] rdata;

	reg [31:0] wdata;
	reg [10:0] addr;
	reg [3:0] byte_enabled;
	reg [1:0] write_location;
	
	reg [1:0] read_size, read_location;
	reg read_sext;
	
	dmem_ram_1port dmem_ram_1port_module (.data(wdata), .address(addr), .wren(dmrw_in), .clock(clk), .clken(dmen_in), .rden(!dmrw_in), .byteena(byte_enabled), .q(rdata));
	
	
	always @(*) begin
		wdata = 32'b0;
		addr = 11'b0;
		byte_enabled = 4'b1111;
		write_location = 2'b0;
		valM_out = 32'b0;
		
		if (dmen_in) begin
			addr = valE_in[12:2];
			write_location = valE_in[1:0];
			if (dmrw_in) begin	// write
				case (dmsize_in)
					2'b00: begin
						byte_enabled = 4'b1111;
						wdata = wdata_in;
					end
					
					2'b01: begin
						case (write_location)
							2'b00: begin
									byte_enabled = 4'b0001;
									wdata = wdata_in;
								end
							2'b01: begin
									byte_enabled = 4'b0010;
									wdata = wdata_in << 4'b1000;
								end
							2'b10: begin
									byte_enabled = 4'b0100;
									wdata = wdata_in << 5'b10000;
								end
							2'b11: begin
									byte_enabled = 4'b1000;
									wdata = wdata_in << 5'b11000;
								end
							default: ;
						endcase
					end
					
					2'b10: begin
						case (write_location)
							2'b00: begin
								byte_enabled = 4'b0011;
								wdata = wdata_in;
							end
							2'b10: begin
								byte_enabled = 4'b1100;
								wdata = wdata_in << 5'b10000;
							end
							default: ;
						endcase
					end
					
					default: ;
				endcase		
			end 		
		end
		
		case (read_size) // read
			2'b00: valM_out = rdata;
			2'b01: begin
				case (read_location)
					2'b00: valM_out = read_sext ? {{24{rdata[7]}}, rdata[7:0]} : {24'b0, rdata[7:0]};
					2'b01: valM_out = read_sext ? {{24{rdata[15]}}, rdata[15:8]} : {24'b0, rdata[15:8]};
					2'b10: valM_out = read_sext ? {{24{rdata[23]}}, rdata[23:16]} : {24'b0, rdata[23:16]};
					2'b11: valM_out = read_sext ? {{24{rdata[31]}}, rdata[31:24]} : {24'b0, rdata[31:24]};
					default: ;
				endcase
			end
			2'b10: begin
				case (read_location)
					2'b00: valM_out = read_sext ? {{16{rdata[15]}}, rdata[15:0]} : {16'b0, rdata[15:0]};
					2'b10: valM_out = read_sext ? {{16{rdata[31]}}, rdata[31:16]} : {16'b0, rdata[31:16]};
					default: ;
				endcase
			end
			default: valM_out = 32'b0;
		endcase	
	end
	
	always @(posedge clk) begin
		if (!nRESET) begin
			read_size <= 2'b0;
			read_location <= 2'b0;
			read_sext <= 1'b0;
		end else if (dmen_in && !dmrw_in) begin
			read_size <= dmsize_in;
			read_location <= valE_in[1:0];
			read_sext <= dmsext_in;
		end
	end
	
endmodule
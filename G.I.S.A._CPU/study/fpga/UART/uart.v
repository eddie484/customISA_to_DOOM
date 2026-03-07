module uart (clk, nRESET, rdx, data_out, outen);
	
	input clk, nRESET;
	input rdx;
	
	output reg [7:0] data_out;
	output reg outen;
	
	reg [3:0] state;
	reg [8:0] counter;
	
	localparam IDLE = 4'b0000;
	localparam START = 4'b0001;
	localparam STOP = 4'b0010;
	localparam DATA_0 = 4'b1000;
	localparam DATA_1 = 4'b1001;
	localparam DATA_2 = 4'b1010;
	localparam DATA_3 = 4'b1011;
	localparam DATA_4 = 4'b1100;
	localparam DATA_5 = 4'b1101;
	localparam DATA_6 = 4'b1110;
	localparam DATA_7 = 4'b1111;
	
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) begin
			state <= IDLE;
			data_out <= 8'b0;
			outen <= 1'b0;
		end else begin
			case (state)
				IDLE: begin
					data_out <= 8'b0;
					outen <= 1'b0;
					counter <= 9'b0;
					if (rdx == 1'b0) state <= START;
				end
				
				START: begin
					counter <= counter + 1;
					if (counter >= 9'b011011001) begin
						if (rdx == 1'b0) begin
							state <= DATA_0;
							counter <= 9'b0;
						end else begin
							data_out <= 8'b0;
							state <= IDLE;
							counter <= 9'b0;
						end
					end
				end
				
				DATA_0: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						data_out[0] <= rdx;
						state <= DATA_1;
						counter <= 9'b0;
					end
				end 
				
				DATA_1: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						data_out[1] <= rdx;
						state <= DATA_2;
						counter <= 9'b0;
					end
				end 
				
				DATA_2: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						data_out[2] <= rdx;
						state <= DATA_3;
						counter <= 9'b0;
					end
				end 
				
				DATA_3: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						data_out[3] <= rdx;
						state <= DATA_4;
						counter <= 9'b0;
					end
				end 
				
				DATA_4: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						data_out[4] <= rdx;
						state <= DATA_5;
						counter <= 9'b0;
					end
				end 
				
				DATA_5: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						data_out[5] <= rdx;
						state <= DATA_6;
						counter <= 9'b0;
					end
				end 
				
				DATA_6: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						data_out[6] <= rdx;
						state <= DATA_7;
						counter <= 9'b0;
					end
				end 
				
				DATA_7: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						data_out[7] <= rdx;
						state <= STOP;
						counter <= 9'b0;
					end
				end 
				
				STOP: begin
					counter <= counter + 1;
					if (counter >= 9'b110110010) begin
						if (rdx == 1'b1) begin
							state <= IDLE;
							outen <= 1'b1;
							counter <= 9'b0;
						end else begin
							data_out <= 8'b11111111;
							state <= IDLE;
							counter <= 9'b0;
						end
					end
				end 
			endcase
		end
	end				
	
	
endmodule
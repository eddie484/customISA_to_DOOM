module uart (clk, nRESET, rdx, data_in, send, tdx, data_out, outen);
	
	input clk, nRESET;
	input rdx, send;
	input [7:0] data_in;
	
	output reg [7:0] data_out;
	output reg outen, tdx;
	
	reg [3:0] r_state;
	reg [3:0] t_state;
	reg [8:0] r_counter;
	reg [8:0] t_counter;
	
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
			r_state <= IDLE;
			data_out <= 8'b0;
			outen <= 1'b0;
		end else begin
			case (r_state)
				IDLE: begin
					data_out <= 8'b0;
					outen <= 1'b0;
					r_counter <= 9'b0;
					if (rdx == 1'b0) r_state <= START;
				end
				
				START: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b011011001) begin
						if (rdx == 1'b0) begin
							r_state <= DATA_0;
							r_counter <= 9'b0;
						end else begin
							data_out <= 8'b0;
							r_state <= IDLE;
							r_counter <= 9'b0;
						end
					end
				end
				
				DATA_0: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						data_out[0] <= rdx;
						r_state <= DATA_1;
						r_counter <= 9'b0;
					end
				end 
				
				DATA_1: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						data_out[1] <= rdx;
						r_state <= DATA_2;
						r_counter <= 9'b0;
					end
				end 
				
				DATA_2: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						data_out[2] <= rdx;
						r_state <= DATA_3;
						r_counter <= 9'b0;
					end
				end 
				
				DATA_3: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						data_out[3] <= rdx;
						r_state <= DATA_4;
						r_counter <= 9'b0;
					end
				end 
				
				DATA_4: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						data_out[4] <= rdx;
						r_state <= DATA_5;
						r_counter <= 9'b0;
					end
				end 
				
				DATA_5: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						data_out[5] <= rdx;
						r_state <= DATA_6;
						r_counter <= 9'b0;
					end
				end 
				
				DATA_6: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						data_out[6] <= rdx;
						r_state <= DATA_7;
						r_counter <= 9'b0;
					end
				end 
				
				DATA_7: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						data_out[7] <= rdx;
						r_state <= STOP;
						r_counter <= 9'b0;
					end
				end 
				
				STOP: begin
					r_counter <= r_counter + 1;
					if (r_counter >= 9'b110110010) begin
						if (rdx == 1'b1) begin
							r_state <= IDLE;
							outen <= 1'b1;
							r_counter <= 9'b0;
						end else begin
							data_out <= 8'b11111111;
							r_state <= IDLE;
							r_counter <= 9'b0;
						end
					end
				end 
			endcase
		end
	end				
	
	
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) begin
			t_state <= IDLE;
			tdx <= 1'b1;
		end else begin
			case (t_state)
				IDLE: begin
					tdx <= 1'b1;
					t_counter <= 9'b0;
					if (send == 1'b1) t_state <= START;
				end
				
				START: begin
					tdx <= 1'b0;
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= DATA_0;
							t_counter <= 9'b0;
					end
				end
				
				DATA_0: begin
					tdx <= data_in[0];
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= DATA_1;
							t_counter <= 9'b0;
					end
				end
				
				DATA_1: begin
					tdx <= data_in[1];
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= DATA_2;
							t_counter <= 9'b0;
					end
				end
				
				DATA_2: begin
					tdx <= data_in[2];
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= DATA_3;
							t_counter <= 9'b0;
					end
				end
				
				DATA_3: begin
					tdx <= data_in[3];
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= DATA_4;
							t_counter <= 9'b0;
					end
				end
				
				DATA_4: begin
					tdx <= data_in[4];
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= DATA_5;
							t_counter <= 9'b0;
					end
				end
				
				DATA_5: begin
					tdx <= data_in[5];
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= DATA_6;
							t_counter <= 9'b0;
					end
				end 
				
				DATA_6: begin
					tdx <= data_in[6];
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= DATA_7;
							t_counter <= 9'b0;
					end
				end
				
				DATA_7: begin
					tdx <= data_in[7];
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= STOP;
							t_counter <= 9'b0;
					end
				end
				
				STOP: begin
					tdx <= 1'b1;
					t_counter <= t_counter + 1;
					if (t_counter >= 9'b110110010) begin
							t_state <= IDLE;
							t_counter <= 9'b0;
					end
				end 
			endcase
		end
	end			
	
endmodule
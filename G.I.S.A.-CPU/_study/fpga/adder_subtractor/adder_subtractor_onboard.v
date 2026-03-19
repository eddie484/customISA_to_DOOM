module adder_subtractor_onboard (clk, nRESET, switch, result_send_button, rdx, tdx, led);

	input clk, nRESET;	// BUTTON[1]
	input result_send_button;	// BUTTON[0]
	input rdx;
	input [3:0] switch;	// [3]: valA 세팅, [2]: valB 세팅, [1]: sign 세팅, [0]: .
	
	output tdx;
	output reg [7:0] led;		// [7:4]: switch, [3]: result_send_button, [2]: nRESET, [1]: carry_out, [0]: result[0].

	wire [31:0] valA, valB;
	wire [31:0] result;
	wire sign, carry_out, result_send_sig;
	
	wire [7:0] uart_receive;	// 라인에서 온 신호를 해석한 값
	wire uart_outen;		// 신호가 해석되었음을 표시. 1일 때 uart_receive이 사용 가능하다.
	wire [3:0] binary;	// 라인에서 받은 값을 이진수로 변환한 값
	wire result_send_pos;	// result_send 버튼의 pos edge를 표시.
	wire [7:0] uart_send;
	
	reg [31:0] input_A, input_B;	// 이진수로 변환된 신호를 순차적으로 저장한다.
	reg [31:0] output_result;
	reg [7:0] sending_data;
	reg [5:0] sending_state;
	reg [12:0] sending_counter;
	reg result_sending;
	
	localparam IDLE = 6'b100000;
	localparam LF = 6'b100001;
	localparam CR = 6'b100010;
	localparam COUT = 6'b100011;
	localparam US = 6'b100100;
	localparam B0 = 6'b000000;
	localparam B1 = 6'b000001;
	localparam B2 = 6'b000010;
	localparam B3 = 6'b000011;
	localparam B4 = 6'b000100;
	localparam B5 = 6'b000101;
	localparam B6 = 6'b000110;
	localparam B7 = 6'b000111;
	localparam B8 = 6'b001000;
	localparam B9 = 6'b001001;
	localparam B10 = 6'b001010;
	localparam B11 = 6'b001011;
	localparam B12 = 6'b001100;
	localparam B13 = 6'b001101;
	localparam B14 = 6'b001110;
	localparam B15 = 6'b001111;
	localparam B16 = 6'b010000;
	localparam B17 = 6'b010001;
	localparam B18 = 6'b010010;
	localparam B19 = 6'b010011;
	localparam B20 = 6'b010100;
	localparam B21 = 6'b010101;
	localparam B22 = 6'b010110;
	localparam B23 = 6'b010111;
	localparam B24 = 6'b011000;
	localparam B25 = 6'b011001;
	localparam B26 = 6'b011010;
	localparam B27 = 6'b011011;
	localparam B28 = 6'b011100;
	localparam B29 = 6'b011101;
	localparam B30 = 6'b011110;
	localparam B31 = 6'b011111;
	
	integer i;
	
	
	assign sign = switch[1];
	assign result_send_sig = ~result_send_button;
	assign valA = input_A;
	assign valB = input_B;
	//assign led = ~{switch, result_send_sig, nRESET, carry_out, 1'b0};
	
	
	edge_checker result_send_button_edge (clk, nRESET, result_send_sig, result_send_pos, );
	uart uart_core (clk, nRESET, rdx, uart_send, result_sending, tdx, uart_receive, uart_outen);
	ascii_bin_translator ascii_to_bin (uart_receive, 0, binary);
	ascii_bin_translator bin_to_ascii (sending_data, 1, uart_send);
	adder add_sub_module (valA, valB, sign, carry_out, result);
	
	
	always @(posedge clk) begin
		if (!nRESET) begin
			input_A <= 32'b0;
			input_B <= 32'b0;
			result_sending <= 1'b0;
			sending_state <= IDLE;
			sending_counter <= 0;
		end else if (result_send_pos || result_sending) begin
		/*
			sending_data <= result[0];
			result_sending <= 1'b1;
			sending_counter <= sending_counter + 1'b1;
			if (sending_counter >= 13'b1000011110100) begin
				sending_counter <= 13'b0;
				result_sending <= 1'b0;
			end*/
		
			case (sending_state)
				IDLE: begin
					sending_counter <= 13'b0;
					result_sending <= 1'b0;
					if (result_send_pos) begin
						sending_state <= COUT;
						result_sending <= 1'b1;
					end
				end
				
				COUT: begin
					sending_data <= carry_out;
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= US;
						sending_counter <= 13'b0;
					end
				end
				
				US: begin
					sending_data <= 8'b01011111;
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B31;
						sending_counter <= 13'b0;
					end
				end
				
				B31: begin
					sending_data <= result[31];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B30;
						sending_counter <= 13'b0;
					end
				end
				
				B30: begin
					sending_data <= result[30];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B29;
						sending_counter <= 13'b0;
					end
				end
				
				B29: begin
					sending_data <= result[29];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B28;
						sending_counter <= 13'b0;
					end
				end
				
				B28: begin
					sending_data <= result[28];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B27;
						sending_counter <= 13'b0;
					end
				end
				
				B27: begin
					sending_data <= result[27];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B26;
						sending_counter <= 13'b0;
					end
				end
				
				B26: begin
					sending_data <= result[26];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B25;
						sending_counter <= 13'b0;
					end
				end
				
				B25: begin
					sending_data <= result[25];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B24;
						sending_counter <= 13'b0;
					end
				end
				
				B24: begin
					sending_data <= result[24];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B23;
						sending_counter <= 13'b0;
					end
				end
				
				B23: begin
					sending_data <= result[23];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B22;
						sending_counter <= 13'b0;
					end
				end
				
				B22: begin
					sending_data <= result[22];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B21;
						sending_counter <= 13'b0;
					end
				end
				
				B21: begin
					sending_data <= result[21];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B20;
						sending_counter <= 13'b0;
					end
				end
				
				B20: begin
					sending_data <= result[20];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B19;
						sending_counter <= 13'b0;
					end
				end
				
				B19: begin
					sending_data <= result[19];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B18;
						sending_counter <= 13'b0;
					end
				end
				
				B18: begin
					sending_data <= result[18];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B17;
						sending_counter <= 13'b0;
					end
				end
				
				B17: begin
					sending_data <= result[17];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B16;
						sending_counter <= 13'b0;
					end
				end
				
				B16: begin
					sending_data <= result[16];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B15;
						sending_counter <= 13'b0;
					end
				end
				
				B15: begin
					sending_data <= result[15];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B14;
						sending_counter <= 13'b0;
					end
				end
				
				B14: begin
					sending_data <= result[14];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B13;
						sending_counter <= 13'b0;
					end
				end
				
				B13: begin
					sending_data <= result[13];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B12;
						sending_counter <= 13'b0;
					end
				end
				
				B12: begin
					sending_data <= result[12];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B11;
						sending_counter <= 13'b0;
					end
				end
				
				B11: begin
					sending_data <= result[11];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B10;
						sending_counter <= 13'b0;
					end
				end
				
				B10: begin
					sending_data <= result[10];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B9;
						sending_counter <= 13'b0;
					end
				end
				
				B9: begin
					sending_data <= result[9];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B8;
						sending_counter <= 13'b0;
					end
				end
				
				B8: begin
					sending_data <= result[8];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B7;
						sending_counter <= 13'b0;
					end
				end
				
				B7: begin
					sending_data <= result[7];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B6;
						sending_counter <= 13'b0;
					end
				end
				
				B6: begin
					sending_data <= result[6];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B5;
						sending_counter <= 13'b0;
					end
				end
				
				B5: begin
					sending_data <= result[5];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B4;
						sending_counter <= 13'b0;
					end
				end
				
				B4: begin
					sending_data <= result[4];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B3;
						sending_counter <= 13'b0;
					end
				end
				
				B3: begin
					sending_data <= result[3];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B2;
						sending_counter <= 13'b0;
					end
				end
				
				B2: begin
					sending_data <= result[2];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B1;
						sending_counter <= 13'b0;
					end
				end
				
				B1: begin
					sending_data <= result[1];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B0;
						sending_counter <= 13'b0;
					end
				end
				
				B0: begin
					sending_data <= result[0];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= LF;
						sending_counter <= 13'b0;
					end
				end
				
				LF: begin
					sending_data <= 8'b00001010;
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= CR;
						sending_counter <= 13'b0;
					end
				end
				
				CR: begin
					sending_data <= 8'b00001101;
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= IDLE;
						sending_counter <= 13'b0;
					end
				end
			endcase
				
			
		end else if (uart_outen) begin		// 신호 해석이 완료되어 uart_receive에 받은 값이 제대로 들어간 경우
			if (switch[3] == 1'b1) begin		// valA에 값을 저장하기
				if (binary == 1'b1) begin
					input_A <= {input_A[30:0], 1'b1};
				end else begin
					input_A <= {input_A[30:0], 1'b0};
				end
			end else if (switch[2] == 1'b1) begin
				if (binary == 1'b1) begin
					input_B <= {input_B[30:0], 1'b1};
				end else begin
					input_B <= {input_B[30:0], 1'b0};
				end
			end
		end else led <= ~result[7:0];
	end
	
endmodule
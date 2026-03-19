module multiplier_onboard (clk, nRESET, switch, result_send_button, rdx, tdx, led);

	input clk, nRESET;	// BUTTON[1]
	input result_send_button;	// BUTTON[0]
	input rdx;
	input [3:0] switch;	// [3]: valA 세팅, [2]: valB 세팅, [1]: sign 세팅, [0]: .
	
	output tdx;
	output reg [7:0] led;		// [7:4]: switch, [3]: result_send_button, [2]: nRESET, [1]: carry_out, [0]: result[0].

	wire [31:0] valA, valB;
	wire [63:0] result;
	wire sign, carry_out, result_send_sig;
	
	wire [7:0] uart_receive;	// 라인에서 온 신호를 해석한 값
	wire uart_outen;		// 신호가 해석되었음을 표시. 1일 때 uart_receive이 사용 가능하다.
	wire [3:0] binary;	// 라인에서 받은 값을 이진수로 변환한 값
	wire result_send_pos;	// result_send 버튼의 pos edge를 표시.
	wire [7:0] uart_send;
	
	reg [31:0] input_A, input_B;	// 이진수로 변환된 신호를 순차적으로 저장한다.
	reg [31:0] output_result;
	reg [7:0] sending_data;
	reg [6:0] sending_state;
	reg [12:0] sending_counter;
	reg result_sending;
	
	localparam IDLE = 7'b1000000;
	localparam LF = 7'b1000001;
	localparam CR = 7'b1000010;
	localparam COUT = 7'b1000011;
	localparam US = 7'b1000100;
	localparam B0 = 7'b0000000;
	localparam B1 = 7'b0000001;
	localparam B2 = 7'b0000010;
	localparam B3 = 7'b0000011;
	localparam B4 = 7'b0000100;
	localparam B5 = 7'b0000101;
	localparam B6 = 7'b0000110;
	localparam B7 = 7'b0000111;
	localparam B8 = 7'b0001000;
	localparam B9 = 7'b0001001;
	localparam B10 = 7'b0001010;
	localparam B11 = 7'b0001011;
	localparam B12 = 7'b0001100;
	localparam B13 = 7'b0001101;
	localparam B14 = 7'b0001110;
	localparam B15 = 7'b0001111;
	localparam B16 = 7'b0010000;
	localparam B17 = 7'b0010001;
	localparam B18 = 7'b0010010;
	localparam B19 = 7'b0010011;
	localparam B20 = 7'b0010100;
	localparam B21 = 7'b0010101;
	localparam B22 = 7'b0010110;
	localparam B23 = 7'b0010111;
	localparam B24 = 7'b0011000;
	localparam B25 = 7'b0011001;
	localparam B26 = 7'b0011010;
	localparam B27 = 7'b0011011;
	localparam B28 = 7'b0011100;
	localparam B29 = 7'b0011101;
	localparam B30 = 7'b0011110;
	localparam B31 = 7'b0011111;
	
	localparam B32 = 7'b0100000;
	localparam B33 = 7'b0100001;
	localparam B34 = 7'b0100010;
	localparam B35 = 7'b0100011;
	localparam B36 = 7'b0100100;
	localparam B37 = 7'b0100101;
	localparam B38 = 7'b0100110;
	localparam B39 = 7'b0100111;
	localparam B40 = 7'b0101000;
	localparam B41 = 7'b0101001;
	localparam B42 = 7'b0101010;
	localparam B43 = 7'b0101011;
	localparam B44 = 7'b0101100;
	localparam B45 = 7'b0101101;
	localparam B46 = 7'b0101110;
	localparam B47 = 7'b0101111;
	localparam B48 = 7'b0110000;
	localparam B49 = 7'b0110001;
	localparam B50 = 7'b0110010;
	localparam B51 = 7'b0110011;
	localparam B52 = 7'b0110100;
	localparam B53 = 7'b0110101;
	localparam B54 = 7'b0110110;
	localparam B55 = 7'b0110111;
	localparam B56 = 7'b0111000;
	localparam B57 = 7'b0111001;
	localparam B58 = 7'b0111010;
	localparam B59 = 7'b0111011;
	localparam B60 = 7'b0111100;
	localparam B61 = 7'b0111101;
	localparam B62 = 7'b0111110;
	localparam B63 = 7'b0111111;
	
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
	multiplier mul_module (valA, valB, sign, result);
	
	
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
						sending_state <= B63;
						result_sending <= 1'b1;
					end
				end
				
				B63: begin
					sending_data <= result[63];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B62;
						sending_counter <= 13'b0;
					end
				end
				
				B62: begin
					sending_data <= result[62];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B61;
						sending_counter <= 13'b0;
					end
				end
				
				B61: begin
					sending_data <= result[61];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B60;
						sending_counter <= 13'b0;
					end
				end
				
				B60: begin
					sending_data <= result[60];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B59;
						sending_counter <= 13'b0;
					end
				end
				
				B59: begin
					sending_data <= result[59];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B58;
						sending_counter <= 13'b0;
					end
				end
				
				B58: begin
					sending_data <= result[58];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B57;
						sending_counter <= 13'b0;
					end
				end
				
				B57: begin
					sending_data <= result[57];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B56;
						sending_counter <= 13'b0;
					end
				end
				
				B56: begin
					sending_data <= result[56];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B55;
						sending_counter <= 13'b0;
					end
				end
				
				B55: begin
					sending_data <= result[55];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B54;
						sending_counter <= 13'b0;
					end
				end
				
				B54: begin
					sending_data <= result[54];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B53;
						sending_counter <= 13'b0;
					end
				end
				
				B53: begin
					sending_data <= result[53];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B52;
						sending_counter <= 13'b0;
					end
				end
				
				B52: begin
					sending_data <= result[52];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B51;
						sending_counter <= 13'b0;
					end
				end
				
				B51: begin
					sending_data <= result[51];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B50;
						sending_counter <= 13'b0;
					end
				end
				
				B50: begin
					sending_data <= result[50];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B49;
						sending_counter <= 13'b0;
					end
				end
				
				B49: begin
					sending_data <= result[49];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B48;
						sending_counter <= 13'b0;
					end
				end
				
				B48: begin
					sending_data <= result[48];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B47;
						sending_counter <= 13'b0;
					end
				end
				
				B47: begin
					sending_data <= result[47];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B46;
						sending_counter <= 13'b0;
					end
				end
				
				B46: begin
					sending_data <= result[46];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B45;
						sending_counter <= 13'b0;
					end
				end
				
				B45: begin
					sending_data <= result[45];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B44;
						sending_counter <= 13'b0;
					end
				end
				
				B44: begin
					sending_data <= result[44];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B43;
						sending_counter <= 13'b0;
					end
				end
				
				B43: begin
					sending_data <= result[43];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B42;
						sending_counter <= 13'b0;
					end
				end
				
				B42: begin
					sending_data <= result[42];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B41;
						sending_counter <= 13'b0;
					end
				end
				
				B41: begin
					sending_data <= result[41];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B40;
						sending_counter <= 13'b0;
					end
				end
				
				B40: begin
					sending_data <= result[40];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B39;
						sending_counter <= 13'b0;
					end
				end
				
				B39: begin
					sending_data <= result[39];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B38;
						sending_counter <= 13'b0;
					end
				end
				
				B38: begin
					sending_data <= result[38];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B37;
						sending_counter <= 13'b0;
					end
				end
				
				B37: begin
					sending_data <= result[37];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B36;
						sending_counter <= 13'b0;
					end
				end
				
				B36: begin
					sending_data <= result[36];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B35;
						sending_counter <= 13'b0;
					end
				end
				
				B35: begin
					sending_data <= result[35];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B34;
						sending_counter <= 13'b0;
					end
				end
				
				B34: begin
					sending_data <= result[34];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B33;
						sending_counter <= 13'b0;
					end
				end
				
				B33: begin
					sending_data <= result[33];
					result_sending <= 1'b1;
					sending_counter <= sending_counter + 1'b1;
					if (sending_counter >= 13'b1000011110100) begin
						sending_state <= B32;
						sending_counter <= 13'b0;
					end
				end
				
				B32: begin
					sending_data <= result[32];
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
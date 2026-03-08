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
	
	reg [31:0] input_A, input_B;	// 이진수로 변환된 신호를 순차적으로 저장한다.
	reg [31:0] output_result;
	reg [7:0] tsr;
	
	
	assign sign = switch[1];
	assign result_send_sig = ~result_send_button;
	assign valA = input_A;
	assign valB = input_B;
	//assign led = ~{switch, result_send_sig, nRESET, carry_out, 1'b0};
	
	
	edge_checker result_send_button_edge (clk, nRESET, result_send_sig, result_send_pos, );
	uart uart_core (clk, nRESET, rdx, tsr, result_send_pos, tdx, uart_receive, uart_outen);
	ascii_bin_translator ascii_to_bin (uart_receive, 0, binary);
	adder add_sub_module (valA, valB, sign, carry_out, result);
	
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) begin
			input_A <= 32'b0;
			input_B <= 32'b0;
		end
		else if (uart_outen) begin		// 신호 해석이 완료되어 uart_receive에 받은 값이 제대로 들어간 경우
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
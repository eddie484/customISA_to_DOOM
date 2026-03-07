module uart_onboard (clk, nRESET, switch, send, rdx, tdx, LED);
	
	input clk, nRESET;	// nRESET = 보드의 버튼 [1]
	input rdx;		// 라인에서 받아오는 신호
	input [3:0] switch;		// 보드의 스위치
	input send;		// 라인으로 값을 발사하는 버튼. 보드의 버튼[0]
	
	output reg [7:0] LED;	// 보드의 LED
	output tdx;		// 라인으로 보내는 신호
	
	wire [7:0] uart_receive;	// 라인에서 온 신호를 해석한 값
	wire uart_outen;		// 신호가 해석되었음을 표시. 1일 때 uart_receive이 사용 가능하다.
	wire [3:0] binary;	// 라인에서 받은 값을 이진수로 변환한 값
	wire [7:0] send_data;	// 라인으로 보낼 값
	
	reg [7:0] give_back;
	
	
	uart uart_core (clk, nRESET, rdx, send_data, ~send, tdx, uart_receive, uart_outen);
	ascii_to_bin bin_making (uart_receive, binary);		// 아스키 코드로 표현된 16진수 수신값을 이진수로 변환한다.
	assign send_data = give_back + switch;		// 받은 값에 스위치만큼 더해 보낼 값을 구성한다.
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) LED <= 8'b0;		// LED를 리셋
		else if (uart_outen) begin		// 신호 해석이 완료되어 uart_receive에 받은 값이 제대로 들어간 경우
			LED <= ~binary;		// 받은 값을 이진수로 LED에 표시한다.
			give_back <= uart_receive;
		end
	end
	
endmodule
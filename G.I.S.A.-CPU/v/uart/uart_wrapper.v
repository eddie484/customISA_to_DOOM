`include "v/defines.v"

module uart_wrapper (clk, nRESET, dmrw_in, dmen_in, valE_in, wdata_in, rdx_in, tdx_out, uart_char_out);

	input clk, nRESET;
	input rdx_in;
	input [31:0] valE_in, wdata_in;
	input dmrw_in, dmen_in;
	
	output tdx_out;
	output reg [31:0] uart_char_out;
	
	wire send_command_sig, receive_command_sig;
	wire [7:0] uart_receive;
	wire uart_outen;
	
	reg receive_command_delay;
	reg [7:0] send_data;
	
	
	assign send_command_sig = dmrw_in & dmen_in & (valE_in == `MEMM_UART);
	assign receive_command_sig = !dmrw_in & dmen_in & (valE_in == `MEMM_UART);
	
	uart uart_core (clk, nRESET, rdx_in, send_data, send_command_sig, tdx_out, uart_receive, uart_outen);
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) begin
			uart_char_out <= 32'h0000_0100;
			receive_command_delay <= 1'b0;
			send_data <= 8'b0;
		end else begin
			if (uart_outen) begin		
				uart_char_out <= {24'b0, uart_receive};
				receive_command_delay <= receive_command_sig;
			end else if (receive_command_delay) begin		// receive check는 이제 값을 읽은 다음 사이클에 처리되므로 우선순위가 낮다. 읽은 다음 사이클에 바로 uart가 들어오면, 새 값이 저장되는 것이므로 읽은 값은 사라지기 때문.
				uart_char_out <= 32'h0000_0100;
				receive_command_delay <= receive_command_sig;
			end else receive_command_delay <= receive_command_sig;
			
			if (send_command_sig) begin
				send_data <= wdata_in[7:0];
			end
			
		end
	end

endmodule
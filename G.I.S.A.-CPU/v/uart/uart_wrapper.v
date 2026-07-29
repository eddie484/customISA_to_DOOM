`include "v/defines.v"

module uart_wrapper (clk, nRESET, dmrw_in, dmen_in, valE_in, wdata_in, rdx_in, tdx_out, uart_char_out);

	input clk, nRESET;
	input rdx_in;
	input [31:0] valE_in, wdata_in;
	input dmrw_in, dmen_in;
	
	output tdx_out;
	output reg [32:0] uart_char_out;
	
	wire send_command_sig, receive_command_sig;
	wire [7:0] uart_receive;
	wire uart_outen;
	
	
	assign send_command_sig = dmrw_in & dmen_in & (valE_in == `MEMM_UART);
	assign receive_command_sig = !dmrw_in & dmen_in & (valE_in == `MEMM_UART);
	
	uart uart_core (clk, nRESET, rdx_in, wdata_in[7:0], send_command_sig, tdx_out, uart_receive, uart_outen);
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) uart_char_out <= 8'b0;		
		else if (receive_command_sig) begin		// receive check의 우선순위가 uart값보다 높다. 읽었다는 사실이 유실되면 다음 읽기에서 읽은 값인지 새 값인지 구분 불가능해지기 때문.
			uart_char_out <= 32'h0000_0100;
		end else if (uart_outen) begin		
			uart_char_out <= {24'b0, uart_receive};
		end
	end

endmodule
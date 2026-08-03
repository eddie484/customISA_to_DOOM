`include "v/defines.v"

module uart_wrapper (clk, nRESET, dmrw_in, dmen_in, valE_in, wdata_in, rdx_in, tdx_out, uart_result_out);

	input clk, nRESET;
	input rdx_in;
	input [31:0] valE_in, wdata_in;
	input dmrw_in, dmen_in;
	
	output tdx_out;
	output reg [31:0] uart_result_out;
	
	wire send_command_sig, receive_command_sig, status_receive_sig;
	wire [7:0] uart_receive;
	wire uart_outen, tdx_busy;
	wire [31:0] status_reg;	// [0]: tdx_busy
	
	reg receive_command_delay;
	reg [7:0] send_data;
	reg [31:0] receive_data;
	
	assign send_command_sig = dmrw_in & dmen_in & (valE_in == `MEMM_UART);
	assign receive_command_sig = !dmrw_in & dmen_in & (valE_in == `MEMM_UART);
	assign status_receive_sig = !dmrw_in & dmen_in & (valE_in == `MEMM_UART_STATUS);
	
	assign status_reg = {31'b0, tdx_busy};
	
	uart uart_core (clk, nRESET, rdx_in, send_data, send_command_sig, tdx_out, uart_receive, uart_outen, tdx_busy);
	
	always @(posedge clk or negedge nRESET) begin
		if (!nRESET) begin
			receive_data <= 32'h0000_0100;
			send_data <= 8'b0;
			uart_result_out <= 32'b0;
		end else begin
			if (uart_outen) begin		
				receive_data <= {24'b0, uart_receive};
			end else if (receive_command_sig) begin		// receive check는 이제 값을 읽은 다음 사이클에 처리되므로 우선순위가 낮다. 읽은 다음 사이클에 바로 uart가 들어오면, 새 값이 저장되는 것이므로 읽은 값은 사라지기 때문.
				receive_data <= 32'h0000_0100;
			end
			
			if (send_command_sig) begin
				send_data <= wdata_in[7:0];
			end
			
			if (receive_command_sig) begin
				uart_result_out <= receive_data;
			end else if (status_receive_sig) begin
				uart_result_out <= status_reg;
			end else begin
				uart_result_out <= 32'b0;
			end
			
		end
	end

endmodule
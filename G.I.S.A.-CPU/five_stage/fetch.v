/*
	5단계 파이프라인 중 1단계인 Fetch.
	pc에 따라 imem에 저장된 instr을 가져오고,
	branch predictor를 이용해 branch 명령일 경우 pc 이동 주소를 구한다.
	
	데이터 출력에 시간이 필요하다는 동기 메모리 특성을 고려하여, 
	imem_out, dmem_out의 파이프라인 레지스터는 imem_module,dmem_module에서 
	미리 처리하여 결과를 내보낸다.
*/

(* keep_hierarchy *) module fetch (clk, nRESET, pc_in, predPC_out, PCplus4_out, instr_out, stall_D, bubble_D);
	
	input clk, nRESET, stall_D, bubble_D;
	
	input [31:0] pc_in;
	
	(* keep *) output [31:0] predPC_out, PCplus4_out, instr_out;
	
	wire [31:0] instr, PCplus4, predPC;
	
	
	imem imem_module (clk, nRESET, stall_D, bubble_D, pc_in, instr);
	branch_predictor bp_module (pc_in, instr[31:26], {{10{instr[25]}}, instr[25:4]}, PCplus4, predPC);
	
	assign predPC_out = predPC;
	assign PCplus4_out = PCplus4;
	assign instr_out = instr;
	
endmodule
/*
	5단계 파이프라인 중 4단계인 Memory.
	연산 결과를 Dmem에 저장하거나, 저장된 값을 가져온다.
	
	데이터 출력에 시간이 필요하다는 동기 메모리 특성을 고려하여, 
	imem_out, dmem_out의 파이프라인 레지스터는 imem_module,dmem_module에서 
	미리 처리하여 결과를 내보낸다.
*/

(* keep_hierarchy *) module memory (clk, nRESET, load_in, lk_in, valE_in, dmsize_in, dmsext_in, PCplus4, valE_DMEM, wdata_DMEM, dmsize_DMEM, dmen_DMEM, dmrw_DMEM, dmsext_DMEM, valM_out, fwd_out);

	input clk, nRESET;
	input [31:0] valE_in, PCplus4;
	input [1:0] dmsize_in;
	input load_in, dmsext_in, lk_in;
	
	input [31:0] valE_DMEM, wdata_DMEM;
	input [1:0] dmsize_DMEM;
	input dmen_DMEM, dmrw_DMEM, dmsext_DMEM;
	
	(* keep *) output [31:0] valM_out, fwd_out;
	
	
	dmem dmem_module (clk, nRESET, valE_in, dmsize_in, dmsext_in, valE_DMEM, wdata_DMEM, dmsize_DMEM, dmen_DMEM, dmrw_DMEM, dmsext_DMEM, valM_out);
	
	assign fwd_out = lk_in ? PCplus4 : (load_in ? valM_out : valE_in);
	
endmodule
/*
	5단계 파이프라인 중 5단계인 Write-Back.
	명령어를 수행해 얻은 최종 결과를 결정하고, 이를 reg_file에 저장한다.
*/

(* keep_hierarchy *) module write_back (load_in, lk_in, valE_in, valM_in, pcplus4_in, valD_out);
	
	input [31:0] valE_in, valM_in, pcplus4_in;
	input load_in, lk_in;
	
	(* keep *) output [31:0] valD_out;
	
	
	assign valD_out = lk_in ? pcplus4_in : (load_in ? valM_in : valE_in);
	
	
endmodule
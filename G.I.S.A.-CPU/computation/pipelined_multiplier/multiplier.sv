module multiplier (clk, nRESET, a_in, b_in, signed_sig_in, mul_start_in, sumout_out, mul_finished_out);

	input clk, nRESET;
	input [31:0] a_in, b_in;
   input signed_sig_in, mul_start_in;   // 1: signed mul, 0: unsigned mul.
	
	(* keep *) output [63:0] sumout_out;
	(* keep *) output mul_finished_out;
	
	wire [33:0] pp_PG [15:0];
   wire [15:0] sign_PG;
	
	wire [33:0] pp_PA [15:0];
	wire [31:0] a_PA;
   wire [15:0] sign_PA;
	wire signed_sig_PA, b_PA;
	
	wire [63:0] tree_result_PA [1:0];
	wire [63:0] tree_result_FA [1:0];
		
	
	cal_checker cal_checker_module (clk, nRESET, mul_start_in, mul_finished_out);
	
	ppgen_level ppgen_level_module (a_in, b_in, signed_sig_in, pp_PG, sign_PG);
	pipereg_PGtoPA pipereg_PGtoPA_module (clk, nRESET, pp_PG, sign_PG, signed_sig_in, a_in, b_in[31], pp_PA, sign_PA, signed_sig_PA, a_PA, b_PA);
	
	ppacc_level ppacc_level_module (pp_PA, sign_PA, signed_sig_PA, a_PA, b_PA, tree_result_PA);
	pipereg_PAtoFA pipereg_PAtoFA_module (clk, nRESET, tree_result_PA, tree_result_FA);
	
	adder_64b cpa (tree_result_FA[0], tree_result_FA[1], 1'b0, , sumout_out);
	
endmodule
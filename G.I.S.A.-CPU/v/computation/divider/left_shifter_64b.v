(* keep_hierarchy *) module left_shifter_64b (valA, valB, result);

    input [63:0] valA;
	 input [5:0] valB;
    (* keep *) output [63:0] result;

    (* keep *) wire [63:0] stage_0, stage_1, stage_2, stage_3, stage_4, stage_5;
    
    genvar i;


    // stage_0
    assign stage_0[0] = valB[0] ? 1'b0 : valA[0];
    assign stage_0[63:1] = valB[0] ? valA[62:0] : valA[63:1];


    // stage_1
    assign stage_1[1:0] = valB[1] ? 2'b0 : stage_0[1:0];
    assign stage_1[63:2] = valB[1] ? stage_0[61:0] : stage_0[63:2];


    // stage_2
    assign stage_2[3:0] = valB[2] ? 4'b0 : stage_1[3:0];    
    assign stage_2[63:4] = valB[2] ? stage_1[59:0] : stage_1[63:4];


    // stage_3
    assign stage_3[7:0] = valB[3] ? 8'b0 : stage_2[7:0];
	 assign stage_3[63:8] = valB[3] ? stage_2[55:0] : stage_2[63:8];


    // stage_4
	 assign stage_4[15:0] = valB[4] ? 16'b0 : stage_3[15:0];
	 assign stage_4[63:16] = valB[4] ? stage_3[47:0] : stage_3[63:16];


    // stage_5
	 assign stage_5[31:0] = valB[5] ? 32'b0 : stage_4[31:0];
	 assign stage_5[63:32] = valB[5] ? stage_4[31:0] : stage_4[63:32];
	 


    assign result = stage_5;

endmodule



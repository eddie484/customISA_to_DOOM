module right_shifter_32b (valA, valB, signed_sig, result);

    input [31:0] valA;
	 input [4:0] valB;
    input signed_sig;
    output [31:0] result;

    (* keep *) wire [31:0] stage_0, stage_1, stage_2, stage_3, stage_4;
    
    genvar i;




    // stage_0
    assign stage_0[0] = valB[0] ? valA[1] : valA[0];

    generate
        for (i = 1; i < 31; i = i + 1) begin : stage_0_block
            assign stage_0[i] = valB[0] ? valA[i + 1] : valA[i];
        end
    endgenerate

    assign stage_0[31] = valB[0] ? (signed_sig ? valA[31] : 1'b0) : valA[31];


    // stage_1
    assign stage_1[0] = valB[1] ? stage_0[2] : stage_0[0];
    assign stage_1[1] = valB[1] ? stage_0[3] : stage_0[1];

    generate
        for (i = 2; i < 30; i = i + 1) begin : stage_1_block
            assign stage_1[i] = valB[1] ? stage_0[i + 2] : stage_0[i];
        end
    endgenerate
    
    assign stage_1[30] = valB[1] ? (signed_sig ? stage_0[31] : 1'b0) : stage_0[30];
    assign stage_1[31] = valB[1] ? (signed_sig ? stage_0[31] : 1'b0) : stage_0[31];


    // stage_2
    assign stage_2[0] = valB[2] ? stage_1[4] : stage_1[0];
    assign stage_2[1] = valB[2] ? stage_1[5] : stage_1[1];
    assign stage_2[2] = valB[2] ? stage_1[6] : stage_1[2];
    assign stage_2[3] = valB[2] ? stage_1[7] : stage_1[3];

    generate
        for (i = 4; i < 28; i = i + 1) begin : stage_2_block
            assign stage_2[i] = valB[2] ? stage_1[i + 4] : stage_1[i];
        end
    endgenerate
    
    assign stage_2[28] = valB[2] ? (signed_sig ? stage_1[31] : 1'b0) : stage_1[28];
    assign stage_2[29] = valB[2] ? (signed_sig ? stage_1[31] : 1'b0) : stage_1[29];
    assign stage_2[30] = valB[2] ? (signed_sig ? stage_1[31] : 1'b0) : stage_1[30];
    assign stage_2[31] = valB[2] ? (signed_sig ? stage_1[31] : 1'b0) : stage_1[31];


    // stage_3
    generate
        for (i = 0; i < 8; i = i + 1) begin : stage_3_start_block
            assign stage_3[i] = valB[3] ? stage_2[i + 8] : stage_2[i];
        end
        
        for (i = 8; i < 24; i = i + 1) begin : stage_3_middle_block
            assign stage_3[i] = valB[3] ? stage_2[i + 8] : stage_2[i];
        end

        for (i = 24; i < 32; i = i + 1) begin : stage_3_end_block
            assign stage_3[i] = valB[3] ? (signed_sig ? stage_2[31] : 1'b0) : stage_2[i];
        end
    endgenerate


    // stage_4
    generate
        for (i = 0; i < 16; i = i + 1) begin : stage_4_start_block
            assign stage_4[i] = valB[4] ? stage_3[i + 16] : stage_3[i];
        end

        for (i = 16; i < 32; i = i + 1) begin : stage_4_end_block
            assign stage_4[i] = valB[4] ? (signed_sig ? stage_3[31] : 1'b0) : stage_3[i];
        end
    endgenerate    


    assign result = stage_4;

	
endmodule
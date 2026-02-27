module shifter_rotator (valA, valB, sftmode, signed_sig, result, pushed);

    input [31:0] valA, valB;
    input [1:0] sftmode;
    input signed_sig;

    output [31:0] result;
    output pushed;

    wire shirot, lftrgt;
    wire [31:0] stage_0, stage_1, stage_2, stage_3, stage_4;
    
    genvar i;


    assign shirot = sftmode[0] ~^ sftmode[1];      // 0 = shift, 1 = rotate
    assign lftrgt = ~sftmode[0];     // 0 = left, 1 = right


    // stage_0
    assign stage_0[0] = valB[0] ? (lftrgt ? valA[1] : (shirot ? valA[31] : 1'b0)) : valA[0];

    generate
        for (i = 1; i < 31; i = i + 1) begin : stage_0_block
            assign stage_0[i] = valB[0] ? (lftrgt ? valA[i + 1] : valA[i - 1]) : valA[i];
        end
    endgenerate

    assign stage_0[31] = valB[0] ? (lftrgt ? (shirot ? valA[0] : (signed_sig ? valA[31] : 1'b0)) : valA[30]) : valA[31];


    // stage_1
    assign stage_1[0] = valB[1] ? (lftrgt ? stage_0[2] : (shirot ? stage_0[30] : 1'b0)) : stage_0[0];
    assign stage_1[1] = valB[1] ? (lftrgt ? stage_0[3] : (shirot ? stage_0[31] : 1'b0)) : stage_0[1];

    generate
        for (i = 2; i < 30; i = i + 1) begin : stage_1_block
            assign stage_1[i] = valB[1] ? (lftrgt ? stage_0[i + 2] : stage_0[i - 2]) : stage_0[i];
        end
    endgenerate
    
    assign stage_1[30] = valB[1] ? (lftrgt ? (shirot ? stage_0[0] : (signed_sig ? stage_0[31] : 1'b0)) : stage_0[28]) : stage_0[30];
    assign stage_1[31] = valB[1] ? (lftrgt ? (shirot ? stage_0[1] : (signed_sig ? stage_0[31] : 1'b0)) : stage_0[29]) : stage_0[31];


    // stage_2
    assign stage_2[0] = valB[2] ? (lftrgt ? stage_1[4] : (shirot ? stage_1[28] : 1'b0)) : stage_1[0];
    assign stage_2[1] = valB[2] ? (lftrgt ? stage_1[5] : (shirot ? stage_1[29] : 1'b0)) : stage_1[1];
    assign stage_2[2] = valB[2] ? (lftrgt ? stage_1[6] : (shirot ? stage_1[30] : 1'b0)) : stage_1[2];
    assign stage_2[3] = valB[2] ? (lftrgt ? stage_1[7] : (shirot ? stage_1[31] : 1'b0)) : stage_1[3];

    generate
        for (i = 4; i < 28; i = i + 1) begin : stage_2_block
            assign stage_2[i] = valB[2] ? (lftrgt ? stage_1[i + 4] : stage_1[i - 4]) : stage_1[i];
        end
    endgenerate
    
    assign stage_2[28] = valB[2] ? (lftrgt ? (shirot ? stage_1[0] : (signed_sig ? stage_1[31] : 1'b0)) : stage_1[24]) : stage_1[28];
    assign stage_2[29] = valB[2] ? (lftrgt ? (shirot ? stage_1[1] : (signed_sig ? stage_1[31] : 1'b0)) : stage_1[25]) : stage_1[29];
    assign stage_2[30] = valB[2] ? (lftrgt ? (shirot ? stage_1[2] : (signed_sig ? stage_1[31] : 1'b0)) : stage_1[26]) : stage_1[30];
    assign stage_2[31] = valB[2] ? (lftrgt ? (shirot ? stage_1[3] : (signed_sig ? stage_1[31] : 1'b0)) : stage_1[27]) : stage_1[31];


    // stage_3
    generate
        for (i = 0; i < 8; i = i + 1) begin : stage_3_start_block
            assign stage_3[i] = valB[3] ? (lftrgt ? stage_2[i + 8] : (shirot ? stage_2[24 + i] : 1'b0)) : stage_2[i];
        end
        
        for (i = 8; i < 24; i = i + 1) begin : stage_3_middle_block
            assign stage_3[i] = valB[3] ? (lftrgt ? stage_2[i + 8] : stage_2[i - 8]) : stage_2[i];
        end

        for (i = 24; i < 32; i = i + 1) begin : stage_3_end_block
            assign stage_3[i] = valB[3] ? (lftrgt ? (shirot ? stage_2[i - 24] : (signed_sig ? stage_2[31] : 1'b0)) : stage_2[i - 8]) : stage_2[i];
        end
    endgenerate


    // stage_4
    generate
        for (i = 0; i < 16; i = i + 1) begin : stage_4_start_block
            assign stage_4[i] = valB[4] ? (lftrgt ? stage_3[i + 16] : (shirot ? stage_3[16 + i] : 1'b0)) : stage_3[i];
        end

        for (i = 16; i < 32; i = i + 1) begin : stage_4_end_block
            assign stage_4[i] = valB[4] ? (lftrgt ? (shirot ? stage_3[i - 16] : (signed_sig ? stage_3[31] : 1'b0)) : stage_3[i - 16]) : stage_3[i];
        end
    endgenerate    


    assign result = stage_4;
    assign pushed = ~|valB ? 1'b0 : (lftrgt ? valA[valB - 1] : valA[32 - valB]);

endmodule



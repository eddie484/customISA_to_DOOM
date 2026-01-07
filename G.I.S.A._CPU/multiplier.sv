`define BOOTH_M2 3'd1
`define BOOTH_M1 3'd2
`define BOOTH_0 3'd3
`define BOOTH_P1 3'd4
`define BOOTH_P2 3'd5


module multiplier (a, b, sumout, cout);
	
	input [31:0] a, b;
	
	output cout;
	output [63:0] sumout;

    wire [2:0] booth_b [15:0];
	wire [32:0] pp [15:0];
    wire [15:0] sign;



    genvar i;

    generate
        booth_radix_4 booth_recoding_0 (b[1], b[0], 1'b0, booth_b[0]);
        for (i = 2; i < 32; i = i + 2) begin: booth_level
            booth_radix_4 booth_recoding (b[i+1], b[i], b[i-1], booth_b[i/2]);
        end

        for (i = 0; i < 16; i = i + 1) begin: ppgen_level
            ppgen ppgenerator (a, booth_b[i], pp[i], sign[i]);
        end
    endgenerate

    wire [63:0] tree_level_1 [16:0];
    wire [63:0] tree_level_2 [11:0];
    wire [63:0] tree_level_3 [7:0];
    wire [63:0] tree_level_4 [5:0];
    wire [63:0] tree_level_5 [4:0];
    wire [63:0] tree_level_6 [2:0];
    wire [63:0] tree_level_7 [1:0];

    assign tree_level_1 [0] = {~pp[15][32], 1'b1, ~pp[14][32], 1'b1, ~pp[13][32], 1'b1, ~pp[12][32], 1'b1, ~pp[11][32], 1'b1, ~pp[10][32], 1'b1, ~pp[9][32], 1'b1, ~pp[8][32], 1'b1, ~pp[7][32], 1'b1, ~pp[6][32], 1'b1, ~pp[5][32], 1'b1, ~pp[4][32], 1'b1, ~pp[3][32], 1'b1, ~pp[2][32], 1'b1, ~pp[0][32], pp[0][32], pp[0][32], pp[0]};
    assign tree_level_1 [1] = {1'b0, pp[15][32:31], pp[14][32:31], pp[13][32:31], pp[12][32:31], pp[11][32:31], pp[10][32:31], pp[9][32:31], pp[8][32:31], pp[7][32:31], pp[6][32:31], pp[5][32:31], pp[4][32:31], pp[3][32:31], pp[2][32], ~pp[1][32], pp[1], 1'b0, sign[0]};
    assign tree_level_1 [2] = {3'b0, pp[15][30:29], pp[14][30:29], pp[13][30:29], pp[12][30:29], pp[11][30:29], pp[10][30:29], pp[9][30:29], pp[8][30:29], pp[7][30:29], pp[6][30:29], pp[5][30:29], pp[4][30:29], pp[3][30], pp[2][31:0], 1'b0, sign[1], 2'b0};
    assign tree_level_1 [3] = {5'b0, pp[15][28:27], pp[14][28:27], pp[13][28:27], pp[12][28:27], pp[11][28:27], pp[10][28:27], pp[9][28:27], pp[8][28:27], pp[7][28:27], pp[6][28:27], pp[5][28:27], pp[4][28], pp[3][29:0], 1'b0, sign[2], 4'b0};
    assign tree_level_1 [4] = {7'b0, pp[15][26:25], pp[14][26:25], pp[13][26:25], pp[12][26:25], pp[11][26:25], pp[10][26:25], pp[9][26:25], pp[8][26:25], pp[7][26:25], pp[6][26:25], pp[5][26], pp[4][27:0], 1'b0, sign[3], 6'b0};
    assign tree_level_1 [5] = {9'b0, pp[15][24:23], pp[14][24:23], pp[13][24:23], pp[12][24:23], pp[11][24:23], pp[10][24:23], pp[9][24:23], pp[8][24:23], pp[7][24:23], pp[6][24], pp[5][25:0], 1'b0, sign[4], 8'b0};
    assign tree_level_1 [6] = {11'b0, pp[15][22:21], pp[14][22:21], pp[13][22:21], pp[12][22:21], pp[11][22:21], pp[10][22:21], pp[9][22:21], pp[8][22:21], pp[7][22], pp[6][23:0], 1'b0, sign[5], 10'b0};
    assign tree_level_1 [7] = {13'b0, pp[15][20:19], pp[14][20:19], pp[13][20:19], pp[12][20:19], pp[11][20:19], pp[10][20:19], pp[9][20:19], pp[8][20], pp[7][21:0], 1'b0, sign[6], 12'b0};
    assign tree_level_1 [8] = {15'b0, pp[15][18:17], pp[14][18:17], pp[13][18:17], pp[12][18:17], pp[11][18:17], pp[10][18:17], pp[9][18], pp[8][19:0], 1'b0, sign[7], 14'b0};
    assign tree_level_1 [9] = {17'b0, pp[15][16:15], pp[14][16:15], pp[13][16:15], pp[12][16:15], pp[11][16:15], pp[10][16], pp[9][17:0], 1'b0, sign[8], 16'b0};
    assign tree_level_1 [10] = {19'b0, pp[15][14:13], pp[14][14:13], pp[13][14:13], pp[12][14:13], pp[11][14], pp[10][15:0], 1'b0, sign[9], 18'b0};
    assign tree_level_1 [11] = {21'b0, pp[15][12:11], pp[14][12:11], pp[13][12:11], pp[12][12], pp[11][13:0], 1'b0, sign[10], 20'b0};
    assign tree_level_1 [12] = {23'b0, pp[15][10:9], pp[14][10:9], pp[13][10], pp[12][11:0], 1'b0, sign[11], 22'b0};
    assign tree_level_1 [13] = {25'b0, pp[15][8:7], pp[14][8], pp[13][9:0], 1'b0, sign[12], 24'b0};
    assign tree_level_1 [14] = {27'b0, pp[15][6], pp[14][7:0], 1'b0, sign[13], 26'b0};
    assign tree_level_1 [15] = {28'b0, pp[15][5:0], 1'b0, sign[14], 28'b0};
    assign tree_level_1 [16] = {33'b0, sign[15], 30'b0};

    csa_level_1 csa1 (tree_level_1, tree_level_2);
    csa_level_2 csa2 (tree_level_2, tree_level_3);
    csa_level_3 csa3 (tree_level_3, tree_level_4);
    csa_level_4 csa4 (tree_level_4, tree_level_5);
    csa_level_5 csa5 (tree_level_5, tree_level_6);
    csa_level_6 csa6 (tree_level_6, tree_level_7);

    adder_64b cpa (tree_level_7[0], tree_level_7[1], 1'b0, cout, sumout);



endmodule



module booth_radix_4 (x_high, x_low, x_prev, y);

    input x_high, x_low, x_prev;
    output reg [2:0] y;

    wire [2:0] x_in;
    assign x_in = {x_high, x_low, x_prev};

    always @(*) begin
        case (x_in)
            3'b000: y = `BOOTH_0;
            3'b010: y = `BOOTH_P1;
            3'b100: y = `BOOTH_M2;
            3'b110: y = `BOOTH_M1;
            3'b001: y = `BOOTH_P1;
            3'b011: y = `BOOTH_P2;
            3'b101: y = `BOOTH_M1;
            3'b111: y = `BOOTH_0;
            default: y = 3'd0;
        endcase
    end
endmodule



module ppgen (a, booth_b, pp, sign);

    input [31:0] a;
    input [2:0] booth_b;

    output reg [32:0] pp;
    output reg sign;

    always @(*) begin
        case (booth_b)
            `BOOTH_M2: begin
                pp = {~a, 1'b1};
                sign = 1'b1;
            end

            `BOOTH_M1: begin
                pp = {~a[31], ~a};
                sign = 1'b1;
            end

            `BOOTH_0: begin
                pp = 33'b0;
                sign = 1'b0;
            end

            `BOOTH_P1: begin
                pp = {a[31], a};
                sign = 1'b0;
            end
            
            `BOOTH_P2: begin
                pp = {a, 1'b0};
                sign = 1'b0;
            end

            default: begin
                pp = 33'b0;
                sign = 1'b0;
            end
        endcase
    end
endmodule


module csa_level_1 (tree_1, tree_2);

    input [63:0] tree_1 [16:0];
    output [63:0] tree_2 [11:0];

    assign tree_2[0][63] = tree_1[0][63];
    assign tree_2[0][58:57] = tree_1[0][58:57];
    assign tree_2[0][52:51] = tree_1[0][52:51];
    assign tree_2[0][46:45] = tree_1[0][46:45];
    assign tree_2[0][40:39] = tree_1[0][40:39];
    assign tree_2[0][35:31] = tree_1[0][35:31];
    assign tree_2[0][28] = tree_1[0][28];
    assign tree_2[0][25] = tree_1[0][25];
    assign tree_2[0][22] = tree_1[0][22];
    assign tree_2[0][19] = tree_1[0][19];
    assign tree_2[0][16] = tree_1[0][16];
    assign tree_2[0][13] = tree_1[0][13];
    assign tree_2[0][10] = tree_1[0][10];
    assign tree_2[0][7] = tree_1[0][7];
    assign tree_2[0][4] = tree_1[0][4];
    assign tree_2[0][1:0] = tree_1[0][1:0];
    assign tree_2[1][0] = tree_1[1][0];

    full_adder adder_0 (tree_1[0][2], tree_1[1][2], tree_1[2][2], tree_2[1][3], tree_2[0][2]);
    half_adder adder_1 (tree_1[0][3], tree_1[1][3], tree_2[2][4], tree_2[0][3]);
    full_adder adder_2 (tree_1[1][4], tree_1[2][4], tree_1[3][4], tree_2[1][5], tree_2[1][4]);
    full_adder adder_3 (tree_1[0][5], tree_1[1][5], tree_1[2][5], tree_2[2][6], tree_2[0][5]);
    half_adder adder_4 (tree_1[0][6], tree_1[1][6], tree_2[2][7], tree_2[0][6]);
    full_adder adder_5 (tree_1[2][6], tree_1[3][6], tree_1[4][6], tree_2[3][7], tree_2[1][6]);
    full_adder adder_6 (tree_1[1][7], tree_1[2][7], tree_1[3][7], tree_2[2][8], tree_2[1][7]);

    full_adder adder_7 (tree_1[0][8], tree_1[1][8], tree_1[2][8], tree_2[2][9], tree_2[0][8]);
    full_adder adder_8 (tree_1[3][8], tree_1[4][8], tree_1[5][8], tree_2[3][9], tree_2[1][8]);

    half_adder adder_9 (tree_1[0][9], tree_1[1][9], tree_2[3][10], tree_2[0][9]);
    full_adder adder_10 (tree_1[2][9], tree_1[3][9], tree_1[4][9], tree_2[4][10], tree_2[1][9]);

    full_adder adder_11 (tree_1[1][10], tree_1[2][10], tree_1[3][10], tree_2[2][11], tree_2[1][10]);
    full_adder adder_12 (tree_1[4][10], tree_1[5][10], tree_1[6][10], tree_2[3][11], tree_2[2][10]);
    
    full_adder adder_13 (tree_1[0][11], tree_1[1][11], tree_1[2][11], tree_2[3][12], tree_2[0][11]);
    full_adder adder_14 (tree_1[3][11], tree_1[4][11], tree_1[5][11], tree_2[4][12], tree_2[1][11]);
    
    half_adder adder_15 (tree_1[0][12], tree_1[1][12], tree_2[3][13], tree_2[0][12]);
    full_adder adder_16 (tree_1[2][12], tree_1[3][12], tree_1[4][12], tree_2[4][13], tree_2[1][12]);
    full_adder adder_17 (tree_1[5][12], tree_1[6][12], tree_1[7][12], tree_2[5][13], tree_2[2][12]);

    full_adder adder_18 (tree_1[1][13], tree_1[2][13], tree_1[3][13], tree_2[3][14], tree_2[1][13]);
    full_adder adder_19 (tree_1[4][13], tree_1[5][13], tree_1[6][13], tree_2[4][14], tree_2[2][13]);

    full_adder adder_20 (tree_1[0][14], tree_1[1][14], tree_1[2][14], tree_2[3][15], tree_2[0][14]);
    full_adder adder_21 (tree_1[3][14], tree_1[4][14], tree_1[5][14], tree_2[4][15], tree_2[1][14]);
    full_adder adder_22 (tree_1[6][14], tree_1[7][14], tree_1[8][14], tree_2[5][15], tree_2[2][14]);
    
    half_adder adder_23 (tree_1[0][15], tree_1[1][15], tree_2[4][16], tree_2[0][15]);
    full_adder adder_24 (tree_1[2][15], tree_1[3][15], tree_1[4][15], tree_2[5][16], tree_2[1][15]);
    full_adder adder_25 (tree_1[5][15], tree_1[6][15], tree_1[7][15], tree_2[6][16], tree_2[2][15]);

    full_adder adder_26 (tree_1[1][16], tree_1[2][16], tree_1[3][16], tree_2[3][17], tree_2[1][16]);
    full_adder adder_27 (tree_1[4][16], tree_1[5][16], tree_1[6][16], tree_2[4][17], tree_2[2][16]);
    full_adder adder_28 (tree_1[7][16], tree_1[8][16], tree_1[9][16], tree_2[5][17], tree_2[3][16]);

    full_adder adder_29 (tree_1[0][17], tree_1[1][17], tree_1[2][17], tree_2[4][18], tree_2[0][17]);
    full_adder adder_30 (tree_1[3][17], tree_1[4][17], tree_1[5][17], tree_2[5][18], tree_2[1][17]);
    full_adder adder_31 (tree_1[6][17], tree_1[7][17], tree_1[8][17], tree_2[6][18], tree_2[2][17]);

    half_adder adder_32 (tree_1[0][18], tree_1[1][18], tree_2[4][19], tree_2[0][18]);
    full_adder adder_33 (tree_1[2][18], tree_1[3][18], tree_1[4][18], tree_2[5][19], tree_2[1][18]);
    full_adder adder_34 (tree_1[5][18], tree_1[6][18], tree_1[7][18], tree_2[6][19], tree_2[2][18]);
    full_adder adder_35 (tree_1[8][18], tree_1[9][18], tree_1[10][18], tree_2[7][19], tree_2[3][18]);

    full_adder adder_36 (tree_1[1][19], tree_1[2][19], tree_1[3][19], tree_2[4][20], tree_2[1][19]);
    full_adder adder_37 (tree_1[4][19], tree_1[5][19], tree_1[6][19], tree_2[5][20], tree_2[2][19]);
    full_adder adder_38 (tree_1[7][19], tree_1[8][19], tree_1[9][19], tree_2[6][20], tree_2[3][19]);

    full_adder adder_39 (tree_1[0][20], tree_1[1][20], tree_1[2][20], tree_2[4][21], tree_2[0][20]);
    full_adder adder_40 (tree_1[3][20], tree_1[4][20], tree_1[5][20], tree_2[5][21], tree_2[1][20]);
    full_adder adder_41 (tree_1[6][20], tree_1[7][20], tree_1[8][20], tree_2[6][21], tree_2[2][20]);
    full_adder adder_42 (tree_1[9][20], tree_1[10][20], tree_1[11][20], tree_2[7][21], tree_2[3][20]);
    
    half_adder adder_43 (tree_1[0][21], tree_1[1][21], tree_2[5][22], tree_2[0][21]);
    full_adder adder_44 (tree_1[2][21], tree_1[3][21], tree_1[4][21], tree_2[6][22], tree_2[1][21]);
    full_adder adder_45 (tree_1[5][21], tree_1[6][21], tree_1[7][21], tree_2[7][22], tree_2[2][21]);
    full_adder adder_46 (tree_1[8][21], tree_1[9][21], tree_1[10][21], tree_2[8][22], tree_2[3][21]);

    full_adder adder_47 (tree_1[1][22], tree_1[2][22], tree_1[3][22], tree_2[4][23], tree_2[1][22]);
    full_adder adder_48 (tree_1[4][22], tree_1[5][22], tree_1[6][22], tree_2[5][23], tree_2[2][22]);
    full_adder adder_49 (tree_1[7][22], tree_1[8][22], tree_1[9][22], tree_2[6][23], tree_2[3][22]);
    full_adder adder_50 (tree_1[10][22], tree_1[11][22], tree_1[12][22], tree_2[7][23], tree_2[4][22]);

    full_adder adder_51 (tree_1[0][23], tree_1[1][23], tree_1[2][23], tree_2[5][24], tree_2[0][23]);
    full_adder adder_52 (tree_1[3][23], tree_1[4][23], tree_1[5][23], tree_2[6][24], tree_2[1][23]);
    full_adder adder_53 (tree_1[6][23], tree_1[7][23], tree_1[8][23], tree_2[7][24], tree_2[2][23]);
    full_adder adder_54 (tree_1[9][23], tree_1[10][23], tree_1[11][23], tree_2[8][24], tree_2[3][23]);
    
    half_adder adder_55 (tree_1[0][24], tree_1[1][24], tree_2[5][25], tree_2[0][24]);
    full_adder adder_56 (tree_1[2][24], tree_1[3][24], tree_1[4][24], tree_2[6][25], tree_2[1][24]);
    full_adder adder_57 (tree_1[5][24], tree_1[6][24], tree_1[7][24], tree_2[7][25], tree_2[2][24]);
    full_adder adder_58 (tree_1[8][24], tree_1[9][24], tree_1[10][24], tree_2[8][25], tree_2[3][24]);
    full_adder adder_59 (tree_1[11][24], tree_1[12][24], tree_1[13][24], tree_2[9][25], tree_2[4][24]);

    full_adder adder_60 (tree_1[1][25], tree_1[2][25], tree_1[3][25], tree_2[5][26], tree_2[1][25]);
    full_adder adder_61 (tree_1[4][25], tree_1[5][25], tree_1[6][25], tree_2[6][26], tree_2[2][25]);
    full_adder adder_62 (tree_1[7][25], tree_1[8][25], tree_1[9][25], tree_2[7][26], tree_2[3][25]);
    full_adder adder_63 (tree_1[10][25], tree_1[11][25], tree_1[12][25], tree_2[8][26], tree_2[4][25]);

    full_adder adder_64 (tree_1[0][26], tree_1[1][26], tree_1[2][26], tree_2[5][27], tree_2[0][26]);
    full_adder adder_65 (tree_1[3][26], tree_1[4][26], tree_1[5][26], tree_2[6][27], tree_2[1][26]);
    full_adder adder_66 (tree_1[6][26], tree_1[7][26], tree_1[8][26], tree_2[7][27], tree_2[2][26]);
    full_adder adder_67 (tree_1[9][26], tree_1[10][26], tree_1[11][26], tree_2[8][27], tree_2[3][26]);
    full_adder adder_68 (tree_1[12][26], tree_1[13][26], tree_1[14][26], tree_2[9][27], tree_2[4][26]);
    
    half_adder adder_69 (tree_1[0][27], tree_1[1][27], tree_2[6][28], tree_2[0][27]);
    full_adder adder_70 (tree_1[2][27], tree_1[3][27], tree_1[4][27], tree_2[7][28], tree_2[1][27]);
    full_adder adder_71 (tree_1[5][27], tree_1[6][27], tree_1[7][27], tree_2[8][28], tree_2[2][27]);
    full_adder adder_72 (tree_1[8][27], tree_1[9][27], tree_1[10][27], tree_2[9][28], tree_2[3][27]);
    full_adder adder_73 (tree_1[11][27], tree_1[12][27], tree_1[13][27], tree_2[10][28], tree_2[4][27]);

    full_adder adder_74 (tree_1[1][28], tree_1[2][28], tree_1[3][28], tree_2[5][29], tree_2[1][28]);
    full_adder adder_75 (tree_1[4][28], tree_1[5][28], tree_1[6][28], tree_2[6][29], tree_2[2][28]);
    full_adder adder_76 (tree_1[7][28], tree_1[8][28], tree_1[9][28], tree_2[7][29], tree_2[3][28]);
    full_adder adder_77 (tree_1[10][28], tree_1[11][28], tree_1[12][28], tree_2[8][29], tree_2[4][28]);
    full_adder adder_78 (tree_1[13][28], tree_1[14][28], tree_1[15][28], tree_2[9][29], tree_2[5][28]);

    full_adder adder_79 (tree_1[0][29], tree_1[1][29], tree_1[2][29], tree_2[6][30], tree_2[0][29]);
    full_adder adder_80 (tree_1[3][29], tree_1[4][29], tree_1[5][29], tree_2[7][30], tree_2[1][29]);
    full_adder adder_81 (tree_1[6][29], tree_1[7][29], tree_1[8][29], tree_2[8][30], tree_2[2][29]);
    full_adder adder_82 (tree_1[9][29], tree_1[10][29], tree_1[11][29], tree_2[9][30], tree_2[3][29]);
    full_adder adder_83 (tree_1[12][29], tree_1[13][29], tree_1[14][29], tree_2[10][30], tree_2[4][29]);
    
    half_adder adder_84 (tree_1[0][30], tree_1[1][30], tree_2[6][31], tree_2[0][30]);
    full_adder adder_85 (tree_1[2][30], tree_1[3][30], tree_1[4][30], tree_2[7][31], tree_2[1][30]);
    full_adder adder_86 (tree_1[5][30], tree_1[6][30], tree_1[7][30], tree_2[8][31], tree_2[2][30]);
    full_adder adder_87 (tree_1[8][30], tree_1[9][30], tree_1[10][30], tree_2[9][31], tree_2[3][30]);
    full_adder adder_88 (tree_1[11][30], tree_1[12][30], tree_1[13][30], tree_2[10][31], tree_2[4][30]);
    full_adder adder_89 (tree_1[14][30], tree_1[15][30], tree_1[16][30], tree_2[11][31], tree_2[5][30]);

    full_adder adder_90 (tree_1[1][31], tree_1[2][31], tree_1[3][31], tree_2[6][32], tree_2[1][31]);
    full_adder adder_91 (tree_1[4][31], tree_1[5][31], tree_1[6][31], tree_2[7][32], tree_2[2][31]);
    full_adder adder_92 (tree_1[7][31], tree_1[8][31], tree_1[9][31], tree_2[8][32], tree_2[3][31]);
    full_adder adder_93 (tree_1[10][31], tree_1[11][31], tree_1[12][31], tree_2[9][32], tree_2[4][31]);
    full_adder adder_94 (tree_1[13][31], tree_1[14][31], tree_1[15][31], tree_2[10][32], tree_2[5][31]);

    full_adder adder_95 (tree_1[1][32], tree_1[2][32], tree_1[3][32], tree_2[6][33], tree_2[1][32]);
    full_adder adder_96 (tree_1[4][32], tree_1[5][32], tree_1[6][32], tree_2[7][33], tree_2[2][32]);
    full_adder adder_97 (tree_1[7][32], tree_1[8][32], tree_1[9][32], tree_2[8][33], tree_2[3][32]);
    full_adder adder_98 (tree_1[10][32], tree_1[11][32], tree_1[12][32], tree_2[9][33], tree_2[4][32]);
    full_adder adder_99 (tree_1[13][32], tree_1[14][32], tree_1[15][32], tree_2[10][33], tree_2[5][32]);

    full_adder adder_100 (tree_1[1][33], tree_1[2][33], tree_1[3][33], tree_2[6][34], tree_2[1][33]);
    full_adder adder_101 (tree_1[4][33], tree_1[5][33], tree_1[6][33], tree_2[7][34], tree_2[2][33]);
    full_adder adder_102 (tree_1[7][33], tree_1[8][33], tree_1[9][33], tree_2[8][34], tree_2[3][33]);
    full_adder adder_103 (tree_1[10][33], tree_1[11][33], tree_1[12][33], tree_2[9][34], tree_2[4][33]);
    full_adder adder_104 (tree_1[13][33], tree_1[14][33], tree_1[15][33], tree_2[10][34], tree_2[5][33]);

    full_adder adder_105 (tree_1[1][34], tree_1[2][34], tree_1[3][34], tree_2[6][35], tree_2[1][34]);
    full_adder adder_106 (tree_1[4][34], tree_1[5][34], tree_1[6][34], tree_2[7][35], tree_2[2][34]);
    full_adder adder_107 (tree_1[7][34], tree_1[8][34], tree_1[9][34], tree_2[8][35], tree_2[3][34]);
    full_adder adder_108 (tree_1[10][34], tree_1[11][34], tree_1[12][34], tree_2[9][35], tree_2[4][34]);
    full_adder adder_109 (tree_1[13][34], tree_1[14][34], tree_1[15][34], tree_2[10][35], tree_2[5][34]);

    full_adder adder_110 (tree_1[1][35], tree_1[2][35], tree_1[3][35], tree_2[5][36], tree_2[1][35]);
    full_adder adder_111 (tree_1[4][35], tree_1[5][35], tree_1[6][35], tree_2[6][36], tree_2[2][35]);
    full_adder adder_112 (tree_1[7][35], tree_1[8][35], tree_1[9][35], tree_2[7][36], tree_2[3][35]);
    full_adder adder_113 (tree_1[10][35], tree_1[11][35], tree_1[12][35], tree_2[8][36], tree_2[4][35]);
    full_adder adder_114 (tree_1[13][35], tree_1[14][35], tree_1[15][35], tree_2[9][36], tree_2[5][35]);

    full_adder adder_115 (tree_1[0][36], tree_1[1][36], tree_1[2][36], tree_2[5][37], tree_2[0][36]);
    full_adder adder_116 (tree_1[3][36], tree_1[4][36], tree_1[5][36], tree_2[6][37], tree_2[1][36]);
    full_adder adder_117 (tree_1[6][36], tree_1[7][36], tree_1[8][36], tree_2[7][37], tree_2[2][36]);
    full_adder adder_118 (tree_1[9][36], tree_1[10][36], tree_1[11][36], tree_2[8][37], tree_2[3][36]);
    full_adder adder_119 (tree_1[12][36], tree_1[13][36], tree_1[14][36], tree_2[9][37], tree_2[4][36]);
    
    half_adder adder_120 (tree_1[0][37], tree_1[1][37], tree_2[5][38], tree_2[0][37]);
    full_adder adder_121 (tree_1[2][37], tree_1[3][37], tree_1[4][37], tree_2[6][38], tree_2[1][37]);
    full_adder adder_122 (tree_1[5][37], tree_1[6][37], tree_1[7][37], tree_2[7][38], tree_2[2][37]);
    full_adder adder_123 (tree_1[8][37], tree_1[9][37], tree_1[10][37], tree_2[8][38], tree_2[3][37]);
    full_adder adder_124 (tree_1[11][37], tree_1[12][37], tree_1[13][37], tree_2[9][38], tree_2[4][37]);
    
    half_adder adder_125 (tree_1[0][38], tree_1[1][38], tree_2[5][39], tree_2[0][38]);
    full_adder adder_126 (tree_1[2][38], tree_1[3][38], tree_1[4][38], tree_2[6][39], tree_2[1][38]);
    full_adder adder_127 (tree_1[5][38], tree_1[6][38], tree_1[7][38], tree_2[7][39], tree_2[2][38]);
    full_adder adder_128 (tree_1[8][38], tree_1[9][38], tree_1[10][38], tree_2[8][39], tree_2[3][38]);
    full_adder adder_129 (tree_1[11][38], tree_1[12][38], tree_1[13][38], tree_2[9][39], tree_2[4][38]);

    full_adder adder_130 (tree_1[1][39], tree_1[2][39], tree_1[3][39], tree_2[5][40], tree_2[1][39]);
    full_adder adder_131 (tree_1[4][39], tree_1[5][39], tree_1[6][39], tree_2[6][40], tree_2[2][39]);
    full_adder adder_132 (tree_1[7][39], tree_1[8][39], tree_1[9][39], tree_2[7][40], tree_2[3][39]);
    full_adder adder_133 (tree_1[10][39], tree_1[11][39], tree_1[12][39], tree_2[8][40], tree_2[4][39]);

    full_adder adder_134 (tree_1[1][40], tree_1[2][40], tree_1[3][40], tree_2[4][41], tree_2[1][40]);
    full_adder adder_135 (tree_1[4][40], tree_1[5][40], tree_1[6][40], tree_2[5][41], tree_2[2][40]);
    full_adder adder_136 (tree_1[7][40], tree_1[8][40], tree_1[9][40], tree_2[6][41], tree_2[3][40]);
    full_adder adder_137 (tree_1[10][40], tree_1[11][40], tree_1[12][40], tree_2[7][41], tree_2[4][40]);

    full_adder adder_138 (tree_1[0][41], tree_1[1][41], tree_1[2][41], tree_2[4][42], tree_2[0][41]);
    full_adder adder_139 (tree_1[3][41], tree_1[4][41], tree_1[5][41], tree_2[5][42], tree_2[1][41]);
    full_adder adder_140 (tree_1[6][41], tree_1[7][41], tree_1[8][41], tree_2[6][42], tree_2[2][41]);
    full_adder adder_141 (tree_1[9][41], tree_1[10][41], tree_1[11][41], tree_2[7][42], tree_2[3][41]);

    full_adder adder_142 (tree_1[0][42], tree_1[1][42], tree_1[2][42], tree_2[4][43], tree_2[0][42]);
    full_adder adder_143 (tree_1[3][42], tree_1[4][42], tree_1[5][42], tree_2[5][43], tree_2[1][42]);
    full_adder adder_144 (tree_1[6][42], tree_1[7][42], tree_1[8][42], tree_2[6][43], tree_2[2][42]);
    full_adder adder_145 (tree_1[9][42], tree_1[10][42], tree_1[11][42], tree_2[7][43], tree_2[3][42]);
    
    half_adder adder_146 (tree_1[0][43], tree_1[1][43], tree_2[4][44], tree_2[0][43]);
    full_adder adder_147 (tree_1[2][43], tree_1[3][43], tree_1[4][43], tree_2[5][44], tree_2[1][43]);
    full_adder adder_148 (tree_1[5][43], tree_1[6][43], tree_1[7][43], tree_2[6][44], tree_2[2][43]);
    full_adder adder_149 (tree_1[8][43], tree_1[9][43], tree_1[10][43], tree_2[7][44], tree_2[3][43]);
    
    half_adder adder_150 (tree_1[0][44], tree_1[1][44], tree_2[4][45], tree_2[0][44]);
    full_adder adder_151 (tree_1[2][44], tree_1[3][44], tree_1[4][44], tree_2[5][45], tree_2[1][44]);
    full_adder adder_152 (tree_1[5][44], tree_1[6][44], tree_1[7][44], tree_2[6][45], tree_2[2][44]);
    full_adder adder_153 (tree_1[8][44], tree_1[9][44], tree_1[10][44], tree_2[7][45], tree_2[3][44]);

    full_adder adder_154 (tree_1[1][45], tree_1[2][45], tree_1[3][45], tree_2[4][46], tree_2[1][45]);
    full_adder adder_155 (tree_1[4][45], tree_1[5][45], tree_1[6][45], tree_2[5][46], tree_2[2][45]);
    full_adder adder_156 (tree_1[7][45], tree_1[8][45], tree_1[9][45], tree_2[6][46], tree_2[3][45]);

    full_adder adder_157 (tree_1[1][46], tree_1[2][46], tree_1[3][46], tree_2[3][47], tree_2[1][46]);
    full_adder adder_158 (tree_1[4][46], tree_1[5][46], tree_1[6][46], tree_2[4][47], tree_2[2][46]);
    full_adder adder_159 (tree_1[7][46], tree_1[8][46], tree_1[9][46], tree_2[5][47], tree_2[3][46]);

    full_adder adder_160 (tree_1[0][47], tree_1[1][47], tree_1[2][47], tree_2[3][48], tree_2[0][47]);
    full_adder adder_161 (tree_1[3][47], tree_1[4][47], tree_1[5][47], tree_2[4][48], tree_2[1][47]);
    full_adder adder_162 (tree_1[6][47], tree_1[7][47], tree_1[8][47], tree_2[5][48], tree_2[2][47]);

    full_adder adder_163 (tree_1[0][48], tree_1[1][48], tree_1[2][48], tree_2[3][49], tree_2[0][48]);
    full_adder adder_164 (tree_1[3][48], tree_1[4][48], tree_1[5][48], tree_2[4][49], tree_2[1][48]);
    full_adder adder_165 (tree_1[6][48], tree_1[7][48], tree_1[8][48], tree_2[5][49], tree_2[2][48]);
    
    half_adder adder_166 (tree_1[0][49], tree_1[1][49], tree_2[3][50], tree_2[0][49]);
    full_adder adder_167 (tree_1[2][49], tree_1[3][49], tree_1[4][49], tree_2[4][50], tree_2[1][49]);
    full_adder adder_168 (tree_1[5][49], tree_1[6][49], tree_1[7][49], tree_2[5][50], tree_2[2][49]);
    
    half_adder adder_169 (tree_1[0][50], tree_1[1][50], tree_2[3][51], tree_2[0][50]);
    full_adder adder_170 (tree_1[2][50], tree_1[3][50], tree_1[4][50], tree_2[4][51], tree_2[1][50]);
    full_adder adder_171 (tree_1[5][50], tree_1[6][50], tree_1[7][50], tree_2[5][51], tree_2[2][50]);

    full_adder adder_172 (tree_1[1][51], tree_1[2][51], tree_1[3][51], tree_2[3][52], tree_2[1][51]);
    full_adder adder_173 (tree_1[4][51], tree_1[5][51], tree_1[6][51], tree_2[4][52], tree_2[2][51]);

    full_adder adder_174 (tree_1[1][52], tree_1[2][52], tree_1[3][52], tree_2[2][53], tree_2[1][52]);
    full_adder adder_175 (tree_1[4][52], tree_1[5][52], tree_1[6][52], tree_2[3][53], tree_2[2][52]);

    full_adder adder_176 (tree_1[0][53], tree_1[1][53], tree_1[2][53], tree_2[2][54], tree_2[0][53]);
    full_adder adder_177 (tree_1[3][53], tree_1[4][53], tree_1[5][53], tree_2[3][54], tree_2[1][53]);

    full_adder adder_178 (tree_1[0][54], tree_1[1][54], tree_1[2][54], tree_2[2][55], tree_2[0][54]);
    full_adder adder_179 (tree_1[3][54], tree_1[4][54], tree_1[5][54], tree_2[3][55], tree_2[1][54]);
    
    half_adder adder_180 (tree_1[0][55], tree_1[1][55], tree_2[2][56], tree_2[0][55]);
    full_adder adder_181 (tree_1[2][55], tree_1[3][55], tree_1[4][55], tree_2[3][56], tree_2[1][55]);
    
    half_adder adder_182 (tree_1[0][56], tree_1[1][56], tree_2[2][57], tree_2[0][56]);
    full_adder adder_183 (tree_1[2][56], tree_1[3][56], tree_1[4][56], tree_2[3][57], tree_2[1][56]);

    full_adder adder_184 (tree_1[1][57], tree_1[2][57], tree_1[3][57], tree_2[2][58], tree_2[1][57]);
    full_adder adder_185 (tree_1[1][58], tree_1[2][58], tree_1[3][58], tree_2[1][59], tree_2[1][58]);
    full_adder adder_186 (tree_1[0][59], tree_1[1][59], tree_1[2][59], tree_2[1][60], tree_2[0][59]);
    full_adder adder_187 (tree_1[0][60], tree_1[1][60], tree_1[2][60], tree_2[1][61], tree_2[0][60]);
    half_adder adder_188 (tree_1[0][61], tree_1[1][61], tree_2[1][62], tree_2[0][61]);
    half_adder adder_189 (tree_1[0][62], tree_1[1][62], tree_2[1][63], tree_2[0][62]);

endmodule


module csa_level_2 (tree_2, tree_3);

    input [63:0] tree_2 [11:0];
    output [63:0] tree_3 [7:0];

    assign tree_3[0][57:53] = tree_2[0][57:53];
    assign tree_3[0][46] = tree_2[0][46];
    assign tree_3[0][39:36] = tree_2[0][39:36];
    assign tree_3[0][29] = tree_2[0][29];
    assign tree_3[0][27] = tree_2[0][27];
    assign tree_3[0][25] = tree_2[0][25];
    assign tree_3[0][20] = tree_2[0][20];
    assign tree_3[0][18] = tree_2[0][18];
    assign tree_3[0][16] = tree_2[0][16];
    assign tree_3[0][11] = tree_2[0][11];
    assign tree_3[0][9] = tree_2[0][9];
    assign tree_3[0][7] = tree_2[0][7];
    assign tree_3[0][3:0] = tree_2[0][3:0];
    assign tree_3[1][3] = tree_2[1][3];
    assign tree_3[1][0] = tree_2[1][0];

    full_adder adder_0 (tree_2[0][4], tree_2[1][4], tree_2[2][4], tree_3[1][5], tree_3[0][4]);
    half_adder adder_1 (tree_2[0][5], tree_2[1][5], tree_3[1][6], tree_3[0][5]);
    full_adder adder_2 (tree_2[0][6], tree_2[1][6], tree_2[2][6], tree_3[2][7], tree_3[0][6]);
    full_adder adder_3 (tree_2[1][7], tree_2[2][7], tree_2[3][7], tree_3[1][8], tree_3[1][7]);
    full_adder adder_4 (tree_2[0][8], tree_2[1][8], tree_2[2][8], tree_3[2][9], tree_3[0][8]);
    full_adder adder_5 (tree_2[1][9], tree_2[2][9], tree_2[3][9], tree_3[2][10], tree_3[1][9]);

    half_adder adder_6 (tree_2[0][10], tree_2[1][10], tree_3[2][11], tree_3[0][10]);
    full_adder adder_7 (tree_2[2][10], tree_2[3][10], tree_2[4][10], tree_3[3][11], tree_3[1][10]);

    full_adder adder_8 (tree_2[1][11], tree_2[2][11], tree_2[3][11], tree_3[2][12], tree_3[1][11]);
    
    half_adder adder_9 (tree_2[0][12], tree_2[1][12], tree_3[2][13], tree_3[0][12]);
    full_adder adder_10 (tree_2[2][12], tree_2[3][12], tree_2[4][12], tree_3[3][13], tree_3[1][12]);
    
    full_adder adder_11 (tree_2[0][13], tree_2[1][13], tree_2[2][13], tree_3[2][14], tree_3[0][13]);
    full_adder adder_12 (tree_2[3][13], tree_2[4][13], tree_2[5][13], tree_3[3][14], tree_3[1][13]);
    
    half_adder adder_13 (tree_2[0][14], tree_2[1][14], tree_3[2][15], tree_3[0][14]);
    full_adder adder_14 (tree_2[2][14], tree_2[3][14], tree_2[4][14], tree_3[3][15], tree_3[1][14]);
    
    full_adder adder_15 (tree_2[0][15], tree_2[1][15], tree_2[2][15], tree_3[3][16], tree_3[0][15]);
    full_adder adder_16 (tree_2[3][15], tree_2[4][15], tree_2[5][15], tree_3[4][16], tree_3[1][15]);

    full_adder adder_17 (tree_2[1][16], tree_2[2][16], tree_2[3][16], tree_3[2][17], tree_3[1][16]);
    full_adder adder_18 (tree_2[4][16], tree_2[5][16], tree_2[6][16], tree_3[3][17], tree_3[2][16]);
    
    full_adder adder_19 (tree_2[0][17], tree_2[1][17], tree_2[2][17], tree_3[3][18], tree_3[0][17]);
    full_adder adder_20 (tree_2[3][17], tree_2[4][17], tree_2[5][17], tree_3[4][18], tree_3[1][17]);

    full_adder adder_21 (tree_2[1][18], tree_2[2][18], tree_2[3][18], tree_3[3][19], tree_3[1][18]);
    full_adder adder_22 (tree_2[4][18], tree_2[5][18], tree_2[6][18], tree_3[4][19], tree_3[2][18]);
    
    half_adder adder_23 (tree_2[0][19], tree_2[1][19], tree_3[3][20], tree_3[0][19]);
    full_adder adder_24 (tree_2[2][19], tree_2[3][19], tree_2[4][19], tree_3[4][20], tree_3[1][19]);
    full_adder adder_25 (tree_2[5][19], tree_2[6][19], tree_2[7][19], tree_3[5][20], tree_3[2][19]);

    full_adder adder_26 (tree_2[1][20], tree_2[2][20], tree_2[3][20], tree_3[3][21], tree_3[1][20]);
    full_adder adder_27 (tree_2[4][20], tree_2[5][20], tree_2[6][20], tree_3[4][21], tree_3[2][20]);
    
    half_adder adder_28 (tree_2[0][21], tree_2[1][21], tree_3[3][22], tree_3[0][21]);
    full_adder adder_29 (tree_2[2][21], tree_2[3][21], tree_2[4][21], tree_3[4][22], tree_3[1][21]);
    full_adder adder_30 (tree_2[5][21], tree_2[6][21], tree_2[7][21], tree_3[5][22], tree_3[2][21]);
    
    full_adder adder_31 (tree_2[0][22], tree_2[1][22], tree_2[2][22], tree_3[3][23], tree_3[0][22]);
    full_adder adder_32 (tree_2[3][22], tree_2[4][22], tree_2[5][22], tree_3[4][23], tree_3[1][22]);
    full_adder adder_33 (tree_2[6][22], tree_2[7][22], tree_2[8][22], tree_3[5][23], tree_3[2][22]);
    
    half_adder adder_34 (tree_2[0][23], tree_2[1][23], tree_3[3][24], tree_3[0][23]);
    full_adder adder_35 (tree_2[2][23], tree_2[3][23], tree_2[4][23], tree_3[4][24], tree_3[1][23]);
    full_adder adder_36 (tree_2[5][23], tree_2[6][23], tree_2[7][23], tree_3[5][24], tree_3[2][23]);
    
    full_adder adder_37 (tree_2[0][24], tree_2[1][24], tree_2[2][24], tree_3[4][25], tree_3[0][24]);
    full_adder adder_38 (tree_2[3][24], tree_2[4][24], tree_2[5][24], tree_3[5][25], tree_3[1][24]);
    full_adder adder_39 (tree_2[6][24], tree_2[7][24], tree_2[8][24], tree_3[6][25], tree_3[2][24]);

    full_adder adder_40 (tree_2[1][25], tree_2[2][25], tree_2[3][25], tree_3[3][26], tree_3[1][25]);
    full_adder adder_41 (tree_2[4][25], tree_2[5][25], tree_2[6][25], tree_3[4][26], tree_3[2][25]);
    full_adder adder_42 (tree_2[7][25], tree_2[8][25], tree_2[9][25], tree_3[5][26], tree_3[3][25]);
    
    full_adder adder_43 (tree_2[0][26], tree_2[1][26], tree_2[2][26], tree_3[4][27], tree_3[0][26]);
    full_adder adder_44 (tree_2[3][26], tree_2[4][26], tree_2[5][26], tree_3[5][27], tree_3[1][26]);
    full_adder adder_45 (tree_2[6][26], tree_2[7][26], tree_2[8][26], tree_3[6][27], tree_3[2][26]);

    full_adder adder_46 (tree_2[1][27], tree_2[2][27], tree_2[3][27], tree_3[4][28], tree_3[1][27]);
    full_adder adder_47 (tree_2[4][27], tree_2[5][27], tree_2[6][27], tree_3[5][28], tree_3[2][27]);
    full_adder adder_48 (tree_2[7][27], tree_2[8][27], tree_2[9][27], tree_3[6][28], tree_3[3][27]);
    
    half_adder adder_49 (tree_2[0][28], tree_2[1][28], tree_3[4][29], tree_3[0][28]);
    full_adder adder_50 (tree_2[2][28], tree_2[3][28], tree_2[4][28], tree_3[5][29], tree_3[1][28]);
    full_adder adder_51 (tree_2[5][28], tree_2[6][28], tree_2[7][28], tree_3[6][29], tree_3[2][28]);
    full_adder adder_52 (tree_2[8][28], tree_2[9][28], tree_2[10][28], tree_3[7][29], tree_3[3][28]);

    full_adder adder_53 (tree_2[1][29], tree_2[2][29], tree_2[3][29], tree_3[4][30], tree_3[1][29]);
    full_adder adder_54 (tree_2[4][29], tree_2[5][29], tree_2[6][29], tree_3[5][30], tree_3[2][29]);
    full_adder adder_55 (tree_2[7][29], tree_2[8][29], tree_2[9][29], tree_3[6][30], tree_3[3][29]);
    
    half_adder adder_56 (tree_2[0][30], tree_2[1][30], tree_3[4][31], tree_3[0][30]);
    full_adder adder_57 (tree_2[2][30], tree_2[3][30], tree_2[4][30], tree_3[5][31], tree_3[1][30]);
    full_adder adder_58 (tree_2[5][30], tree_2[6][30], tree_2[7][30], tree_3[6][31], tree_3[2][30]);
    full_adder adder_59 (tree_2[8][30], tree_2[9][30], tree_2[10][30], tree_3[7][31], tree_3[3][30]);
    
    full_adder adder_60 (tree_2[0][31], tree_2[1][31], tree_2[2][31], tree_3[4][32], tree_3[0][31]);
    full_adder adder_61 (tree_2[3][31], tree_2[4][31], tree_2[5][31], tree_3[5][32], tree_3[1][31]);
    full_adder adder_62 (tree_2[6][31], tree_2[7][31], tree_2[8][31], tree_3[6][32], tree_3[2][31]);
    full_adder adder_63 (tree_2[9][31], tree_2[10][31], tree_2[11][31], tree_3[7][32], tree_3[3][31]);
    
    half_adder adder_64 (tree_2[0][32], tree_2[1][32], tree_3[4][33], tree_3[0][32]);
    full_adder adder_65 (tree_2[2][32], tree_2[3][32], tree_2[4][32], tree_3[5][33], tree_3[1][32]);
    full_adder adder_66 (tree_2[5][32], tree_2[6][32], tree_2[7][32], tree_3[6][33], tree_3[2][32]);
    full_adder adder_67 (tree_2[8][32], tree_2[9][32], tree_2[10][32], tree_3[7][33], tree_3[3][32]);
    
    half_adder adder_68 (tree_2[0][33], tree_2[1][33], tree_3[4][34], tree_3[0][33]);
    full_adder adder_69 (tree_2[2][33], tree_2[3][33], tree_2[4][33], tree_3[5][34], tree_3[1][33]);
    full_adder adder_70 (tree_2[5][33], tree_2[6][33], tree_2[7][33], tree_3[6][34], tree_3[2][33]);
    full_adder adder_71 (tree_2[8][33], tree_2[9][33], tree_2[10][33], tree_3[7][34], tree_3[3][33]);
    
    half_adder adder_72 (tree_2[0][34], tree_2[1][34], tree_3[4][35], tree_3[0][34]);
    full_adder adder_73 (tree_2[2][34], tree_2[3][34], tree_2[4][34], tree_3[5][35], tree_3[1][34]);
    full_adder adder_74 (tree_2[5][34], tree_2[6][34], tree_2[7][34], tree_3[6][35], tree_3[2][34]);
    full_adder adder_75 (tree_2[8][34], tree_2[9][34], tree_2[10][34], tree_3[7][35], tree_3[3][34]);
    
    half_adder adder_76 (tree_2[0][35], tree_2[1][35], tree_3[4][36], tree_3[0][35]);
    full_adder adder_77 (tree_2[2][35], tree_2[3][35], tree_2[4][35], tree_3[5][36], tree_3[1][35]);
    full_adder adder_78 (tree_2[5][35], tree_2[6][35], tree_2[7][35], tree_3[6][36], tree_3[2][35]);
    full_adder adder_79 (tree_2[8][35], tree_2[9][35], tree_2[10][35], tree_3[7][36], tree_3[3][35]);

    full_adder adder_80 (tree_2[1][36], tree_2[2][36], tree_2[3][36], tree_3[4][37], tree_3[1][36]);
    full_adder adder_81 (tree_2[4][36], tree_2[5][36], tree_2[6][36], tree_3[5][37], tree_3[2][36]);
    full_adder adder_82 (tree_2[7][36], tree_2[8][36], tree_2[9][36], tree_3[6][37], tree_3[3][36]);

    full_adder adder_83 (tree_2[1][37], tree_2[2][37], tree_2[3][37], tree_3[4][38], tree_3[1][37]);
    full_adder adder_84 (tree_2[4][37], tree_2[5][37], tree_2[6][37], tree_3[5][38], tree_3[2][37]);
    full_adder adder_85 (tree_2[7][37], tree_2[8][37], tree_2[9][37], tree_3[6][38], tree_3[3][37]);

    full_adder adder_86 (tree_2[1][38], tree_2[2][38], tree_2[3][38], tree_3[4][39], tree_3[1][38]);
    full_adder adder_87 (tree_2[4][38], tree_2[5][38], tree_2[6][38], tree_3[5][39], tree_3[2][38]);
    full_adder adder_88 (tree_2[7][38], tree_2[8][38], tree_2[9][38], tree_3[6][39], tree_3[3][38]);

    full_adder adder_89 (tree_2[1][39], tree_2[2][39], tree_2[3][39], tree_3[3][40], tree_3[1][39]);
    full_adder adder_90 (tree_2[4][39], tree_2[5][39], tree_2[6][39], tree_3[4][40], tree_3[2][39]);
    full_adder adder_91 (tree_2[7][39], tree_2[8][39], tree_2[9][39], tree_3[5][40], tree_3[3][39]);
    
    full_adder adder_92 (tree_2[0][40], tree_2[1][40], tree_2[2][40], tree_3[3][41], tree_3[0][40]);
    full_adder adder_93 (tree_2[3][40], tree_2[4][40], tree_2[5][40], tree_3[4][41], tree_3[1][40]);
    full_adder adder_94 (tree_2[6][40], tree_2[7][40], tree_2[8][40], tree_3[5][41], tree_3[2][40]);
    
    half_adder adder_95 (tree_2[0][41], tree_2[1][41], tree_3[3][42], tree_3[0][41]);
    full_adder adder_96 (tree_2[2][41], tree_2[3][41], tree_2[4][41], tree_3[4][42], tree_3[1][41]);
    full_adder adder_97 (tree_2[5][41], tree_2[6][41], tree_2[7][41], tree_3[5][42], tree_3[2][41]);
    
    half_adder adder_98 (tree_2[0][42], tree_2[1][42], tree_3[3][43], tree_3[0][42]);
    full_adder adder_99 (tree_2[2][42], tree_2[3][42], tree_2[4][42], tree_3[4][43], tree_3[1][42]);
    full_adder adder_100 (tree_2[5][42], tree_2[6][42], tree_2[7][42], tree_3[5][43], tree_3[2][42]);
    
    half_adder adder_101 (tree_2[0][43], tree_2[1][43], tree_3[3][44], tree_3[0][43]);
    full_adder adder_102 (tree_2[2][43], tree_2[3][43], tree_2[4][43], tree_3[4][44], tree_3[1][43]);
    full_adder adder_103 (tree_2[5][43], tree_2[6][43], tree_2[7][43], tree_3[5][44], tree_3[2][43]);
    
    half_adder adder_104 (tree_2[0][44], tree_2[1][44], tree_3[3][45], tree_3[0][44]);
    full_adder adder_105 (tree_2[2][44], tree_2[3][44], tree_2[4][44], tree_3[4][45], tree_3[1][44]);
    full_adder adder_106 (tree_2[5][44], tree_2[6][44], tree_2[7][44], tree_3[5][45], tree_3[2][44]);
    
    half_adder adder_107 (tree_2[0][45], tree_2[1][45], tree_3[3][46], tree_3[0][45]);
    full_adder adder_108 (tree_2[2][45], tree_2[3][45], tree_2[4][45], tree_3[4][46], tree_3[1][45]);
    full_adder adder_109 (tree_2[5][45], tree_2[6][45], tree_2[7][45], tree_3[5][46], tree_3[2][45]);

    full_adder adder_110 (tree_2[1][46], tree_2[2][46], tree_2[3][46], tree_3[2][47], tree_3[1][46]);
    full_adder adder_111 (tree_2[4][46], tree_2[5][46], tree_2[6][46], tree_3[3][47], tree_3[2][46]);
    
    full_adder adder_112 (tree_2[0][47], tree_2[1][47], tree_2[2][47], tree_3[2][48], tree_3[0][47]);
    full_adder adder_113 (tree_2[3][47], tree_2[4][47], tree_2[5][47], tree_3[3][48], tree_3[1][47]);
    
    full_adder adder_114 (tree_2[0][48], tree_2[1][48], tree_2[2][48], tree_3[2][49], tree_3[0][48]);
    full_adder adder_115 (tree_2[3][48], tree_2[4][48], tree_2[5][48], tree_3[3][49], tree_3[1][48]);
    
    full_adder adder_116 (tree_2[0][49], tree_2[1][49], tree_2[2][49], tree_3[2][50], tree_3[0][49]);
    full_adder adder_117 (tree_2[3][49], tree_2[4][49], tree_2[5][49], tree_3[3][50], tree_3[1][49]);
    
    full_adder adder_118 (tree_2[0][50], tree_2[1][50], tree_2[2][50], tree_3[2][51], tree_3[0][50]);
    full_adder adder_119 (tree_2[3][50], tree_2[4][50], tree_2[5][50], tree_3[3][51], tree_3[1][50]);
    
    full_adder adder_120 (tree_2[0][51], tree_2[1][51], tree_2[2][51], tree_3[2][52], tree_3[0][51]);
    full_adder adder_121 (tree_2[3][51], tree_2[4][51], tree_2[5][51], tree_3[3][52], tree_3[1][51]);
    
    half_adder adder_122 (tree_2[0][52], tree_2[1][52], tree_3[2][53], tree_3[0][52]);
    full_adder adder_123 (tree_2[2][52], tree_2[3][52], tree_2[4][52], tree_3[3][53], tree_3[1][52]);

    full_adder adder_124 (tree_2[1][53], tree_2[2][53], tree_2[3][53], tree_3[2][54], tree_3[1][53]);
    full_adder adder_125 (tree_2[1][54], tree_2[2][54], tree_2[3][54], tree_3[2][55], tree_3[1][54]);
    full_adder adder_126 (tree_2[1][55], tree_2[2][55], tree_2[3][55], tree_3[2][56], tree_3[1][55]);
    full_adder adder_127 (tree_2[1][56], tree_2[2][56], tree_2[3][56], tree_3[2][57], tree_3[1][56]);
    full_adder adder_128 (tree_2[1][57], tree_2[2][57], tree_2[3][57], tree_3[1][58], tree_3[1][57]);
    
    full_adder adder_129 (tree_2[0][58], tree_2[1][58], tree_2[2][58], tree_3[1][59], tree_3[0][58]);
    half_adder adder_130 (tree_2[0][59], tree_2[1][59], tree_3[1][60], tree_3[0][59]);
    half_adder adder_131 (tree_2[0][60], tree_2[1][60], tree_3[1][61], tree_3[0][60]);
    half_adder adder_132 (tree_2[0][61], tree_2[1][61], tree_3[1][62], tree_3[0][61]);
    half_adder adder_133 (tree_2[0][62], tree_2[1][62], tree_3[1][63], tree_3[0][62]);
    half_adder adder_134 (tree_2[0][63], tree_2[1][63], , tree_3[0][63]);

endmodule


module csa_level_3 (tree_3, tree_4);

    input [63:0] tree_3 [7:0];
    output [63:0] tree_4 [5:0];

    assign tree_4[0][53:47] = tree_3[0][53:47];
    assign tree_4[0][39:37] = tree_3[0][39:37];
    assign tree_4[0][30] = tree_3[0][30];
    assign tree_4[0][28:27] = tree_3[0][28:27];
    assign tree_4[0][25] = tree_3[0][25];
    assign tree_4[0][17] = tree_3[0][17];
    assign tree_4[0][15:13] = tree_3[0][15:13];
    assign tree_4[0][11] = tree_3[0][11];
    assign tree_4[0][6:0] = tree_3[0][6:0];
    assign tree_4[1][6:5] = tree_3[1][6:5];
    assign tree_4[1][3] = tree_3[1][3];
    assign tree_4[1][0] = tree_3[1][0];

    full_adder adder_0 (tree_3[0][7], tree_3[1][7], tree_3[2][7], tree_4[1][8], tree_4[0][7]);
    half_adder adder_1 (tree_3[0][8], tree_3[1][8], tree_4[1][9], tree_4[0][8]);
    full_adder adder_2 (tree_3[0][9], tree_3[1][9], tree_3[2][9], tree_4[1][10], tree_4[0][9]);
    full_adder adder_3 (tree_3[0][10], tree_3[1][10], tree_3[2][10], tree_4[2][11], tree_4[0][10]);
    full_adder adder_4 (tree_3[1][11], tree_3[2][11], tree_3[3][11], tree_4[1][12], tree_4[1][11]);
    full_adder adder_5 (tree_3[0][12], tree_3[1][12], tree_3[2][12], tree_4[2][13], tree_4[0][12]);
    full_adder adder_6 (tree_3[1][13], tree_3[2][13], tree_3[3][13], tree_4[2][14], tree_4[1][13]);
    full_adder adder_7 (tree_3[1][14], tree_3[2][14], tree_3[3][14], tree_4[2][15], tree_4[1][14]);
    full_adder adder_8 (tree_3[1][15], tree_3[2][15], tree_3[3][15], tree_4[2][16], tree_4[1][15]);
    
    half_adder adder_9 (tree_3[0][16], tree_3[1][16], tree_4[2][17], tree_4[0][16]);
    full_adder adder_10 (tree_3[2][16], tree_3[3][16], tree_3[4][16], tree_4[3][17], tree_4[1][16]);
    
    full_adder adder_11 (tree_3[1][17], tree_3[2][17], tree_3[3][17], tree_4[2][18], tree_4[1][17]);
    
    half_adder adder_12 (tree_3[0][18], tree_3[1][18], tree_4[2][19], tree_4[0][18]);
    full_adder adder_13 (tree_3[2][18], tree_3[3][18], tree_3[4][18], tree_4[3][19], tree_4[1][18]);
    
    half_adder adder_14 (tree_3[0][19], tree_3[1][19], tree_4[2][20], tree_4[0][19]);
    full_adder adder_15 (tree_3[2][19], tree_3[3][19], tree_3[4][19], tree_4[3][20], tree_4[1][19]);
    
    full_adder adder_16 (tree_3[0][20], tree_3[1][20], tree_3[2][20], tree_4[2][21], tree_4[0][20]);
    full_adder adder_17 (tree_3[3][20], tree_3[4][20], tree_3[5][20], tree_4[3][21], tree_4[1][20]);
    
    half_adder adder_18 (tree_3[0][21], tree_3[1][21], tree_4[2][22], tree_4[0][21]);
    full_adder adder_19 (tree_3[2][21], tree_3[3][21], tree_3[4][21], tree_4[3][22], tree_4[1][21]);
    
    full_adder adder_20 (tree_3[0][22], tree_3[1][22], tree_3[2][22], tree_4[2][23], tree_4[0][22]);
    full_adder adder_21 (tree_3[3][22], tree_3[4][22], tree_3[5][22], tree_4[3][23], tree_4[1][22]);
    
    full_adder adder_22 (tree_3[0][23], tree_3[1][23], tree_3[2][23], tree_4[2][24], tree_4[0][23]);
    full_adder adder_23 (tree_3[3][23], tree_3[4][23], tree_3[5][23], tree_4[3][24], tree_4[1][23]);
    
    full_adder adder_24 (tree_3[0][24], tree_3[1][24], tree_3[2][24], tree_4[3][25], tree_4[0][24]);
    full_adder adder_25 (tree_3[3][24], tree_3[4][24], tree_3[5][24], tree_4[4][25], tree_4[1][24]);
    
    full_adder adder_26 (tree_3[1][25], tree_3[2][25], tree_3[3][25], tree_4[2][26], tree_4[1][25]);
    full_adder adder_27 (tree_3[4][25], tree_3[5][25], tree_3[6][25], tree_4[3][26], tree_4[2][25]);
    
    full_adder adder_28 (tree_3[0][26], tree_3[1][26], tree_3[2][26], tree_4[3][27], tree_4[0][26]);
    full_adder adder_29 (tree_3[3][26], tree_3[4][26], tree_3[5][26], tree_4[4][27], tree_4[1][26]);
    
    full_adder adder_30 (tree_3[1][27], tree_3[2][27], tree_3[3][27], tree_4[3][28], tree_4[1][27]);
    full_adder adder_31 (tree_3[4][27], tree_3[5][27], tree_3[6][27], tree_4[4][28], tree_4[2][27]);
    
    full_adder adder_32 (tree_3[1][28], tree_3[2][28], tree_3[3][28], tree_4[3][29], tree_4[1][28]);
    full_adder adder_33 (tree_3[4][28], tree_3[5][28], tree_3[6][28], tree_4[4][29], tree_4[2][28]);
    
    half_adder adder_34 (tree_3[0][29], tree_3[1][29], tree_4[3][30], tree_4[0][29]);
    full_adder adder_35 (tree_3[2][29], tree_3[3][29], tree_3[4][29], tree_4[4][30], tree_4[1][29]);
    full_adder adder_36 (tree_3[5][29], tree_3[6][29], tree_3[7][29], tree_4[5][30], tree_4[2][29]);
    
    full_adder adder_37 (tree_3[1][30], tree_3[2][30], tree_3[3][30], tree_4[3][31], tree_4[1][30]);
    full_adder adder_38 (tree_3[4][30], tree_3[5][30], tree_3[6][30], tree_4[4][31], tree_4[2][30]);
    
    half_adder adder_39 (tree_3[0][31], tree_3[1][31], tree_4[3][32], tree_4[0][31]);
    full_adder adder_40 (tree_3[2][31], tree_3[3][31], tree_3[4][31], tree_4[4][32], tree_4[1][31]);
    full_adder adder_41 (tree_3[5][31], tree_3[6][31], tree_3[7][31], tree_4[5][32], tree_4[2][31]);
    
    half_adder adder_42 (tree_3[0][32], tree_3[1][32], tree_4[3][33], tree_4[0][32]);
    full_adder adder_43 (tree_3[2][32], tree_3[3][32], tree_3[4][32], tree_4[4][33], tree_4[1][32]);
    full_adder adder_44 (tree_3[5][32], tree_3[6][32], tree_3[7][32], tree_4[5][33], tree_4[2][32]);
    
    half_adder adder_45 (tree_3[0][33], tree_3[1][33], tree_4[3][34], tree_4[0][33]);
    full_adder adder_46 (tree_3[2][33], tree_3[3][33], tree_3[4][33], tree_4[4][34], tree_4[1][33]);
    full_adder adder_47 (tree_3[5][33], tree_3[6][33], tree_3[7][33], tree_4[5][34], tree_4[2][33]);
    
    half_adder adder_48 (tree_3[0][34], tree_3[1][34], tree_4[3][35], tree_4[0][34]);
    full_adder adder_49 (tree_3[2][34], tree_3[3][34], tree_3[4][34], tree_4[4][35], tree_4[1][34]);
    full_adder adder_50 (tree_3[5][34], tree_3[6][34], tree_3[7][34], tree_4[5][35], tree_4[2][34]);
    
    half_adder adder_51 (tree_3[0][35], tree_3[1][35], tree_4[3][36], tree_4[0][35]);
    full_adder adder_52 (tree_3[2][35], tree_3[3][35], tree_3[4][35], tree_4[4][36], tree_4[1][35]);
    full_adder adder_53 (tree_3[5][35], tree_3[6][35], tree_3[7][35], tree_4[5][36], tree_4[2][35]);
    
    half_adder adder_54 (tree_3[0][36], tree_3[1][36], tree_4[3][37], tree_4[0][36]);
    full_adder adder_55 (tree_3[2][36], tree_3[3][36], tree_3[4][36], tree_4[4][37], tree_4[1][36]);
    full_adder adder_56 (tree_3[5][36], tree_3[6][36], tree_3[7][36], tree_4[5][37], tree_4[2][36]);
    
    full_adder adder_57 (tree_3[1][37], tree_3[2][37], tree_3[3][37], tree_4[3][38], tree_4[1][37]);
    full_adder adder_58 (tree_3[4][37], tree_3[5][37], tree_3[6][37], tree_4[4][38], tree_4[2][37]);
    
    full_adder adder_59 (tree_3[1][38], tree_3[2][38], tree_3[3][38], tree_4[3][39], tree_4[1][38]);
    full_adder adder_60 (tree_3[4][38], tree_3[5][38], tree_3[6][38], tree_4[4][39], tree_4[2][38]);
    
    full_adder adder_61 (tree_3[1][39], tree_3[2][39], tree_3[3][39], tree_4[2][40], tree_4[1][39]);
    full_adder adder_62 (tree_3[4][39], tree_3[5][39], tree_3[6][39], tree_4[3][40], tree_4[2][39]);
    
    full_adder adder_63 (tree_3[0][40], tree_3[1][40], tree_3[2][40], tree_4[2][41], tree_4[0][40]);
    full_adder adder_64 (tree_3[3][40], tree_3[4][40], tree_3[5][40], tree_4[3][41], tree_4[1][40]);
    
    full_adder adder_65 (tree_3[0][41], tree_3[1][41], tree_3[2][41], tree_4[2][42], tree_4[0][41]);
    full_adder adder_66 (tree_3[3][41], tree_3[4][41], tree_3[5][41], tree_4[3][42], tree_4[1][41]);
    
    full_adder adder_67 (tree_3[0][42], tree_3[1][42], tree_3[2][42], tree_4[2][43], tree_4[0][42]);
    full_adder adder_68 (tree_3[3][42], tree_3[4][42], tree_3[5][42], tree_4[3][43], tree_4[1][42]);
    
    full_adder adder_69 (tree_3[0][43], tree_3[1][43], tree_3[2][43], tree_4[2][44], tree_4[0][43]);
    full_adder adder_70 (tree_3[3][43], tree_3[4][43], tree_3[5][43], tree_4[3][44], tree_4[1][43]);
    
    full_adder adder_71 (tree_3[0][44], tree_3[1][44], tree_3[2][44], tree_4[2][45], tree_4[0][44]);
    full_adder adder_72 (tree_3[3][44], tree_3[4][44], tree_3[5][44], tree_4[3][45], tree_4[1][44]);
    
    full_adder adder_73 (tree_3[0][45], tree_3[1][45], tree_3[2][45], tree_4[2][46], tree_4[0][45]);
    full_adder adder_74 (tree_3[3][45], tree_3[4][45], tree_3[5][45], tree_4[3][46], tree_4[1][45]);
    
    full_adder adder_75 (tree_3[0][46], tree_3[1][46], tree_3[2][46], tree_4[2][47], tree_4[0][46]);
    full_adder adder_76 (tree_3[3][46], tree_3[4][46], tree_3[5][46], tree_4[3][47], tree_4[1][46]);
    
    full_adder adder_77 (tree_3[1][47], tree_3[2][47], tree_3[3][47], tree_4[2][48], tree_4[1][47]);
    full_adder adder_78 (tree_3[1][48], tree_3[2][48], tree_3[3][48], tree_4[2][49], tree_4[1][48]);
    full_adder adder_79 (tree_3[1][49], tree_3[2][49], tree_3[3][49], tree_4[2][50], tree_4[1][49]);
    full_adder adder_80 (tree_3[1][50], tree_3[2][50], tree_3[3][50], tree_4[2][51], tree_4[1][50]);
    full_adder adder_81 (tree_3[1][51], tree_3[2][51], tree_3[3][51], tree_4[2][52], tree_4[1][51]);
    full_adder adder_82 (tree_3[1][52], tree_3[2][52], tree_3[3][52], tree_4[2][53], tree_4[1][52]);
    full_adder adder_83 (tree_3[1][53], tree_3[2][53], tree_3[3][53], tree_4[1][54], tree_4[1][53]);
    full_adder adder_84 (tree_3[0][54], tree_3[1][54], tree_3[2][54], tree_4[1][55], tree_4[0][54]);
    full_adder adder_85 (tree_3[0][55], tree_3[1][55], tree_3[2][55], tree_4[1][56], tree_4[0][55]);
    full_adder adder_86 (tree_3[0][56], tree_3[1][56], tree_3[2][56], tree_4[1][57], tree_4[0][56]);
    full_adder adder_87 (tree_3[0][57], tree_3[1][57], tree_3[2][57], tree_4[1][58], tree_4[0][57]);
    half_adder adder_88 (tree_3[0][58], tree_3[1][58], tree_4[1][59], tree_4[0][58]);
    half_adder adder_89 (tree_3[0][59], tree_3[1][59], tree_4[1][60], tree_4[0][59]);
    half_adder adder_90 (tree_3[0][60], tree_3[1][60], tree_4[1][61], tree_4[0][60]);
    half_adder adder_91 (tree_3[0][61], tree_3[1][61], tree_4[1][62], tree_4[0][61]);
    half_adder adder_92 (tree_3[0][62], tree_3[1][62], tree_4[1][63], tree_4[0][62]);
    half_adder adder_93 (tree_3[0][63], tree_3[1][63], , tree_4[0][63]);

endmodule


module csa_level_4 (tree_4, tree_5);

    input [63:0] tree_4 [5:0];
    output [63:0] tree_5 [4:0];

    assign tree_5[0][47:40] = tree_4[0][47:40];
    assign tree_5[0][26] = tree_4[0][26];
    assign tree_5[0][24:19] = tree_4[0][24:19];
    assign tree_5[0][17] = tree_4[0][17];
    assign tree_5[0][10:0] = tree_4[0][10:0];
    assign tree_5[1][10:8] = tree_4[1][10:8];
    assign tree_5[1][6:5] = tree_4[1][6:5];
    assign tree_5[1][3] = tree_4[1][3];
    assign tree_5[1][0] = tree_4[1][0];
    
    full_adder adder_0 (tree_4[0][11], tree_4[1][11], tree_4[2][11], tree_5[1][12], tree_5[0][11]);
    half_adder adder_1 (tree_4[0][12], tree_4[1][12], tree_5[1][13], tree_5[0][12]);
    full_adder adder_2 (tree_4[0][13], tree_4[1][13], tree_4[2][13], tree_5[1][14], tree_5[0][13]);
    full_adder adder_3 (tree_4[0][14], tree_4[1][14], tree_4[2][14], tree_5[1][15], tree_5[0][14]);
    full_adder adder_4 (tree_4[0][15], tree_4[1][15], tree_4[2][15], tree_5[1][16], tree_5[0][15]);
    full_adder adder_5 (tree_4[0][16], tree_4[1][16], tree_4[2][16], tree_5[2][17], tree_5[0][16]);
    full_adder adder_6 (tree_4[1][17], tree_4[2][17], tree_4[3][17], tree_5[1][18], tree_5[1][17]);
    full_adder adder_7 (tree_4[0][18], tree_4[1][18], tree_4[2][18], tree_5[2][19], tree_5[0][18]);
    full_adder adder_8 (tree_4[1][19], tree_4[2][19], tree_4[3][19], tree_5[2][20], tree_5[1][19]);
    full_adder adder_9 (tree_4[1][20], tree_4[2][20], tree_4[3][20], tree_5[2][21], tree_5[1][20]);
    full_adder adder_10 (tree_4[1][21], tree_4[2][21], tree_4[3][21], tree_5[2][22], tree_5[1][21]);
    full_adder adder_11 (tree_4[1][22], tree_4[2][22], tree_4[3][22], tree_5[2][23], tree_5[1][22]);
    full_adder adder_12 (tree_4[1][23], tree_4[2][23], tree_4[3][23], tree_5[2][24], tree_5[1][23]);
    full_adder adder_13 (tree_4[1][24], tree_4[2][24], tree_4[3][24], tree_5[2][25], tree_5[1][24]);
    
    half_adder adder_14 (tree_4[0][25], tree_4[1][25], tree_5[2][26], tree_5[0][25]);
    full_adder adder_15 (tree_4[2][25], tree_4[3][25], tree_4[4][25], tree_5[3][26], tree_5[1][25]);
    
    full_adder adder_16 (tree_4[1][26], tree_4[2][26], tree_4[3][26], tree_5[2][27], tree_5[1][26]);
    
    half_adder adder_17 (tree_4[0][27], tree_4[1][27], tree_5[2][28], tree_5[0][27]);
    full_adder adder_18 (tree_4[2][27], tree_4[3][27], tree_4[4][27], tree_5[3][28], tree_5[1][27]);
    
    half_adder adder_19 (tree_4[0][28], tree_4[1][28], tree_5[2][29], tree_5[0][28]);
    full_adder adder_20 (tree_4[2][28], tree_4[3][28], tree_4[4][28], tree_5[3][29], tree_5[1][28]);
    
    half_adder adder_21 (tree_4[0][29], tree_4[1][29], tree_5[2][30], tree_5[0][29]);
    full_adder adder_22 (tree_4[2][29], tree_4[3][29], tree_4[4][29], tree_5[3][30], tree_5[1][29]);
    
    full_adder adder_23 (tree_4[0][30], tree_4[1][30], tree_4[2][30], tree_5[2][31], tree_5[0][30]);
    full_adder adder_24 (tree_4[3][30], tree_4[4][30], tree_4[5][30], tree_5[3][31], tree_5[1][30]);
    
    half_adder adder_25 (tree_4[0][31], tree_4[1][31], tree_5[2][32], tree_5[0][31]);
    full_adder adder_26 (tree_4[2][31], tree_4[3][31], tree_4[4][31], tree_5[3][32], tree_5[1][31]);
    
    full_adder adder_27 (tree_4[0][32], tree_4[1][32], tree_4[2][32], tree_5[2][33], tree_5[0][32]);
    full_adder adder_28 (tree_4[3][32], tree_4[4][32], tree_4[5][32], tree_5[3][33], tree_5[1][32]);
    
    full_adder adder_29 (tree_4[0][33], tree_4[1][33], tree_4[2][33], tree_5[2][34], tree_5[0][33]);
    full_adder adder_30 (tree_4[3][33], tree_4[4][33], tree_4[5][33], tree_5[3][34], tree_5[1][33]);
    
    full_adder adder_31 (tree_4[0][34], tree_4[1][34], tree_4[2][34], tree_5[2][35], tree_5[0][34]);
    full_adder adder_32 (tree_4[3][34], tree_4[4][34], tree_4[5][34], tree_5[3][35], tree_5[1][34]);
    
    full_adder adder_33 (tree_4[0][35], tree_4[1][35], tree_4[2][35], tree_5[2][36], tree_5[0][35]);
    full_adder adder_34 (tree_4[3][35], tree_4[4][35], tree_4[5][35], tree_5[3][36], tree_5[1][35]);
    
    full_adder adder_35 (tree_4[0][36], tree_4[1][36], tree_4[2][36], tree_5[2][37], tree_5[0][36]);
    full_adder adder_36 (tree_4[3][36], tree_4[4][36], tree_4[5][36], tree_5[3][37], tree_5[1][36]);
    
    full_adder adder_37 (tree_4[0][37], tree_4[1][37], tree_4[2][37], tree_5[2][38], tree_5[0][37]);
    full_adder adder_38 (tree_4[3][37], tree_4[4][37], tree_4[5][37], tree_5[3][38], tree_5[1][37]);
    
    half_adder adder_39 (tree_4[0][38], tree_4[1][38], tree_5[2][39], tree_5[0][38]);
    full_adder adder_40 (tree_4[2][38], tree_4[3][38], tree_4[4][38], tree_5[3][39], tree_5[1][38]);
    
    half_adder adder_41 (tree_4[0][39], tree_4[1][39], tree_5[2][40], tree_5[0][39]);
    full_adder adder_42 (tree_4[2][39], tree_4[3][39], tree_4[4][39], tree_5[3][40], tree_5[1][39]);
    
    full_adder adder_43 (tree_4[1][40], tree_4[2][40], tree_4[3][40], tree_5[2][41], tree_5[1][40]);
    full_adder adder_44 (tree_4[1][41], tree_4[2][41], tree_4[3][41], tree_5[2][42], tree_5[1][41]);
    full_adder adder_45 (tree_4[1][42], tree_4[2][42], tree_4[3][42], tree_5[2][43], tree_5[1][42]);
    full_adder adder_46 (tree_4[1][43], tree_4[2][43], tree_4[3][43], tree_5[2][44], tree_5[1][43]);
    full_adder adder_47 (tree_4[1][44], tree_4[2][44], tree_4[3][44], tree_5[2][45], tree_5[1][44]);
    full_adder adder_48 (tree_4[1][45], tree_4[2][45], tree_4[3][45], tree_5[2][46], tree_5[1][45]);
    full_adder adder_49 (tree_4[1][46], tree_4[2][46], tree_4[3][46], tree_5[2][47], tree_5[1][46]);
    full_adder adder_50 (tree_4[1][47], tree_4[2][47], tree_4[3][47], tree_5[1][48], tree_5[1][47]);
    full_adder adder_51 (tree_4[0][48], tree_4[1][48], tree_4[2][48], tree_5[1][49], tree_5[0][48]);
    full_adder adder_52 (tree_4[0][49], tree_4[1][49], tree_4[2][49], tree_5[1][50], tree_5[0][49]);
    full_adder adder_53 (tree_4[0][50], tree_4[1][50], tree_4[2][50], tree_5[1][51], tree_5[0][50]);
    full_adder adder_54 (tree_4[0][51], tree_4[1][51], tree_4[2][51], tree_5[1][52], tree_5[0][51]);
    full_adder adder_55 (tree_4[0][52], tree_4[1][52], tree_4[2][52], tree_5[1][53], tree_5[0][52]);
    full_adder adder_56 (tree_4[0][53], tree_4[1][53], tree_4[2][53], tree_5[1][54], tree_5[0][53]);
    half_adder adder_57 (tree_4[0][54], tree_4[1][54], tree_5[1][55], tree_5[0][54]);
    half_adder adder_58 (tree_4[0][55], tree_4[1][55], tree_5[1][56], tree_5[0][55]);
    half_adder adder_59 (tree_4[0][56], tree_4[1][56], tree_5[1][57], tree_5[0][56]);
    half_adder adder_60 (tree_4[0][57], tree_4[1][57], tree_5[1][58], tree_5[0][57]);
    half_adder adder_61 (tree_4[0][58], tree_4[1][58], tree_5[1][59], tree_5[0][58]);
    half_adder adder_62 (tree_4[0][59], tree_4[1][59], tree_5[1][60], tree_5[0][59]);
    half_adder adder_63 (tree_4[0][60], tree_4[1][60], tree_5[1][61], tree_5[0][60]);
    half_adder adder_64 (tree_4[0][61], tree_4[1][61], tree_5[1][62], tree_5[0][61]);
    half_adder adder_65 (tree_4[0][62], tree_4[1][62], tree_5[1][63], tree_5[0][62]);
    half_adder adder_66 (tree_4[0][63], tree_4[1][63], , tree_5[0][63]);

endmodule


module csa_level_5 (tree_5, tree_6);

    input [63:0] tree_5 [4:0];
    output [63:0] tree_6 [2:0];

    assign tree_6[0][40:28] = tree_5[0][40:28];
    assign tree_6[0][26] = tree_5[0][26];
    assign tree_6[0][16:0] = tree_5[0][16:0];
    assign tree_6[1][16:12] = tree_5[1][16:12];
    assign tree_6[1][10:8] = tree_5[1][10:8];
    assign tree_6[1][6:5] = tree_5[1][6:5];
    assign tree_6[1][3] = tree_5[1][3];
    assign tree_6[1][0] = tree_5[1][0];
    
    full_adder adder_0 (tree_5[0][17], tree_5[1][17], tree_5[2][17], tree_6[1][18], tree_6[0][17]);
    half_adder adder_1 (tree_5[0][18], tree_5[1][18], tree_6[1][19], tree_6[0][18]);
    full_adder adder_2 (tree_5[0][19], tree_5[1][19], tree_5[2][19], tree_6[1][20], tree_6[0][19]);
    full_adder adder_3 (tree_5[0][20], tree_5[1][20], tree_5[2][20], tree_6[1][21], tree_6[0][20]);
    full_adder adder_4 (tree_5[0][21], tree_5[1][21], tree_5[2][21], tree_6[1][22], tree_6[0][21]);
    full_adder adder_5 (tree_5[0][22], tree_5[1][22], tree_5[2][22], tree_6[1][23], tree_6[0][22]);
    full_adder adder_6 (tree_5[0][23], tree_5[1][23], tree_5[2][23], tree_6[1][24], tree_6[0][23]);
    full_adder adder_7 (tree_5[0][24], tree_5[1][24], tree_5[2][24], tree_6[1][25], tree_6[0][24]);
    full_adder adder_8 (tree_5[0][25], tree_5[1][25], tree_5[2][25], tree_6[2][26], tree_6[0][25]);
    full_adder adder_9 (tree_5[1][26], tree_5[2][26], tree_5[3][26], tree_6[1][27], tree_6[1][26]);
    full_adder adder_10 (tree_5[0][27], tree_5[1][27], tree_5[2][27], tree_6[2][28], tree_6[0][27]);
    full_adder adder_11 (tree_5[1][28], tree_5[2][28], tree_5[3][28], tree_6[2][29], tree_6[1][28]);
    full_adder adder_12 (tree_5[1][29], tree_5[2][29], tree_5[3][29], tree_6[2][30], tree_6[1][29]);
    full_adder adder_13 (tree_5[1][30], tree_5[2][30], tree_5[3][30], tree_6[2][31], tree_6[1][30]);
    full_adder adder_14 (tree_5[1][31], tree_5[2][31], tree_5[3][31], tree_6[2][32], tree_6[1][31]);
    full_adder adder_15 (tree_5[1][32], tree_5[2][32], tree_5[3][32], tree_6[2][33], tree_6[1][32]);
    full_adder adder_16 (tree_5[1][33], tree_5[2][33], tree_5[3][33], tree_6[2][34], tree_6[1][33]);
    full_adder adder_17 (tree_5[1][34], tree_5[2][34], tree_5[3][34], tree_6[2][35], tree_6[1][34]);
    full_adder adder_18 (tree_5[1][35], tree_5[2][35], tree_5[3][35], tree_6[2][36], tree_6[1][35]);
    full_adder adder_19 (tree_5[1][36], tree_5[2][36], tree_5[3][36], tree_6[2][37], tree_6[1][36]);
    full_adder adder_20 (tree_5[1][37], tree_5[2][37], tree_5[3][37], tree_6[2][38], tree_6[1][37]);
    full_adder adder_21 (tree_5[1][38], tree_5[2][38], tree_5[3][38], tree_6[2][39], tree_6[1][38]);
    full_adder adder_22 (tree_5[1][39], tree_5[2][39], tree_5[3][39], tree_6[2][40], tree_6[1][39]);
    full_adder adder_23 (tree_5[1][40], tree_5[2][40], tree_5[3][40], tree_6[1][41], tree_6[1][40]);
    full_adder adder_24 (tree_5[0][41], tree_5[1][41], tree_5[2][41], tree_6[1][42], tree_6[0][41]);
    full_adder adder_25 (tree_5[0][42], tree_5[1][42], tree_5[2][42], tree_6[1][43], tree_6[0][42]);
    full_adder adder_26 (tree_5[0][43], tree_5[1][43], tree_5[2][43], tree_6[1][44], tree_6[0][43]);
    full_adder adder_27 (tree_5[0][44], tree_5[1][44], tree_5[2][44], tree_6[1][45], tree_6[0][44]);
    full_adder adder_28 (tree_5[0][45], tree_5[1][45], tree_5[2][45], tree_6[1][46], tree_6[0][45]);
    full_adder adder_29 (tree_5[0][46], tree_5[1][46], tree_5[2][46], tree_6[1][47], tree_6[0][46]);
    full_adder adder_30 (tree_5[0][47], tree_5[1][47], tree_5[2][47], tree_6[1][48], tree_6[0][47]);
    half_adder adder_31 (tree_5[0][48], tree_5[1][48], tree_6[1][49], tree_6[0][48]);
    half_adder adder_32 (tree_5[0][49], tree_5[1][49], tree_6[1][50], tree_6[0][49]);
    half_adder adder_33 (tree_5[0][50], tree_5[1][50], tree_6[1][51], tree_6[0][50]);
    half_adder adder_34 (tree_5[0][51], tree_5[1][51], tree_6[1][52], tree_6[0][51]);
    half_adder adder_35 (tree_5[0][52], tree_5[1][52], tree_6[1][53], tree_6[0][52]);
    half_adder adder_36 (tree_5[0][53], tree_5[1][53], tree_6[1][54], tree_6[0][53]);
    half_adder adder_37 (tree_5[0][54], tree_5[1][54], tree_6[1][55], tree_6[0][54]);
    half_adder adder_38 (tree_5[0][55], tree_5[1][55], tree_6[1][56], tree_6[0][55]);
    half_adder adder_39 (tree_5[0][56], tree_5[1][56], tree_6[1][57], tree_6[0][56]);
    half_adder adder_40 (tree_5[0][57], tree_5[1][57], tree_6[1][58], tree_6[0][57]);
    half_adder adder_41 (tree_5[0][58], tree_5[1][58], tree_6[1][59], tree_6[0][58]);
    half_adder adder_42 (tree_5[0][59], tree_5[1][59], tree_6[1][60], tree_6[0][59]);
    half_adder adder_43 (tree_5[0][60], tree_5[1][60], tree_6[1][61], tree_6[0][60]);
    half_adder adder_44 (tree_5[0][61], tree_5[1][61], tree_6[1][62], tree_6[0][61]);
    half_adder adder_45 (tree_5[0][62], tree_5[1][62], tree_6[1][63], tree_6[0][62]);
    half_adder adder_46 (tree_5[0][63], tree_5[1][63], , tree_6[0][63]);

endmodule


module csa_level_6 (tree_6, tree_7);

    input [63:0] tree_6 [2:0];
    output [63:0] tree_7 [1:0];

    assign tree_7[0][25:0] = tree_6[0][25:0];
    assign tree_7[1][25:18] = tree_6[1][25:18];
    assign tree_7[1][16:12] = tree_6[1][16:12];
    assign tree_7[1][10:8] = tree_6[1][10:8];
    assign tree_7[1][6:5] = tree_6[1][6:5];
    assign tree_7[1][3] = tree_6[1][3];
    assign tree_7[1][0] = tree_6[1][0];
    assign tree_7[1][26] = 1'b0;
    assign tree_7[1][17] = 1'b0;
    assign tree_7[1][11] = 1'b0;
    assign tree_7[1][7] = 1'b0;
    assign tree_7[1][4] = 1'b0;
    assign tree_7[1][2:1] = 2'b0;
    
    full_adder adder_0 (tree_6[0][26], tree_6[1][26], tree_6[2][26], tree_7[1][27], tree_7[0][26]);
    half_adder adder_1 (tree_6[0][27], tree_6[1][27], tree_7[1][28], tree_7[0][27]);
    full_adder adder_2 (tree_6[0][28], tree_6[1][28], tree_6[2][28], tree_7[1][29], tree_7[0][28]);
    full_adder adder_3 (tree_6[0][29], tree_6[1][29], tree_6[2][29], tree_7[1][30], tree_7[0][29]);
    full_adder adder_4 (tree_6[0][30], tree_6[1][30], tree_6[2][30], tree_7[1][31], tree_7[0][30]);
    full_adder adder_5 (tree_6[0][31], tree_6[1][31], tree_6[2][31], tree_7[1][32], tree_7[0][31]);
    full_adder adder_6 (tree_6[0][32], tree_6[1][32], tree_6[2][32], tree_7[1][33], tree_7[0][32]);
    full_adder adder_7 (tree_6[0][33], tree_6[1][33], tree_6[2][33], tree_7[1][34], tree_7[0][33]);
    full_adder adder_8 (tree_6[0][34], tree_6[1][34], tree_6[2][34], tree_7[1][35], tree_7[0][34]);
    full_adder adder_9 (tree_6[0][35], tree_6[1][35], tree_6[2][35], tree_7[1][36], tree_7[0][35]);
    full_adder adder_10 (tree_6[0][36], tree_6[1][36], tree_6[2][36], tree_7[1][37], tree_7[0][36]);
    full_adder adder_11 (tree_6[0][37], tree_6[1][37], tree_6[2][37], tree_7[1][38], tree_7[0][37]);
    full_adder adder_12 (tree_6[0][38], tree_6[1][38], tree_6[2][38], tree_7[1][39], tree_7[0][38]);
    full_adder adder_13 (tree_6[0][39], tree_6[1][39], tree_6[2][39], tree_7[1][40], tree_7[0][39]);
    full_adder adder_14 (tree_6[0][40], tree_6[1][40], tree_6[2][40], tree_7[1][41], tree_7[0][40]);
    half_adder adder_15 (tree_6[0][41], tree_6[1][41], tree_7[1][42], tree_7[0][41]);
    half_adder adder_16 (tree_6[0][42], tree_6[1][42], tree_7[1][43], tree_7[0][42]);
    half_adder adder_17 (tree_6[0][43], tree_6[1][43], tree_7[1][44], tree_7[0][43]);
    half_adder adder_18 (tree_6[0][44], tree_6[1][44], tree_7[1][45], tree_7[0][44]);
    half_adder adder_19 (tree_6[0][45], tree_6[1][45], tree_7[1][46], tree_7[0][45]);
    half_adder adder_20 (tree_6[0][46], tree_6[1][46], tree_7[1][47], tree_7[0][46]);
    half_adder adder_21 (tree_6[0][47], tree_6[1][47], tree_7[1][48], tree_7[0][47]);
    half_adder adder_22 (tree_6[0][48], tree_6[1][48], tree_7[1][49], tree_7[0][48]);
    half_adder adder_23 (tree_6[0][49], tree_6[1][49], tree_7[1][50], tree_7[0][49]);
    half_adder adder_24 (tree_6[0][50], tree_6[1][50], tree_7[1][51], tree_7[0][50]);
    half_adder adder_25 (tree_6[0][51], tree_6[1][51], tree_7[1][52], tree_7[0][51]);
    half_adder adder_26 (tree_6[0][52], tree_6[1][52], tree_7[1][53], tree_7[0][52]);
    half_adder adder_27 (tree_6[0][53], tree_6[1][53], tree_7[1][54], tree_7[0][53]);
    half_adder adder_28 (tree_6[0][54], tree_6[1][54], tree_7[1][55], tree_7[0][54]);
    half_adder adder_29 (tree_6[0][55], tree_6[1][55], tree_7[1][56], tree_7[0][55]);
    half_adder adder_30 (tree_6[0][56], tree_6[1][56], tree_7[1][57], tree_7[0][56]);
    half_adder adder_31 (tree_6[0][57], tree_6[1][57], tree_7[1][58], tree_7[0][57]);
    half_adder adder_32 (tree_6[0][58], tree_6[1][58], tree_7[1][59], tree_7[0][58]);
    half_adder adder_33 (tree_6[0][59], tree_6[1][59], tree_7[1][60], tree_7[0][59]);
    half_adder adder_34 (tree_6[0][60], tree_6[1][60], tree_7[1][61], tree_7[0][60]);
    half_adder adder_35 (tree_6[0][61], tree_6[1][61], tree_7[1][62], tree_7[0][61]);
    half_adder adder_36 (tree_6[0][62], tree_6[1][62], tree_7[1][63], tree_7[0][62]);
    half_adder adder_37 (tree_6[0][63], tree_6[1][63], , tree_7[0][63]);

endmodule




module full_adder (a, b, cin, cout, sum);
	
	input a, b, cin;
	output cout, sum;
	
	assign cout = (a & b) | (cin & (a ^ b));
	assign sum = a ^ b ^ cin;
	
endmodule


module half_adder (a, b, cout, sum);
	
	input a, b;
	output cout, sum;
	
	assign cout = a & b;
	assign sum = a ^ b;
	
endmodule
`define BOOTH_M2 3'd1
`define BOOTH_M1 3'd2
`define BOOTH_0 3'd3
`define BOOTH_P1 3'd4
`define BOOTH_P2 3'd5


module radix_4_booth_wallace_multiplier_8b (a, b, sumout, cout);
	
	input [7:0] a, b;
	
	output cout;
	output [15:0] sumout;

    wire [2:0] booth_b [3:0];
	wire [8:0] pp [3:0];
    wire [3:0] sign;



    genvar i;

    generate
        booth_radix_4 booth_recoding_0 (b[1], b[0], 1'b0, booth_b[0]);
        for (i = 2; i < 8; i = i + 2) begin: booth_level
            booth_radix_4 booth_recoding (b[i+1], b[i], b[i-1], booth_b[i/2]);
        end

        for (i = 0; i < 4; i = i + 1) begin: ppgen_level
            ppgen ppgenerator (a, booth_b[i], pp[i], sign[i]);
        end
    endgenerate

    wire [15:0] tree_level_1 [4:0];
    wire [15:0] tree_level_2 [3:0];
    wire [15:0] tree_level_3 [2:0];
    wire [15:0] tree_level_4 [1:0];

    assign tree_level_1 [0] = {~pp[3][8], 1'b1, ~pp[2][8], 1'b1, ~pp[0][8], pp[0][8], pp[0][8], pp[0]};
    assign tree_level_1 [1] = {1'b0, pp[3][8:7], pp[2][8], ~pp[1][8], pp[1], 1'b0, sign[0]};
    assign tree_level_1 [2] = {3'b0, pp[3][6], pp[2][7:0], 1'b0, sign[1], 2'b0};
    assign tree_level_1 [3] = {4'b0, pp[3][5:0], 1'b0, sign[2], 4'b0};
    assign tree_level_1 [4] = {9'b0, sign[3], 6'b0};

    csa_level_1 csa1 (tree_level_1, tree_level_2);
    csa_level_2 csa2 (tree_level_2, tree_level_3);
    csa_level_3 csa3 (tree_level_3, tree_level_4);

    adder cpa (tree_level_4[0], tree_level_4[1], 1'b0, cout, sumout);



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

    input [7:0] a;
    input [2:0] booth_b;

    output reg [8:0] pp;
    output reg sign;

    always @(*) begin
        case (booth_b)
            `BOOTH_M2: begin
                pp = {~a, 1'b1};
                sign = 1'b1;
            end

            `BOOTH_M1: begin
                pp = {~a[7], ~a};
                sign = 1'b1;
            end

            `BOOTH_0: begin
                pp = 9'b0;
                sign = 1'b0;
            end

            `BOOTH_P1: begin
                pp = {a[7], a};
                sign = 1'b0;
            end
            
            `BOOTH_P2: begin
                pp = {a, 1'b0};
                sign = 1'b0;
            end

            default: begin
                pp = 9'b0;
                sign = 1'b0;
            end
        endcase
    end
endmodule


module csa_level_1 (tree_1, tree_2);

    input [15:0] tree_1 [4:0];
    output [15:0] tree_2 [3:0];

    assign tree_2[0][15] = tree_1[0][15];
    assign tree_2[0][11:7] = tree_1[0][11:7];
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
    full_adder adder_7 (tree_1[1][8], tree_1[2][8], tree_1[3][8], tree_2[2][9], tree_2[1][8]);
    full_adder adder_8 (tree_1[1][9], tree_1[2][9], tree_1[3][9], tree_2[2][10], tree_2[1][9]);
    full_adder adder_9 (tree_1[1][10], tree_1[2][10], tree_1[3][10], tree_2[2][11], tree_2[1][10]);
    full_adder adder_10 (tree_1[1][11], tree_1[2][11], tree_1[3][11], tree_2[1][12], tree_2[1][11]);
    full_adder adder_11 (tree_1[0][12], tree_1[1][12], tree_1[2][12], tree_2[1][13], tree_2[0][12]);
    half_adder adder_12 (tree_1[0][13], tree_1[1][13], tree_2[1][14], tree_2[0][13]);
    half_adder adder_13 (tree_1[0][14], tree_1[1][14], tree_2[1][15], tree_2[0][14]);

endmodule


module csa_level_2 (tree_2, tree_3);

    input [15:0] tree_2 [3:0];
    output [15:0] tree_3 [2:0];

    assign tree_3[0][7] = tree_2[0][7];
    assign tree_3[0][3:0] = tree_2[0][3:0];
    assign tree_3[1][3] = tree_2[1][3];
    assign tree_3[1][0] = tree_2[1][0];

    full_adder adder_0 (tree_2[0][4], tree_2[1][4], tree_2[2][4], tree_3[1][5], tree_3[0][4]);
    half_adder adder_1 (tree_2[0][5], tree_2[1][5], tree_3[1][6], tree_3[0][5]);
    full_adder adder_2 (tree_2[0][6], tree_2[1][6], tree_2[2][6], tree_3[2][7], tree_3[0][6]);
    full_adder adder_3 (tree_2[1][7], tree_2[2][7], tree_2[3][7], tree_3[1][8], tree_3[1][7]);
    full_adder adder_4 (tree_2[0][8], tree_2[1][8], tree_2[2][8], tree_3[1][9], tree_3[0][8]);
    full_adder adder_5 (tree_2[0][9], tree_2[1][9], tree_2[2][9], tree_3[1][10], tree_3[0][9]);
    full_adder adder_6 (tree_2[0][10], tree_2[1][10], tree_2[2][10], tree_3[1][11], tree_3[0][10]);
    full_adder adder_7 (tree_2[0][11], tree_2[1][11], tree_2[2][11], tree_3[1][12], tree_3[0][11]);
    half_adder adder_8 (tree_2[0][12], tree_2[1][12], tree_3[1][13], tree_3[0][12]);
    half_adder adder_9 (tree_2[0][13], tree_2[1][13], tree_3[1][14], tree_3[0][13]);
    half_adder adder_10 (tree_2[0][14], tree_2[1][14], tree_3[1][15], tree_3[0][14]);
    half_adder adder_11 (tree_2[0][15], tree_2[1][15], , tree_3[0][15]);

endmodule


module csa_level_3 (tree_3, tree_4);

    input [15:0] tree_3 [2:0];
    output [15:0] tree_4 [1:0];

    assign tree_4[0][6:0] = tree_3[0][6:0];
    assign tree_4[1][6:5] = tree_3[1][6:5];
    assign tree_4[1][3] = tree_3[1][3];
    assign tree_4[1][0] = tree_3[1][0];
    assign tree_4[1][7] = 1'b0;
    assign tree_4[1][4] = 1'b0;
    assign tree_4[1][2:1] = 2'b0;

    full_adder adder_0 (tree_3[0][7], tree_3[1][7], tree_3[2][7], tree_4[1][8], tree_4[0][7]);
    half_adder adder_1 (tree_3[0][8], tree_3[1][8], tree_4[1][9], tree_4[0][8]);
    half_adder adder_2 (tree_3[0][9], tree_3[1][9], tree_4[1][10], tree_4[0][9]);
    half_adder adder_3 (tree_3[0][10], tree_3[1][10], tree_4[1][11], tree_4[0][10]);
    half_adder adder_4 (tree_3[0][11], tree_3[1][11], tree_4[1][12], tree_4[0][11]);
    half_adder adder_5 (tree_3[0][12], tree_3[1][12], tree_4[1][13], tree_4[0][12]);
    half_adder adder_6 (tree_3[0][13], tree_3[1][13], tree_4[1][14], tree_4[0][13]);
    half_adder adder_7 (tree_3[0][14], tree_3[1][14], tree_4[1][15], tree_4[0][14]);
    half_adder adder_8 (tree_3[0][15], tree_3[1][15], tree_4[0][15]);

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
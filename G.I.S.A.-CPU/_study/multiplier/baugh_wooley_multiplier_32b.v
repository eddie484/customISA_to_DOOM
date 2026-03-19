module baugh_wooley_multiplier_32b (a, b, sumout, cout);
	
	input [31:0]a, b;
	
	output cout;
	output [63:0]sumout;

	wire [31:0] pp [31:0];
	wire [31:0] ppm [31:0];
    
	wire [31:0] sum [31:0];
	wire [31:0] c [31:0];

    genvar i;

    generate
        for (i = 0; i < 32; i = i + 1) begin: ppgen_level
            ppgen ppgenerator (a, b[i], pp[i]);
        end

        for (i = 0; i < 31; i = i + 1) begin: ppmod_level
            assign ppm[i] = {~pp[i][31], pp[i][30:0]};
        end
        assign ppm[31] = {pp[31][31], ~pp[31][30:0]};
    endgenerate
	
	
    csa csa_adder_1 (ppm[0], 32'b0, 32'b0, sum[0], c[0]);
	assign sumout[0] = sum[0][0];

    csa csa_adder_2 (ppm[1], {1'b1, sum[0][31:1]}, c[0], sum[1], c[1]);
	assign sumout[1] = sum[1][0];
	
	generate
        for (i = 2; i < 32; i = i + 1) begin: csa_level
            csa csa_adder (ppm[i], {1'b0, sum[i-1][31:1]}, c[i-1], sum[i], c[i]);
				assign sumout[i] = sum[i][0];
        end
	endgenerate


	wire [31:0] fsa_a;

	assign fsa_a = {1'b1, sum[31][31:1]};

	adder fsa (fsa_a, c[31], 1'b0, cout, sumout[63:32]);
endmodule


module csa (pp, sumin, cin, sumout, cout);

    input [31:0] pp, sumin, cin;
    output [31:0] sumout, cout;

    genvar i;
    generate
        for (i = 0; i < 32; i = i + 1) begin: fa_level
            full_adder fulladder (pp[i], sumin[i], cin[i], cout[i], sumout[i]);
        end
    endgenerate

endmodule



module ppgen (a, b, pp);

    input [31:0] a;
    input b;

    output [31:0] pp;

    assign pp = a & {32{b}}; 

endmodule


module full_adder (a, b, cin, cout, sum);
	
	input a, b, cin;
	output cout, sum;
	
	assign cout = (a & b) | (cin & (a ^ b));
	assign sum = a ^ b ^ cin;
	
endmodule
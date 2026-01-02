module array_multiplier_4b (a, b, sumout, cout);
	
	input [3:0]a, b;
	
	output cout;
	output [7:0]sumout;

	wire [3:0] pp [3:0];
	wire [3:0] sum [3:0];
	wire [3:0] c [3:0];

    genvar i;

    generate
        for (i = 0; i < 4; i = i + 1) begin: ppgen_level
            ppgen ppgenerator (a, b[i], pp[i]);
        end
    endgenerate
	
	
    csa adder_1 (pp[0], 4'b0, 4'b0, sum[0], c[0]);
	assign sumout[0] = sum[0][0];
	
	generate
        for (i = 1; i < 4; i = i + 1) begin: csa_level
            csa csa_adder (pp[i], {1'b0, sum[i-1][3:1]}, c[i-1], sum[i], c[i]);
				assign sumout[i] = sum[i][0];
        end
	endgenerate


	wire [3:0] fsa_a;

	assign fsa_a = {1'b0, sum[3][3:1]};

	adder fsa (fsa_a, c[3], 1'b0, cout, sumout[7:4]);
endmodule


module csa (pp, sumin, cin, sumout, cout);

    input [3:0] pp, sumin, cin;
    output [3:0] sumout, cout;

    genvar i;
    generate
        for (i = 0; i < 4; i = i + 1) begin: fa_level
            full_adder fulladder (pp[i], sumin[i], cin[i], cout[i], sumout[i]);
        end
    endgenerate

endmodule



module ppgen (a, b, pp);

    input [3:0] a;
    input b;

    output [3:0] pp;

    assign pp = a & {4{b}}; 

endmodule
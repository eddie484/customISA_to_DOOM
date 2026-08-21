module csa (pp, sumin, cin, sumout, cout);

    input [34:0] pp, sumin, cin;
    (* keep *) output [34:0] sumout, cout;
	 
	 wire [35:0] carry;
	 assign carry[0] = 1'b0;
	 assign cout = carry[34:0];

    genvar i;
    generate
        for (i = 0; i < 35; i = i + 1) begin: fa_level
            full_adder fulladder (pp[i], sumin[i], cin[i], carry[i + 1], sumout[i]);
        end
    endgenerate

endmodule
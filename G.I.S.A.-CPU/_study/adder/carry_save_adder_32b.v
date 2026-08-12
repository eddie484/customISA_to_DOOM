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
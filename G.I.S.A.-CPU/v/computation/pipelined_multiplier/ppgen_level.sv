`define BOOTH_M2 3'd1
`define BOOTH_M1 3'd2
`define BOOTH_0 3'd3
`define BOOTH_P1 3'd4
`define BOOTH_P2 3'd5

module ppgen_level (a, b, signed_sig, pp, sign);
	
	input [31:0] a, b;
   input signed_sig;   // 1: signed mul, 0: unsigned mul.
	
	(* keep *) output [33:0] pp [15:0];
   (* keep *) output [15:0] sign;

	wire [32:0] ext_a;
   (* keep *) wire [2:0] booth_b [15:0];
	
	
	
	assign ext_a = signed_sig ? {a[31], a} : {1'b0, a};

    genvar i;

    generate
        booth_radix_4 booth_recoding_0 (b[1], b[0], 1'b0, booth_b[0]);
        for (i = 2; i < 32; i = i + 2) begin: booth_level
            booth_radix_4 booth_recoding (b[i+1], b[i], b[i-1], booth_b[i/2]);
        end

        for (i = 0; i < 16; i = i + 1) begin: ppgen_level
            ppgen ppgenerator (ext_a, booth_b[i], pp[i], sign[i]);
        end
    endgenerate
endmodule



module booth_radix_4 (x_high, x_low, x_prev, y);

    input x_high, x_low, x_prev;
    (* keep *) output reg [2:0] y;

    (* keep *) wire [2:0] x_in;
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

    input [32:0] a;
    input [2:0] booth_b;

    (* keep *) output reg [33:0] pp;
    (* keep *) output reg sign;

    always @(*) begin
        case (booth_b)
            `BOOTH_M2: begin
                pp = {~a, 1'b1};
                sign = 1'b1;
            end

            `BOOTH_M1: begin
                pp = {~a[32], ~a};
                sign = 1'b1;
            end

            `BOOTH_0: begin
                pp = 33'b0;
                sign = 1'b0;
            end

            `BOOTH_P1: begin
                pp = {a[32], a};
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

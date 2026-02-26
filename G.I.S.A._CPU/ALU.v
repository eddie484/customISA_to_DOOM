module ALU (valA, valB, signed, aluop, mulsel, valE, alucc);

    input [31:0] valA, valB;
    input signed;
    input [3:0] aluop;
    input [1:0] mulsel;

    output [31:0] valE;
    output [3:0] alucc;

    wire isSub, isMod;
    wire [1:0] sftmode;

    wire [31:0] adder_result;
    wire adder_cout;

    wire [63:0] multiplier_result;
    wire [31:0] multiplier_selected;

    wire [31:0] divider_result;

    wire [31:0] shifter_result;
    wire shifter_pushed;

    wire [31:0] and_result, or_result, xor_result, not_result;

    assign isSub = aluop[0];
    assign isMod = aluop[2];
    assign sftmode = aluop[1:0];    // 00: ror, 01: shl, 10: shr, 11: rol


    adder add_module(valA, valB, isSub, adder_cout, adder_result);
    multiplier mul_module(valA, valB, signed, multiplier_result);
    assign divider_result = valA / valB;    // 개발해야함
    shifter sft_module(valA, valB, sftmode, signed, shifter_result, shifter_pushed);

    assign and_result = valA & valB;
    assign or_result = valA | valB;
    assign xor_result = valA ^ valB;
    assign not_result = ~valB;


    always @(*) begin
        case (mulsel)
            2'b00: multiplier_selected = multiplier_result[31:0];
            2'b01: multiplier_selected = multiplier_result[31:0];
            2'b10: multiplier_selected = multiplier_result[47:16];
            default: multiplier_selected = 32'b0;
        endcase

        case (aluop)
            4'b0000: valE = adder_result;
            4'b0001: valE = adder_result;
            4'b0010: valE = multiplier_result;
            4'b0011: valE = divider_result;
            4'b0100: valE = divider_result;
            4'b0101: valE = shifter_result;
            4'b0110: valE = shifter_result;
            4'b0111: valE = shifter_result;
            4'b1000: valE = shifter_result;
            4'b1001: valE = and_result;
            4'b1010: valE = or_result;
            4'b1011: valE = xor_result;
            4'b1100: valE = not_result;
            default: valE = 32'b0;
        endcase
    end


    ccgen cc_result(valA, valB, valE, aluop, adder_cout, multiplier_result, mulsel, shifter_pushed, signed, alucc);

endmodule

module ccgen (valA, valB, valE, aluop, cout, mul, mulsel, sft, signed, alucc);
    input [31:0] valA, valB, valE;
    input [3:0] aluop;
    input [1:0] mulsel;
    input cout, sft, signed;
    input [63:0] mul;

    output [3:0] alucc;


    always @(*) begin
        case(aluop)
            4'b0000: alucc = {valE[31], ~|valE, cout, ((valA[31] == valB[31]) && (valA[31] != valE[31]))};  // ADD
            4'b0001: alucc = {valE[31], ~|valE, cout, ((valA[31] != valB[31]) && (valA[31] != valE[31]))};  // SUB
            4'b0010: alucc = begin  // MUL
                                case (mulsel)
                                    2'b00: alucc = {valE[31], ~|valE, mul[32], |mul[63:32]};  // MUL
                                    2'b01: alucc = {valE[31], ~|valE, 2'b0};  // MULH
                                    2'b10: alucc = {valE[31], ~|valE, |valE[15:0], };  // MULFX
                                    default: alucc = 4'b0;
                                endcase
                            end
            4'b0011: alucc = begin  // DIV
                                case (signed)
                                    2'b00: alucc = {valE[31], ~|valE, ~|valB, ((valA[31] && ~|valA[30:0]) && (&valB))};  // DIV
                                    2'b01: alucc = {valE[31], ~|valE, ~|valB, 1'b0};  // DIVU
                                    default: alucc = 4'b0;
                                endcase
                            end
            4'b0100: alucc = {valE[31], ~|valE, ~|valB, 1'b0};    // MOD
            4'b0101: alucc = {valE[31], ~|valE, sft, 1'b0};    // SHL
            4'b0110: alucc = {valE[31], ~|valE, sft, 1'b0};    // SHR
            4'b0111: alucc = {valE[31], ~|valE, sft, 1'b0};    // ROL
            4'b1000: alucc = {valE[31], ~|valE, sft, 1'b0};    // ROR
            4'b1001: alucc = {valE[31], ~|valE, 2'b0};    // AND
            4'b1010: alucc = {valE[31], ~|valE, 2'b0}; // OR
            4'b1011: alucc = {valE[31], ~|valE, 2'b0};    // XOR
            4'b1100: alucc = {valE[31], ~|valE, 2'b0};    // NOT
            default: alucc = 4'b0;
        endcase
    end

endmodule
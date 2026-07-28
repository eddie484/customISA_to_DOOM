`timescale 1ns / 1ps

module GISA_CPU_tb;

    parameter integer CLK_PERIOD = 10;
    parameter integer RESET_CYCLES = 5;
    parameter integer RUN_CYCLES = 1000;

    reg clk;
    reg nRESET;

    wire [31:0] reg_0;
    wire [31:0] reg_1;
    wire [31:0] reg_2;
    wire [31:0] reg_3;
    wire [31:0] reg_4;
    wire [31:0] reg_5;
    wire [31:0] reg_6;
    wire [31:0] reg_7;
    wire [31:0] reg_8;
    wire [31:0] reg_9;
    wire [31:0] reg_10;
    wire [31:0] reg_11;
    wire [31:0] reg_12;
    wire [31:0] reg_13;
    wire [31:0] reg_14;
    wire [31:0] reg_15;

    integer i;
    integer rf_file;

    always #(CLK_PERIOD / 2) clk = ~clk;

    GISA_CPU myGISA (
        .clk(clk),
        .nRESET(nRESET),
        .reg_0(reg_0),
        .reg_1(reg_1),
        .reg_2(reg_2),
        .reg_3(reg_3),
        .reg_4(reg_4),
        .reg_5(reg_5),
        .reg_6(reg_6),
        .reg_7(reg_7),
        .reg_8(reg_8),
        .reg_9(reg_9),
        .reg_10(reg_10),
        .reg_11(reg_11),
        .reg_12(reg_12),
        .reg_13(reg_13),
        .reg_14(reg_14),
        .reg_15(reg_15)
    );

    initial begin
        clk = 1'b0;
        nRESET = 1'b0;

        $dumpfile("GISA_CPU_tb.vcd");
        $dumpvars(0, GISA_CPU_tb);

        repeat (RESET_CYCLES) @(posedge clk);
        @(negedge clk);
        nRESET = 1'b1;

        repeat (RUN_CYCLES) @(posedge clk);

        dump_registers();

        $finish;
    end

    function [31:0] get_reg;
        input [3:0] index;
        begin
            case (index)
                4'd0:  get_reg = reg_0;
                4'd1:  get_reg = reg_1;
                4'd2:  get_reg = reg_2;
                4'd3:  get_reg = reg_3;
                4'd4:  get_reg = reg_4;
                4'd5:  get_reg = reg_5;
                4'd6:  get_reg = reg_6;
                4'd7:  get_reg = reg_7;
                4'd8:  get_reg = reg_8;
                4'd9:  get_reg = reg_9;
                4'd10: get_reg = reg_10;
                4'd11: get_reg = reg_11;
                4'd12: get_reg = reg_12;
                4'd13: get_reg = reg_13;
                4'd14: get_reg = reg_14;
                4'd15: get_reg = reg_15;
                default: get_reg = 32'h00000000;
            endcase
        end
    endfunction

    task dump_registers;
        begin
            rf_file = $fopen("regfile_out.txt", "w");

            if (rf_file == 0) begin
                $display("ERROR: can't open regfile_out.txt");
            end else begin
                for (i = 0; i < 16; i = i + 1) begin
                    $fdisplay(rf_file, "R%0d = %h", i, get_reg(i[3:0]));
                    $display("R%0d = %h", i, get_reg(i[3:0]));
                end

                $fclose(rf_file);
                $display(">> regfile_out.txt write complete");
            end
        end
    endtask

endmodule
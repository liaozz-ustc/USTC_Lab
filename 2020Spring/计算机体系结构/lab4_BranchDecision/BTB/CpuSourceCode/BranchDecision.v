`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: Huang Yifan (hyf15@mail.ustc.edu.cn)
// 
// Design Name: RV32I Core
// Module Name: Branch Decision
// Tool Versions: Vivado 2017.4.1
// Description: Decide whether to branch
// 
//////////////////////////////////////////////////////////////////////////////////


//  功能说明
    //  判断是否branch
// 输入
    // reg1               寄存器1
    // reg2               寄存器2
    // br_type            branch类型
// 输出
    // br                 是否branch
// 实验要求
    // 补全模块

`include "Parameters.v"   
module BranchDecision(
    input wire [31:0] reg1, reg2,
    input wire [2:0] br_type,
    output reg br
    );

    // TODO: Complete this module
    wire signed [31:0] reg1_signed = $signed(reg1);
    wire signed [31:0] reg2_signed = $signed(reg2);

    always@(*)
    begin
        case (br_type)
            `NOBRANCH: br<=1'b0;
            `BEQ: br <= (reg1 == reg2) ? 1'b1 : 1'b0;
            `BNE: br <= (reg1 != reg2) ? 1'b1 : 1'b0;
            `BLT: br <= (reg1_signed < reg2_signed) ? 1'b1 : 1'b0;
            `BLTU: br <= (reg1 < reg2) ? 1'b1 : 1'b0;
            `BGE: br <= (reg1_signed >= reg2_signed) ? 1'b1 : 1'b0;
            `BGEU: br <= (reg1 >= reg2) ? 1'b1 : 1'b0;
            default: br <= 1'b0;
        endcase
    end
endmodule

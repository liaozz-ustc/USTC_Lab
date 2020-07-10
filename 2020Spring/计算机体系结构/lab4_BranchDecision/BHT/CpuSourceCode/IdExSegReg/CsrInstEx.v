`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC
// Engineer: Liao Zhouzhou (liaozz@mail.ustc.edu.cn)
// 
// Design Name: RV32I Core
// Module Name: CSR_Inst_Ex
// Tool Versions: Vivado 2017.4.1
// Description:CSR instruction Seg Reg for ID\EX
// 
//////////////////////////////////////////////////////////////////////////////////


//  功能说明
    // CSR指令[31:15]的段寄存器
// 输入
    // clk               时钟信号
    // inst[31:15]       CSR指令
    // bubbleE           EX阶段的bubble信号
    // flushE            EX阶段的flush信号
// 输出
    // inst_EX           传给下一流水段的指令



module CSR_Inst_EX(
    input wire clk, bubbleE, flushE,
    input wire [19:0] CSR_inst,
    output reg [19:0] CSR_inst_EX
    );

    initial CSR_inst_EX = 0;
    
    always@(posedge clk)
        if (!bubbleE) 
        begin
            if (flushE)
                CSR_inst_EX <= 0;
            else 
                CSR_inst_EX <= CSR_inst;
        end
    
endmodule
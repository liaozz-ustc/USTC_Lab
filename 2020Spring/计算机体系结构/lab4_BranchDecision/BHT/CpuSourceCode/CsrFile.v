`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC
// Engineer: Liao Zhouzhou (liaozz@mail.ustc.edu.cn)
// 
// Design Name: RV32I Core
// Module Name: CSR Register File
// Tool Versions: Vivado 2017.4.1
// Description: CSR Register File
// 
//////////////////////////////////////////////////////////////////////////////////


//  功能说明
    //  CSR寄存器，提供读写端口（同步写，异步读）
    //  时钟下降沿写入
    //  0号寄存器的值始终为0
// 输入
    // clk               时钟信号
    // rst               寄存器重置信号
    // CSR_write_en      寄存器写使能
    // addr              地址
    // zimm              立即数
    // reg1_data         寄存器1值
    // CSR_funct3        funct3
    // imm_reg_sel       立即数或reg，0为立即数，1为reg值 不需要，有funct3
// 输出
    // CSR_data          读数据
// 实验要求
    // 无需修改

`include "Parameters.v" 
module CsrRegisterFile(
    input wire clk,
    input wire rst,
    input wire CSR_write_en,
    input wire [11:0] addr,
    input wire [4:0] zimm,
    input wire [31:0] reg1_data,
    input wire [2:0] CSR_funct3,
  //  input wire imm_reg_sel,
    output reg [31:0] CSR_data
    );

    reg [31:0] reg_file[1:4095];
    integer i;

    // init register file
    initial
    begin
        for(i = 1; i < 4096; i = i + 1) 
            reg_file[i][31:0] <= 32'b0;
    end

    // write in clk negedge, reset in rst posedge
    // if write register in clk posedge,
    // new wb data also write in clk posedge,
    // so old wb data will be written to register
    always@(negedge clk or posedge rst) 
    begin 
        if (rst)
            for (i = 1; i < 4096; i = i + 1) 
                reg_file[i][31:0] <= 32'b0;
        else if(CSR_write_en && ( addr != 12'h0))
            begin
                CSR_data = (addr == 12'b0) ? 32'h0 : reg_file[addr];
                case (CSR_funct3)
                `CSRRW: reg_file[addr] <= reg1_data;
                `CSRRS: reg_file[addr] <= reg_file[addr] | reg1_data;
                `CSRRC: reg_file[addr] <= reg_file[addr] & (~reg1_data);
                `CSRRWI: reg_file[addr] <= {{27{1'b0}},zimm};
                `CSRRSI: reg_file[addr] <= reg_file[addr] | {{27{1'b0}},zimm};
                `CSRRCI: reg_file[addr] <= reg_file[addr] & {~{{27{1'b0}},zimm}};
                default: reg_file[addr] <= 32'hxxxx_xxxx;
                endcase
            end
    end

    // read data changes when address changes
    //assign CSR_data = (addr == 12'b0) ? 32'h0 : reg_file[addr];错误的写法

/*    always@(posedge clk)
    begin
        CSR_data = (addr == 12'b0) ? 32'h0 : reg_file[addr];
    end
*/

endmodule
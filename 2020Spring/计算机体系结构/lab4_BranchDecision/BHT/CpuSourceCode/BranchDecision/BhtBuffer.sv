
/*
 * @Author: liaozz
 * @Date: 2020-06-11 10:55:12
 * @LastEditTime: 2020-06-11 23:15:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \CALab_Complete\CpuSourceCode\BranchDecision\BtbBuffer.sv
 */ 
//异步读，同步写
 module BhtBuffer #(
    //parameter LINE_ADDR_LEN = 2,// 每一路地址长度，决定了每一路具有4word，即预测的PC为32位
    parameter SET_ADDR_LEN = 12 //直接映射，组地址长度为12，说明有2^12=4096组
    //parameter TAG_PC_LEN = 2,   //PCtag长度
    //parameter STATE_LEN = 1     //标志最后一次执行是否跳转
 )(
    input clk,rst,
    input [31:0] PC_IF,        //输入IF阶段产生的PC，用于比较是否Buffer中是否有对应项
    input isHit_BTB,            //输入BTB是否命中
    output reg isTakenBr_Bht,      //根据PC_IF查找Buffer，预测分支是否跳转

    input wr_req,              //更新Bht_Buffer请求
    input [31:0] PC_EX,        //输入EX阶段的PC，用于更新Buffer表
    input isTakenBr_Ex         //根据是否真实跳转更新Bth表
 );

 //localparam LINE_SIZE = 1 << LINE_ADDR_LEN;
localparam SET_SIZE = 1 << SET_ADDR_LEN;
localparam UNSED_ADDR_LEN = 32 - SET_ADDR_LEN - 2;
 

reg [1:0] state_buffer [SET_SIZE];

wire [2-1:0] word_PC_IF;
wire [SET_ADDR_LEN -1 : 0] set_PC_IF;
wire [UNSED_ADDR_LEN - 1 : 0] unused_PC_IF;


wire [2-1:0] word_PC_EX;
wire [SET_ADDR_LEN -1 : 0] set_PC_EX;
wire [UNSED_ADDR_LEN - 1 : 0] unused_PC_EX;
//分割PC地址
assign {unused_PC_IF, set_PC_IF, word_PC_IF} = PC_IF;
assign {unused_PC_EX, set_PC_EX, word_PC_EX} = PC_EX;

always@(*)begin
    if(isHit_BTB && state_buffer[set_PC_IF][1] == 1'b1 ) begin
        isTakenBr_Bht = 1'b1;
    end
    else begin
        isTakenBr_Bht = 1'b0;
    end
end

always @ (posedge clk or posedge rst) begin
    if(rst) begin
        for(integer i = 0; i < SET_SIZE; i++)begin
            state_buffer[i] <= 2'b11;
        end
    end 
    else begin
        if(wr_req) begin
            case(state_buffer[set_PC_EX])
            2'b11:  begin
                if(!isTakenBr_Ex)begin
                    state_buffer[set_PC_EX] <= 2'b10;
                end
            end
            2'b10:  begin
                if(isTakenBr_Ex)begin
                    state_buffer[set_PC_EX] <= 2'b11;
                end else begin
                    state_buffer[set_PC_EX] <= 2'b00;
                end
            end
            2'b00:begin
                if(isTakenBr_Ex) begin
                    state_buffer[set_PC_EX] <= 2'b01;
                end
            end
            2'b01:begin
                if(isTakenBr_Ex) begin
                    state_buffer[set_PC_EX] <= 2'b11;
                end else begin
                    state_buffer[set_PC_EX] <= 2'b00;
                end
            end
            endcase
        end
    end
end

endmodule
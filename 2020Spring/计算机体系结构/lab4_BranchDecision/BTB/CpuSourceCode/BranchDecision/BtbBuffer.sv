
/*
 * @Author: liaozz
 * @Date: 2020-06-11 10:55:12
 * @LastEditTime: 2020-06-11 15:48:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \CALab_Complete\CpuSourceCode\BranchDecision\BtbBuffer.sv
 */ 
//异步读，同步写
 module BtbBuffer #(
    //parameter LINE_ADDR_LEN = 2,// 每一路地址长度，决定了每一路具有4word，即预测的PC为32位
    parameter SET_ADDR_LEN = 7, //直接映射，组地址长度为7，说明有2^7=128组
    parameter TAG_PC_LEN = 2,   //PCtag长度
    parameter STATE_LEN = 1     //标志最后一次执行是否跳转
 )(
    input clk,rst,
    input [31:0] PC_IF,        //输入IF阶段产生的PC，用于比较是否Buffer中是否有对应项
    output reg isTakenBr_BTB,      //根据PC_IF查找Buffer，预测分支是否跳转
    output reg [31:0] predictedPC, //若预测分支跳转，输出预测跳转地址

    input wr_req,              //更新Buffer请求
    input [31:0] PC_EX,        //输入EX阶段的PC，用于更新Buffer表
    input [31:0] PC_Branch,    //输入EX阶段Branch指令对应的分支地址，用于更新Buffer表
    input isTakenBr_Ex         //更新历史跳转信息
 );

 //localparam LINE_SIZE = 1 << LINE_ADDR_LEN;
localparam SET_SIZE = 1 << SET_ADDR_LEN;
localparam UNSED_ADDR_LEN = 32 - TAG_PC_LEN - SET_ADDR_LEN - 2;
 
reg [31 : 0] predictedPC_buffer[SET_SIZE];
reg [TAG_PC_LEN-1 : 0] tagPC_buffer[SET_SIZE];
reg state_buffer [SET_SIZE];

wire [2-1:0] word_PC_IF;
wire [SET_ADDR_LEN -1 : 0] set_PC_IF;
wire [TAG_PC_LEN - 1 : 0] tag_PC_IF;
wire [UNSED_ADDR_LEN - 1 : 0] unused_PC_IF;


wire [2-1:0] word_PC_EX;
wire [SET_ADDR_LEN -1 : 0] set_PC_EX;
wire [TAG_PC_LEN - 1 : 0] tag_PC_EX;
wire [UNSED_ADDR_LEN - 1 : 0] unused_PC_EX;
//分割PC地址
assign {unused_PC_IF, tag_PC_IF, set_PC_IF, word_PC_IF} = PC_IF;
assign {unused_PC_EX, tag_PC_EX, set_PC_EX, word_PC_EX} = PC_EX;

always@(*)begin
    if(tagPC_buffer[set_PC_IF] == tag_PC_IF && state_buffer[set_PC_IF] == 1)begin
        isTakenBr_BTB = 1'b1;
        predictedPC = predictedPC_buffer[set_PC_IF];
    end
    else begin
        isTakenBr_BTB = 1'b0;
        predictedPC = 0;
    end
end

always @ (posedge clk or posedge rst) begin
    if(rst) begin
        for(integer i = 0; i < SET_SIZE; i++)begin
            predictedPC_buffer[i] <= 0;
            tagPC_buffer[i] <= 0;
            state_buffer[i] <= 0;
        end
    end 
    else begin
        if(wr_req) begin
            tagPC_buffer[set_PC_EX] <= tag_PC_EX;
            state_buffer[set_PC_EX] <= isTakenBr_Ex;
            predictedPC_buffer[set_PC_EX] <= PC_Branch;
        end
    end
end

 endmodule
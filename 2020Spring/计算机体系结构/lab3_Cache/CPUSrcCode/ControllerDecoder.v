`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: USTC ESLAB
// Engineer: Huang Yifan (hyf15@mail.ustc.edu.cn)
// 
// Design Name: RV32I Core
// Module Name: Controller Decoder
// Tool Versions: Vivado 2017.4.1
// Description: Controller Decoder Module
// 
//////////////////////////////////////////////////////////////////////////////////

//  功能说明
    //  对指令进行译码，将其翻译成控制信号，传输给各个部件
// 输入
    // Inst              待译码指令
// 输出
    // jal               jal跳转指令
    // jalr              jalr跳转指令
    // op2_src           ALU的第二个操作数来源。为1时，op2选择imm，为0时，op2选择reg2
    // ALU_func          ALU执行的运算类型
    // br_type           branch的判断条件，可以是不进行branch
    // load_npc          写回寄存器的值的来源（PC或者ALU计算结果）, load_npc == 1时选择PC
    // wb_select         写回寄存器的值的来源（Cache内容或者ALU计算结果），wb_select == 1时选择cache内容
    // load_type         load类型
    // src_reg_en        指令中src reg的地址是否有效，src_reg_en[1] == 1表示reg1被使用到了，src_reg_en[0]==1表示reg2被使用到了
    // reg_write_en      通用寄存器写使能，reg_write_en == 1表示需要写回reg
    // cache_write_en    按字节写入data cache
    // imm_type          指令中立即数类型
    // alu_src1          alu操作数1来源，alu_src1 == 0表示来自reg1，alu_src1 == 1表示来自PC
    // alu_src2          alu操作数2来源，alu_src2 == 2’b00表示来自reg2，alu_src2 == 2'b01表示来自reg2地址，alu_src2 == 2'b10表示来自立即数
// 实验要求
    // 补全模块


`include "Parameters.v"   
module ControllerDecoder(
    input wire [31:0] inst,
    output wire jal,                    //done
    output wire jalr,                   //done
    output wire op2_src,                //done
    output reg [3:0] ALU_func,          //done
    output reg [2:0] br_type,           //done
    output wire load_npc,               //done
    output wire wb_select,              //done
    output reg [2:0] load_type,         //done
    output reg [1:0] src_reg_en,        //done
    output reg reg_write_en,            //done
    output reg [3:0] cache_write_en,    //done
    output wire alu_src1,               //done
    output wire [1:0] alu_src2,         //done
    output reg [2:0] imm_type           //done
    );

    // TODO: Complete this module
    assign jal = (inst[6:0] == 7'b1101111) ? 1 : 0; 
    assign jalr = (inst[6:0] == 7'b1100111) ? 1 : 0;
    assign op2_src = (inst[6:0] == 7'b0110011) ? 0 : 1;         //R-type
    assign load_npc = (inst[6:0] == 7'b1101111 || inst[6:0] == 7'b1100111) ?  1: 0;     //jal jalr
    assign wb_select = (inst[6:0] == 7'b0000011) ? 1: 0;    //load指令来自cache
    assign alu_src1 = (inst[6:0] == 7'b1101111 || inst[6:0] == 7'b0010111) ?  1: 0;
    assign alu_src2 = inst[6:0] == 0110011 ? 2'b00 :
                                             ((inst[6:0] == 7'b0010011 && (inst[14:12] == 3'b001 || inst[14:12] == 3'b101)) ? 2'b01 : 2'b10);
    //这里因为beq等指令有branch decision模块来执行，因此只有R型之林更要用到reg2中的数据
    
    always @(*) begin //alu_fun
        ALU_func = `ADD;//默认为加法
        if (inst[6:0] == 7'b0110011) begin //R-type
            case({inst[31:25], inst[14:12]})
                10'b0000000_000 : ALU_func = `ADD;
                10'b0100000_000 : ALU_func = `SUB;
                10'b0000000_001 : ALU_func = `SLL;
                10'b0000000_010 : ALU_func = `SLT;
                10'b0000000_011 : ALU_func = `SLTU;
                10'b0000000_100 : ALU_func = `XOR;
                10'b0000000_101 : ALU_func = `SRL;
                10'b0000000_110 : ALU_func = `OR;
                10'b0000000_111 : ALU_func = `AND;
                10'b0100000_101 : ALU_func = `SRA;
            endcase
        end
        if (inst[6:0] == 7'b0010011) begin //I-type
            case( inst[14:12] )
                3'b000 : ALU_func = `ADD;
                3'b001 : ALU_func = `SLL;
                3'b010 : ALU_func = `SLT;
                3'b011 : ALU_func = `SLTU;
                3'b100 : ALU_func = `XOR;
                3'b101 : ALU_func = (inst[30] == 1'b0)? `SRL : `SRA;
                3'b110 : ALU_func = `OR;
                3'b111 : ALU_func = `AND;
            endcase
        end
        if(inst[6:0] == 7'b0110111)begin
            ALU_func = `LUI;
        end
    end

    always @(*) begin //imm_type
        imm_type = `RTYPE; //组合逻辑需要一个值。
        case (inst[6:0])
            7'b0110011 : imm_type = `RTYPE;    //R-type
            7'b0010011 : imm_type = `ITYPE;    //I-type
            7'b0000011 : imm_type = `ITYPE;    //load
            7'b1100111 : imm_type = `ITYPE;    //jalr
            7'b1101111 : imm_type = `JTYPE;    //jal
            7'b0110111 : imm_type = `UTYPE;    //LUI
            7'b0010111 : imm_type = `UTYPE;    //AUIPC
            7'b0100011 : imm_type = `STYPE;    //store
            7'b1100011 : imm_type = `BTYPE;    //branch分支指令
        endcase
    end

    always @(*) begin //reg_write_en
        reg_write_en = 0; //不写回
        case (inst[6:0])
            7'b0110011 : reg_write_en = 1;    //R-type
            7'b0010011 : reg_write_en = 1;    //I-type
            7'b0000011 : reg_write_en = 1;    //load
            7'b1100111 : reg_write_en = 1;    //jalr
            7'b1101111 : reg_write_en = 1;    //jal
            7'b0110111 : reg_write_en = 1;    //LUI
            7'b0010111 : reg_write_en = 1;    //AUIPC
            7'b0100011 : reg_write_en = 0;    //store
            7'b1100011 : reg_write_en = 0;    //branch分支指令
        endcase
    end

    always @(*) begin //src_reg_en
        src_reg_en = 2'b11;
        case (inst[6:0])
            7'b0110011 : src_reg_en = 2'b11;    //R-type
            7'b0010011 : src_reg_en = 2'b10;    //I-type
            7'b0000011 : src_reg_en = 2'b10;    //load
            7'b1100111 : src_reg_en = 2'b10;    //jalr
            7'b1101111 : src_reg_en = 2'b00;    //jal
            7'b0110111 : src_reg_en = 2'b00;    //LUI
            7'b0010111 : src_reg_en = 2'b00;    //AUIPC
            7'b0100011 : src_reg_en = 2'b10;    //store
            7'b1100011 : src_reg_en = 2'b11;    //branch分支指令
        endcase
    end

    always@(*) begin //cache_write_en
        cache_write_en = 4'b0000;   //默认不写
        case (inst[6:0])
            7'b0110011 : cache_write_en <= 4'b0000;     //R-type
            7'b0010011 : cache_write_en <= 4'b0000;     //I-type
            7'b0000011 : cache_write_en <= 4'b0000;     //load
            7'b1100111 : cache_write_en <= 4'b0000;     //jalr
            7'b1101111 : cache_write_en <= 4'b0000;     //jal
            7'b0110111 : cache_write_en <= 4'b0000;     //LUI
            7'b0010111 : cache_write_en <= 4'b0000;     //AUIPC
            7'b0100011 :begin                           //store
                            if (inst[14:12] == 3'b010) cache_write_en <= 4'b1111;
                            else if (inst[14:12] == 3'b001) cache_write_en <= 4'b0011;
                            else if (inst[14:12] == 3'b000) cache_write_en <= 4'b0001;
                            else cache_write_en <= 4'b0000;
                        end
            7'b1100011 : cache_write_en <= 4'b0000;     //branch分支指令
        endcase
    end

    always @(*) begin //br_type
        br_type = `NOBRANCH; 
        if (inst[6:0] == 7'b1100011)
            case (inst[14:12])
                3'b000 : br_type = `BEQ;
                3'b001 : br_type = `BNE;
                3'b100 : br_type = `BLT;
                3'b101 : br_type = `BGE;
                3'b110 : br_type = `BLTU;
                3'b111 : br_type = `BGEU;
            endcase
    end

    always @(*) begin //load_type
        load_type = `NOREGWRITE;
        if (inst[6:0] == 7'b0000011)
            case (inst[14:12])
                3'b000 : load_type = `LB;
                3'b001 : load_type = `LH;
                3'b010 : load_type = `LW;
                3'b100 : load_type = `LBU;
                3'b101 : load_type = `LHU;
            endcase
    end


endmodule

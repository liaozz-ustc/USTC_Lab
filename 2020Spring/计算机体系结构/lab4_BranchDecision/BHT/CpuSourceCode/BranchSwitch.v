`include "Parameters.v"   
module BranchSwitch(
    input wire realBr,
    input wire isHit_BTB,
    input wire [2:0] br_type,
    input wire isTakenBr_BHT_EX,

    output reg br,
    output reg br_target_Or_PC4,
    output reg wr_req_btb,
    output reg wr_req_bht
);

always@(*)
begin
    if(realBr != isTakenBr_BHT_EX)begin
       br <= 1;
       if(realBr)begin
           br_target_Or_PC4 <= 1;
       end else begin
           br_target_Or_PC4 <= 0;
       end
    end
    else begin
        br <= 0;
        br_target_Or_PC4 <= 0;
    end
    if(br_type != `NOBRANCH) begin
        wr_req_bht <= 1'b1;
    end else begin 
        wr_req_bht <= 1'b0;
    end
    if(isHit_BTB == 1'b0 && realBr == 1'b1) begin
        wr_req_btb <= 1'b1;
    end else begin 
        wr_req_btb <= 1'b0;
    end
end

endmodule
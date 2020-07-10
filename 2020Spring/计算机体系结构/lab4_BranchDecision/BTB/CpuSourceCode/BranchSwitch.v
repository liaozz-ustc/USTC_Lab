module BranchSwitch(
    input wire realBr,
    input wire isTakenBr_BTB_EX,
    output reg br,
    output reg br_target_Or_PC4,
    output reg wr_req_btb
);

always@(*)
begin
    if(realBr != isTakenBr_BTB_EX)begin
       br <= 1;
       wr_req_btb <= 1;
       if(realBr)begin
           br_target_Or_PC4 <= 1;
       end else begin
           br_target_Or_PC4 <= 0;
       end
    end
    else begin
        br <= 0;
        br_target_Or_PC4 <= 0;
        wr_req_btb <= 0;
    end
end
endmodule
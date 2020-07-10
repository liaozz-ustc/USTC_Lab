module Btb_IF_ID(
    input wire clk, bubbleD, flushD,
    input wire isTakenBr_BTB_IF,
    output reg isTakenBr_BTB_ID
    );

    initial isTakenBr_BTB_ID = 0;
    
    always@(posedge clk)
        if (!bubbleD) 
        begin
            if (flushD)
                isTakenBr_BTB_ID <= 0;
            else 
                isTakenBr_BTB_ID <= isTakenBr_BTB_IF;
        end
    
endmodule
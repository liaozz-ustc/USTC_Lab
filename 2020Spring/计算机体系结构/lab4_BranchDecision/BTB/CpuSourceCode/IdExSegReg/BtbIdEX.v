module Btb_ID_EX(
    input wire clk, bubbleE, flushE,
    input wire  isTakenBr_BTB_ID,
    output reg  isTakenBr_BTB_EX
    );

    initial isTakenBr_BTB_EX = 0;
    
    always@(posedge clk)
        if (!bubbleE) 
        begin
            if (flushE)
                isTakenBr_BTB_EX <= 0;
            else 
                isTakenBr_BTB_EX <= isTakenBr_BTB_ID;
        end
    
endmodule
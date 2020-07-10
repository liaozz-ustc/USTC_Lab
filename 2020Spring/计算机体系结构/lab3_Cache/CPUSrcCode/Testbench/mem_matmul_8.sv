
module mem #(                   // 
    parameter  ADDR_LEN  = 11   // 
) (
    input  clk, rst,
    input  [ADDR_LEN-1:0] addr, // memory address
    output reg [31:0] rd_data,  // data read out
    input  wr_req,
    input  [31:0] wr_data       // data write in
);
localparam MEM_SIZE = 1<<ADDR_LEN;
reg [31:0] ram_cell [MEM_SIZE];

always @ (posedge clk or posedge rst)
    if(rst)
        rd_data <= 0;
    else
        rd_data <= ram_cell[addr];

always @ (posedge clk)
    if(wr_req) 
        ram_cell[addr] <= wr_data;

initial begin
    // dst matrix C
    ram_cell[       0] = 32'h0;  // 32'hf89a72d0;
    ram_cell[       1] = 32'h0;  // 32'h865073ba;
    ram_cell[       2] = 32'h0;  // 32'h4b391f32;
    ram_cell[       3] = 32'h0;  // 32'h09b5ee6a;
    ram_cell[       4] = 32'h0;  // 32'h552be053;
    ram_cell[       5] = 32'h0;  // 32'hf22a0459;
    ram_cell[       6] = 32'h0;  // 32'h130967b8;
    ram_cell[       7] = 32'h0;  // 32'h24050285;
    ram_cell[       8] = 32'h0;  // 32'hdfd8820b;
    ram_cell[       9] = 32'h0;  // 32'h64cfc4a6;
    ram_cell[      10] = 32'h0;  // 32'h0e9be829;
    ram_cell[      11] = 32'h0;  // 32'h4f01ceac;
    ram_cell[      12] = 32'h0;  // 32'h817de14d;
    ram_cell[      13] = 32'h0;  // 32'hfe960e9e;
    ram_cell[      14] = 32'h0;  // 32'h1b975b41;
    ram_cell[      15] = 32'h0;  // 32'h69175c33;
    ram_cell[      16] = 32'h0;  // 32'he0c50e0f;
    ram_cell[      17] = 32'h0;  // 32'hab1d1683;
    ram_cell[      18] = 32'h0;  // 32'he61c28b8;
    ram_cell[      19] = 32'h0;  // 32'hed7fe6ad;
    ram_cell[      20] = 32'h0;  // 32'h46bd4baa;
    ram_cell[      21] = 32'h0;  // 32'h65f31918;
    ram_cell[      22] = 32'h0;  // 32'h7ec4de02;
    ram_cell[      23] = 32'h0;  // 32'h672d5dd6;
    ram_cell[      24] = 32'h0;  // 32'h8d499e7a;
    ram_cell[      25] = 32'h0;  // 32'h2de539f3;
    ram_cell[      26] = 32'h0;  // 32'h4bc3415b;
    ram_cell[      27] = 32'h0;  // 32'h30929ade;
    ram_cell[      28] = 32'h0;  // 32'h08621fed;
    ram_cell[      29] = 32'h0;  // 32'h10837548;
    ram_cell[      30] = 32'h0;  // 32'ha9b2cc38;
    ram_cell[      31] = 32'h0;  // 32'h81c00382;
    ram_cell[      32] = 32'h0;  // 32'he978ebb7;
    ram_cell[      33] = 32'h0;  // 32'hf0780072;
    ram_cell[      34] = 32'h0;  // 32'ha753aa63;
    ram_cell[      35] = 32'h0;  // 32'hdd041619;
    ram_cell[      36] = 32'h0;  // 32'hf331915f;
    ram_cell[      37] = 32'h0;  // 32'h28a61a90;
    ram_cell[      38] = 32'h0;  // 32'hdd54b01a;
    ram_cell[      39] = 32'h0;  // 32'hb4f3fdb8;
    ram_cell[      40] = 32'h0;  // 32'he2522385;
    ram_cell[      41] = 32'h0;  // 32'hb6c5a028;
    ram_cell[      42] = 32'h0;  // 32'hd966d9d4;
    ram_cell[      43] = 32'h0;  // 32'h2ebf8aff;
    ram_cell[      44] = 32'h0;  // 32'ha1e51fdf;
    ram_cell[      45] = 32'h0;  // 32'h0aac00a7;
    ram_cell[      46] = 32'h0;  // 32'h51344a20;
    ram_cell[      47] = 32'h0;  // 32'hc12fe47a;
    ram_cell[      48] = 32'h0;  // 32'h94a03641;
    ram_cell[      49] = 32'h0;  // 32'hf5cd04d9;
    ram_cell[      50] = 32'h0;  // 32'h35be7460;
    ram_cell[      51] = 32'h0;  // 32'hf8c4b749;
    ram_cell[      52] = 32'h0;  // 32'hea05624b;
    ram_cell[      53] = 32'h0;  // 32'h9ed01194;
    ram_cell[      54] = 32'h0;  // 32'h179d0b5c;
    ram_cell[      55] = 32'h0;  // 32'h096c758e;
    ram_cell[      56] = 32'h0;  // 32'hfbcb93c7;
    ram_cell[      57] = 32'h0;  // 32'he5bb2009;
    ram_cell[      58] = 32'h0;  // 32'hd916ba61;
    ram_cell[      59] = 32'h0;  // 32'ha3ec5330;
    ram_cell[      60] = 32'h0;  // 32'h75c3b1ac;
    ram_cell[      61] = 32'h0;  // 32'h0c29b057;
    ram_cell[      62] = 32'h0;  // 32'h028bd2c4;
    ram_cell[      63] = 32'h0;  // 32'hd6a3583d;
    // src matrix A
    ram_cell[      64] = 32'h0d9ea8a7;
    ram_cell[      65] = 32'h3ca1d246;
    ram_cell[      66] = 32'ha61eb85a;
    ram_cell[      67] = 32'h8c0809bf;
    ram_cell[      68] = 32'h7b1512ac;
    ram_cell[      69] = 32'h67a131f4;
    ram_cell[      70] = 32'hd7373f35;
    ram_cell[      71] = 32'h46b29659;
    ram_cell[      72] = 32'hbbe8f24c;
    ram_cell[      73] = 32'hc3cfe66d;
    ram_cell[      74] = 32'h677e5ab5;
    ram_cell[      75] = 32'hdb80dd4c;
    ram_cell[      76] = 32'hcc07ce72;
    ram_cell[      77] = 32'h494c61bd;
    ram_cell[      78] = 32'h0f7ad9f5;
    ram_cell[      79] = 32'h90ff29d5;
    ram_cell[      80] = 32'h961a2d21;
    ram_cell[      81] = 32'hc7ce8317;
    ram_cell[      82] = 32'h05ebe923;
    ram_cell[      83] = 32'h1b81a8a1;
    ram_cell[      84] = 32'h5cd5b908;
    ram_cell[      85] = 32'h11dba962;
    ram_cell[      86] = 32'heecb5152;
    ram_cell[      87] = 32'h261e6436;
    ram_cell[      88] = 32'hd5062d0e;
    ram_cell[      89] = 32'h2a92a58b;
    ram_cell[      90] = 32'h4e8367bf;
    ram_cell[      91] = 32'h6134876e;
    ram_cell[      92] = 32'hafcfebf1;
    ram_cell[      93] = 32'ha8c1c319;
    ram_cell[      94] = 32'h4ee31f87;
    ram_cell[      95] = 32'h07687587;
    ram_cell[      96] = 32'h13383fd1;
    ram_cell[      97] = 32'h7db56357;
    ram_cell[      98] = 32'h7bd593cd;
    ram_cell[      99] = 32'h2adc2af2;
    ram_cell[     100] = 32'h4fc7e5f3;
    ram_cell[     101] = 32'h206f3d21;
    ram_cell[     102] = 32'h27cd600d;
    ram_cell[     103] = 32'h480c499c;
    ram_cell[     104] = 32'h7e6fcbcb;
    ram_cell[     105] = 32'h5eaec21a;
    ram_cell[     106] = 32'h8363ab62;
    ram_cell[     107] = 32'hc324718b;
    ram_cell[     108] = 32'h2cc6e102;
    ram_cell[     109] = 32'h519c8d89;
    ram_cell[     110] = 32'h580841fa;
    ram_cell[     111] = 32'h9b1949f5;
    ram_cell[     112] = 32'h0d5e4f54;
    ram_cell[     113] = 32'h7c670da5;
    ram_cell[     114] = 32'h80ee343d;
    ram_cell[     115] = 32'h4b2c5820;
    ram_cell[     116] = 32'h5f09205e;
    ram_cell[     117] = 32'h45a5b52b;
    ram_cell[     118] = 32'he97492c9;
    ram_cell[     119] = 32'h2dcbaeb0;
    ram_cell[     120] = 32'h5b0490bb;
    ram_cell[     121] = 32'h7fc198ac;
    ram_cell[     122] = 32'h9d872063;
    ram_cell[     123] = 32'h8400e39c;
    ram_cell[     124] = 32'he9d9b662;
    ram_cell[     125] = 32'h3d54d9a9;
    ram_cell[     126] = 32'h5e877c89;
    ram_cell[     127] = 32'h0a7cd384;
    // src matrix B
    ram_cell[     128] = 32'h8d31738b;
    ram_cell[     129] = 32'h4bb3feea;
    ram_cell[     130] = 32'h57bffd51;
    ram_cell[     131] = 32'h7af804ec;
    ram_cell[     132] = 32'h0f503a7a;
    ram_cell[     133] = 32'h9da942d0;
    ram_cell[     134] = 32'hbb45f90b;
    ram_cell[     135] = 32'h138c8bfd;
    ram_cell[     136] = 32'h8c4344e9;
    ram_cell[     137] = 32'h0fecdf7a;
    ram_cell[     138] = 32'hae39e988;
    ram_cell[     139] = 32'hed70203d;
    ram_cell[     140] = 32'h44c283fb;
    ram_cell[     141] = 32'h57c51633;
    ram_cell[     142] = 32'hecc05f87;
    ram_cell[     143] = 32'h5cf442ce;
    ram_cell[     144] = 32'hacd4f17f;
    ram_cell[     145] = 32'h213a4faf;
    ram_cell[     146] = 32'h8edbfd70;
    ram_cell[     147] = 32'h2be7ded3;
    ram_cell[     148] = 32'hddd88abe;
    ram_cell[     149] = 32'hd959fa4b;
    ram_cell[     150] = 32'h94d50827;
    ram_cell[     151] = 32'hebecafef;
    ram_cell[     152] = 32'h16477eee;
    ram_cell[     153] = 32'hbf58d1de;
    ram_cell[     154] = 32'h0efd017b;
    ram_cell[     155] = 32'hf8e7f153;
    ram_cell[     156] = 32'hd619ce81;
    ram_cell[     157] = 32'hd375e590;
    ram_cell[     158] = 32'h20c8cfff;
    ram_cell[     159] = 32'h3e37e056;
    ram_cell[     160] = 32'hc5408b31;
    ram_cell[     161] = 32'hdbc5f75b;
    ram_cell[     162] = 32'h8b28e426;
    ram_cell[     163] = 32'h161fc9f4;
    ram_cell[     164] = 32'hab444b14;
    ram_cell[     165] = 32'h132be5a6;
    ram_cell[     166] = 32'hb2bcaef1;
    ram_cell[     167] = 32'h79ef03e9;
    ram_cell[     168] = 32'h70b97986;
    ram_cell[     169] = 32'h6c25aeed;
    ram_cell[     170] = 32'h60b6f571;
    ram_cell[     171] = 32'h42d2b6a3;
    ram_cell[     172] = 32'h95b747ea;
    ram_cell[     173] = 32'hafd79c3b;
    ram_cell[     174] = 32'h3db27e19;
    ram_cell[     175] = 32'hdb5c4a70;
    ram_cell[     176] = 32'h5be11393;
    ram_cell[     177] = 32'h28742649;
    ram_cell[     178] = 32'h3f1bba09;
    ram_cell[     179] = 32'hd2f29348;
    ram_cell[     180] = 32'hc475cb70;
    ram_cell[     181] = 32'h5304e3e9;
    ram_cell[     182] = 32'h01ac592d;
    ram_cell[     183] = 32'h75548aa3;
    ram_cell[     184] = 32'hfbb733f6;
    ram_cell[     185] = 32'h8daddec7;
    ram_cell[     186] = 32'h94efd317;
    ram_cell[     187] = 32'h3438581b;
    ram_cell[     188] = 32'hd6a9d6e2;
    ram_cell[     189] = 32'h62872528;
    ram_cell[     190] = 32'h1894183f;
    ram_cell[     191] = 32'h67ba83a3;
end

endmodule


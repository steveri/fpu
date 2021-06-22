// 0 for add, 1 for sub (matches designware why not)
`define ADD 1'b0
`define SUB 1'b1


module SUB (
   input logic [31:0]  a,
   input logic [31:0]  b,
   output logic [31:0] z
);
   // sradd ADD(.a(a), .b(b), .z(z));
   addsub ADDSUB(.op(`SUB), .a(a), .b(b), .z(z));

endmodule

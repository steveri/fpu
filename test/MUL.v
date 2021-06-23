module MUL (
   input logic [31:0]  a,
   input logic [31:0]  b,
   output logic [31:0] z
);
   // sradd ADD(.a(a), .b(b), .z(z));
   srmul MUL(.a(a), .b(b), .z(z));

endmodule

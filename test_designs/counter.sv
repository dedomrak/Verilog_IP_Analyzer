// DC, 17-Jan-2014
`timescale  1 ns / 1 ps
module counter 
# (
   parameter pfi_pflash_size_g        = 4,     // Physical size of local PFlash bank in megabytes
   parameter pfi_num_drlb_g           = 1,     // Number of DRLB's to instantiate (0 .. 1)
   parameter pfi_num_fpb_g            = 4     // Number of FPB's to instantiate (2 .. 8)
   )
   (
  input  sclk,
  input [1:0] abus,
  //inout tri [7:0] dbus,
  input wrq,
  input rdq,
  input csq,
  input [3:0] gate,
  input [3:0] clk,

  output reg [3:0] cout,
  output [7:0] qbus,
  
  output analog_t qbusr,
  input logic [pfi_num_fpb_g-1:0]    local_pmbi_tag_id_i,
  input analog_t tir
);

//  real qbusr_const;
//  real tir_const;
    real qbusr_int;
  
//  tri [7:0] dbus;  
  reg [7:0] rdbus;
  reg [7:0] rqbus;
//  reg cout3;
//  reg cout2;
//  reg cout1;
//  reg cout0;
//  reg [3:0] cout;
  integer dbs[3:0], db[3:0], ad;
  integer i;
  

  initial begin
    ad = 0;
    for (i=0; i<4; i=i+1)
      db[i] = 'hff;
  end // initial

  always @(negedge rdq) begin
    ad = abus;
    if (csq == 0) begin
      rdbus = #20 db[ad];
    end
    else begin
      rdbus = #5 8'bz;
    end
  end

  always @(posedge rdq) begin
    rdbus = #10 8'bz;
  end

  always @(negedge wrq) begin
    ad = abus;
  end

  always @(posedge wrq) begin
    if (csq == 0) begin
      db[ad] = dbus;
      dbs[ad]= dbus;
      rqbus[7:0] = #15 db[ad];
      qbusr_int = #15 real'(dbus);
    end
    else begin
      rdbus[7:0] = #5 8'bz;
      //qbusr_int = #15 `wrealZState;
    end
  end

  always @(posedge clk[3]) begin
    if (gate[3] == 1) begin
      db[3] = db[3] - 1;
    end
  end
  always @(negedge clk[3]) begin
    if (db[3] == 0) begin
      db[3] = dbs[3];
      #10 cout[3] = 1;
    end
    else begin
      #10 cout[3] = 0;
    end
  end

  always @(posedge clk[2]) begin
    if (gate[2] == 1) begin
      db[2] = db[2] - 1;
    end
  end
  always @(negedge clk[2]) begin
    if (db[2] == 0) begin
      db[2] = dbs[2];
      #10 cout[2] = 1;
    end
    else begin
      #10 cout[2] = 0;
    end
  end

  always @(posedge clk[1]) begin
    if (gate[1] == 1) begin
      db[1] = db[1] - 1;
    end
  end
  always @(negedge clk[1]) begin
    if (db[1] == 0) begin
      db[1] = dbs[1];
      #10 cout[1] = 1;
    end
    else begin
      #10 cout[1] = 0;
    end
  end

  always @(posedge clk[0]) begin
    //tir += 0.1;
    if (gate[0] == 1) begin
      db[0] = db[0] - 1;
    end
  end
  always @(negedge clk[0]) begin
    if (db[0] == 0) begin
      db[0] = dbs[0];
      #10 cout[0] = 1;
    end
    else begin
      #10 cout[0] = 0;
    end
  end


  assign dbus = rdbus;
  assign qbus = rqbus; 
  
  
  // propagate the INPUT signal from TIR to the OUTPUT signal QBUSR
  
        
    always @(tir)
        qbusr_int = #3 tir;
        
    assign qbusr = qbusr_int;
    
    always @(qbus)
        qbusr_int = real'(qbus);
  
    initial
        qbusr_int = `wrealZState;

  
endmodule



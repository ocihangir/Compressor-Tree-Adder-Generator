(* RLOC_ORIGIN = "X0Y0" *)
module mult_12x12_lut6(in0, in1, mult_out);

input [11:0] in0;
input [11:0] in1;
output [24:0] mult_out;

wire [11:0] pp0;
assign pp0 = (in1[0]) ? in0 : 1'b0;
wire [11:0] pp1;
assign pp1 = (in1[1]) ? in0 : 1'b0;
wire [11:0] pp2;
assign pp2 = (in1[2]) ? in0 : 1'b0;
wire [11:0] pp3;
assign pp3 = (in1[3]) ? in0 : 1'b0;
wire [11:0] pp4;
assign pp4 = (in1[4]) ? in0 : 1'b0;
wire [11:0] pp5;
assign pp5 = (in1[5]) ? in0 : 1'b0;
wire [11:0] pp6;
assign pp6 = (in1[6]) ? in0 : 1'b0;
wire [11:0] pp7;
assign pp7 = (in1[7]) ? in0 : 1'b0;
wire [11:0] pp8;
assign pp8 = (in1[8]) ? in0 : 1'b0;
wire [11:0] pp9;
assign pp9 = (in1[9]) ? in0 : 1'b0;
wire [11:0] pp10;
assign pp10 = (in1[10]) ? in0 : 1'b0;
wire [11:0] pp11;
assign pp11 = (in1[11]) ? in0 : 1'b0;
wire [2:0] gpcOutL1_0;

(* RLOC = "X1Y0" *)
gpc006 gpcL1_0 ({pp0[11],pp1[10],pp2[9],pp3[8],pp4[7],pp5[6]},gpcOutL1_0);
wire [2:0] gpcOutL1_1;

(* RLOC = "X1Y1" *)
gpc006 gpcL1_1 ({pp0[10],pp1[9],pp2[8],pp3[7],pp4[6],pp5[5]},gpcOutL1_1);
wire [2:0] gpcOutL1_2;

(* RLOC = "X1Y2" *)
gpc006 gpcL1_2 ({pp1[11],pp2[10],pp3[9],pp4[8],pp5[7],pp6[6]},gpcOutL1_2);
wire [2:0] gpcOutL1_3;

(* RLOC = "X1Y3" *)
gpc006 gpcL1_3 ({pp0[9],pp1[8],pp2[7],pp3[6],pp4[5],pp5[4]},gpcOutL1_3);
wire [2:0] gpcOutL1_4;

(* RLOC = "X1Y4" *)
gpc006 gpcL1_4 ({pp2[11],pp3[10],pp4[9],pp5[8],pp6[7],pp7[6]},gpcOutL1_4);
wire [2:0] gpcOutL1_5;

(* RLOC = "X1Y5" *)
gpc006 gpcL1_5 ({pp0[8],pp1[7],pp2[6],pp3[5],pp4[4],pp5[3]},gpcOutL1_5);
wire [2:0] gpcOutL1_6;

(* RLOC = "X1Y6" *)
gpc006 gpcL1_6 ({pp3[11],pp4[10],pp5[9],pp6[8],pp7[7],pp8[6]},gpcOutL1_6);
wire [2:0] gpcOutL1_7;

(* RLOC = "X1Y7" *)
gpc006 gpcL1_7 ({pp0[7],pp1[6],pp2[5],pp3[4],pp4[3],pp5[2]},gpcOutL1_7);
wire [2:0] gpcOutL1_8;

(* RLOC = "X1Y8" *)
gpc006 gpcL1_8 ({pp4[11],pp5[10],pp6[9],pp7[8],pp8[7],pp9[6]},gpcOutL1_8);
wire [2:0] gpcOutL1_9;

(* RLOC = "X1Y9" *)
gpc006 gpcL1_9 ({pp0[6],pp1[5],pp2[4],pp3[3],pp4[2],pp5[1]},gpcOutL1_9);
wire [2:0] gpcOutL1_10;

(* RLOC = "X1Y10" *)
gpc006 gpcL1_10 ({pp5[11],pp6[10],pp7[9],pp8[8],pp9[7],pp10[6]},gpcOutL1_10);
wire [2:0] gpcOutL1_11;

(* RLOC = "X1Y11" *)
gpc006 gpcL1_11 ({pp0[5],pp1[4],pp2[3],pp3[2],pp4[1],pp5[0]},gpcOutL1_11);
wire [2:0] gpcOutL1_12;

(* RLOC = "X1Y12" *)
gpc006 gpcL1_12 ({pp6[5],pp7[4],pp8[3],pp9[2],pp10[1],pp11[0]},gpcOutL1_12);
wire [2:0] gpcOutL1_13;

(* RLOC = "X1Y13" *)
gpc006 gpcL1_13 ({pp6[11],pp7[10],pp8[9],pp9[8],pp10[7],pp11[6]},gpcOutL1_13);
wire [2:0] gpcOutL1_14;

(* RLOC = "X1Y14" *)
gpc005 gpcL1_14 ({pp0[4],pp1[3],pp2[2],pp3[1],pp4[0]},gpcOutL1_14);
wire [2:0] gpcOutL1_15;

(* RLOC = "X1Y15" *)
gpc005 gpcL1_15 ({pp6[4],pp7[3],pp8[2],pp9[1],pp10[0]},gpcOutL1_15);
wire [2:0] gpcOutL1_16;

(* RLOC = "X1Y16" *)
gpc015 gpcL1_16 ({pp8[5],pp7[5],pp8[4],pp9[3],pp10[2],pp11[1]},gpcOutL1_16);
wire [2:0] gpcOutL1_17;

(* RLOC = "X1Y17" *)
gpc015 gpcL1_17 ({pp8[11],pp7[11],pp8[10],pp9[9],pp10[8],pp11[7]},gpcOutL1_17);
wire [2:0] gpcOutL1_18;

(* RLOC = "X1Y18" *)
gpc023 gpcL1_18 ({pp0[3],pp1[2],pp0[2],pp1[1],pp2[0]},gpcOutL1_18);
wire [2:0] gpcOutL1_19;

(* RLOC = "X1Y19" *)
gpc023 gpcL1_19 ({pp6[3],pp7[2],pp6[2],pp7[1],pp8[0]},gpcOutL1_19);
wire [2:0] gpcOutL1_20;

(* RLOC = "X1Y20" *)
gpc023 gpcL1_20 ({pp9[5],pp10[4],pp9[4],pp10[3],pp11[2]},gpcOutL1_20);
wire [2:0] gpcOutL1_21;

(* RLOC = "X1Y21" *)
gpc023 gpcL1_21 ({pp9[11],pp10[10],pp9[10],pp10[9],pp11[8]},gpcOutL1_21);
wire [2:0] gpcOutL2_0;

(* RLOC = "X2Y0" *)
gpc006 gpcL2_0 ({gpcOutL1_3[0],gpcOutL1_5[1],gpcOutL1_7[2],gpcOutL1_19[1],pp8[1],pp9[0]},gpcOutL2_0);
wire [2:0] gpcOutL2_1;

(* RLOC = "X2Y1" *)
gpc006 gpcL2_1 ({gpcOutL1_0[1],gpcOutL1_1[2],gpcOutL1_2[0],gpcOutL1_12[1],gpcOutL1_15[2],gpcOutL1_16[0]},gpcOutL2_1);
wire [2:0] gpcOutL2_2;

(* RLOC = "X2Y2" *)
gpc006 gpcL2_2 ({gpcOutL1_0[2],gpcOutL1_2[1],gpcOutL1_4[0],gpcOutL1_12[2],gpcOutL1_16[1],gpcOutL1_20[0]},gpcOutL2_2);
wire [2:0] gpcOutL2_3;

(* RLOC = "X2Y3" *)
gpc006 gpcL2_3 ({gpcOutL1_2[2],gpcOutL1_4[1],gpcOutL1_6[0],gpcOutL1_16[2],gpcOutL1_20[1],pp11[3]},gpcOutL2_3);
wire [2:0] gpcOutL2_4;

(* RLOC = "X2Y4" *)
gpc006 gpcL2_4 ({gpcOutL1_4[2],gpcOutL1_6[1],gpcOutL1_8[0],gpcOutL1_20[2],pp10[5],pp11[4]},gpcOutL2_4);
wire [2:0] gpcOutL2_5;

(* RLOC = "X2Y5" *)
gpc015 gpcL2_5 ({gpcOutL1_5[0],gpcOutL1_7[0],gpcOutL1_9[1],gpcOutL1_11[2],pp6[1],pp7[0]},gpcOutL2_5);
wire [2:0] gpcOutL2_6;

(* RLOC = "X2Y6" *)
gpc015 gpcL2_6 ({gpcOutL1_0[0],gpcOutL1_1[0],gpcOutL1_3[1],gpcOutL1_5[2],gpcOutL1_15[0],gpcOutL1_19[2]},gpcOutL2_6);
wire [3:0] gpcOutL2_7;

(* RLOC = "X2Y7" *)
gpc042 gpcL2_7 ({gpcOutL1_9[0],gpcOutL1_11[1],gpcOutL1_14[2],pp6[0],gpcOutL1_11[0],gpcOutL1_14[1]},gpcOutL2_7);
wire [2:0] gpcOutL2_8;

(* RLOC = "X2Y8" *)
gpc004 gpcL2_8 ({gpcOutL1_1[1],gpcOutL1_3[2],gpcOutL1_12[0],gpcOutL1_15[1]},gpcOutL2_8);
wire [2:0] gpcOutL2_9;

(* RLOC = "X2Y9" *)
gpc014 gpcL2_9 ({gpcOutL1_8[2],gpcOutL1_6[2],gpcOutL1_8[1],gpcOutL1_10[0],pp11[5]},gpcOutL2_9);
wire [2:0] gpcOutL2_10;

(* RLOC = "X2Y10" *)
gpc023 gpcL2_10 ({gpcOutL1_14[0],gpcOutL1_18[2],gpcOutL1_18[1],pp2[1],pp3[0]},gpcOutL2_10);
wire [2:0] gpcOutL2_11;

(* RLOC = "X2Y11" *)
gpc023 gpcL2_11 ({gpcOutL1_13[2],gpcOutL1_17[1],gpcOutL1_10[2],gpcOutL1_13[1],gpcOutL1_17[0]},gpcOutL2_11);
wire [2:0] gpcOutL2_12;

(* RLOC = "X2Y12" *)
gpc023 gpcL2_12 ({gpcOutL1_21[2],pp10[11],gpcOutL1_17[2],gpcOutL1_21[1],pp11[9]},gpcOutL2_12);
wire [2:0] gpcOutL3_0;

(* RLOC = "X3Y0" *)
gpc015 gpcL3_0 ({gpcOutL2_0[0],gpcOutL2_5[1],gpcOutL2_7[3],gpcOutL1_7[1],gpcOutL1_9[2],gpcOutL1_19[0]},gpcOutL3_0);
wire [2:0] gpcOutL3_1;

(* RLOC = "X3Y1" *)
gpc014 gpcL3_1 ({gpcOutL2_9[2],gpcOutL2_4[2],gpcOutL2_9[1],gpcOutL1_10[1],gpcOutL1_13[0]},gpcOutL3_1);
wire [2:0] gpcOutL3_2;

(* RLOC = "X3Y2" *)
gpc023 gpcL3_2 ({gpcOutL2_1[0],gpcOutL2_6[2],gpcOutL2_0[2],gpcOutL2_6[1],gpcOutL2_8[0]},gpcOutL3_2);
wire [2:0] gpcOutL3_3;

(* RLOC = "X3Y3" *)
gpc023 gpcL3_3 ({gpcOutL2_1[2],gpcOutL2_2[1],gpcOutL2_1[1],gpcOutL2_2[0],gpcOutL2_8[2]},gpcOutL3_3);
wire [2:0] gpcOutL3_4;

(* RLOC = "X3Y4" *)
gpc023 gpcL3_4 ({gpcOutL2_3[2],gpcOutL2_4[1],gpcOutL2_2[2],gpcOutL2_3[1],gpcOutL2_4[0]},gpcOutL3_4);
wire [3:0] gpcOutL3_5;

(* RLOC = "X3Y5" *)
gpc212 gpcL3_5 ({gpcOutL2_5[0],gpcOutL2_7[2],gpcOutL2_7[1],gpcOutL2_7[0],gpcOutL2_10[2]},gpcOutL3_5);
wire [3:0] gpcOutL3_6;

(* RLOC = "X3Y6" *)
gpc222 gpcL3_6 ({gpcOutL2_12[1],pp11[10],gpcOutL2_11[2],gpcOutL2_12[0],gpcOutL2_11[1],gpcOutL1_21[0]},gpcOutL3_6);
wire [3:0] gpcOutL4_0;

(* RLOC = "X4Y0" *)
gpc213 gpcL4_0 ({gpcOutL3_2[1],gpcOutL2_8[1],gpcOutL3_2[0],gpcOutL3_0[2],gpcOutL2_0[1],gpcOutL2_6[0]},gpcOutL4_0);
wire [2:0] gpcOutL4_1;

(* RLOC = "X4Y1" *)
gpc022 gpcL4_1 ({gpcOutL3_0[1],gpcOutL2_5[2],gpcOutL3_0[0],gpcOutL3_5[3]},gpcOutL4_1);
wire [3:0] gpcOutL4_2;

(* RLOC = "X4Y2" *)
gpc222 gpcL4_2 ({gpcOutL3_3[2],gpcOutL3_4[0],gpcOutL3_3[1],gpcOutL2_3[0],gpcOutL3_2[2],gpcOutL3_3[0]},gpcOutL4_2);
wire [3:0] gpcOutL4_3;

(* RLOC = "X4Y3" *)
gpc222 gpcL4_3 ({gpcOutL3_1[1],gpcOutL2_11[0],gpcOutL3_1[0],gpcOutL3_4[2],gpcOutL3_4[1],gpcOutL2_9[0]},gpcOutL4_3);
wire [2:0] gpcOutL5_0;

(* RLOC = "X5Y0" *)
gpc013 gpcL5_0 ({gpcOutL3_6[1],gpcOutL4_3[3],gpcOutL3_1[2],gpcOutL3_6[0]},gpcOutL5_0);
wire [2:0] gpcOutL6_0;

(* RLOC = "X6Y0" *)
gpc022 gpcL6_0 ({gpcOutL3_6[3],gpcOutL2_12[2],gpcOutL5_0[2],gpcOutL3_6[2]},gpcOutL6_0);
wire [23:0] adderIn0;
wire [23:0] adderIn1;
wire [24:0] adderOut;

assign adderIn0 = {gpcOutL6_0[2],gpcOutL6_0[1],gpcOutL6_0[0],gpcOutL5_0[1],gpcOutL5_0[0],gpcOutL4_3[2],gpcOutL4_3[1],gpcOutL4_2[3],gpcOutL4_2[2],gpcOutL4_2[1],gpcOutL4_0[3],gpcOutL4_0[2],gpcOutL4_0[1],gpcOutL4_0[0],gpcOutL4_1[1],gpcOutL4_1[0],gpcOutL3_5[2],gpcOutL3_5[1],gpcOutL3_5[0],gpcOutL2_10[1],gpcOutL2_10[0],gpcOutL1_18[0],pp0[1],pp0[0]};
assign adderIn1 = {1'b0,pp11[11],1'b0,1'b0,1'b0,1'b0,1'b0,gpcOutL4_3[0],1'b0,1'b0,gpcOutL4_2[0],1'b0,1'b0,gpcOutL4_1[2],1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,pp1[0],1'b0};
assign adderOut = adderIn0 + adderIn1;
assign mult_out = adderOut;

endmodule

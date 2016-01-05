# Compressor-Tree-Adder-Generator

This project is an implementation of an MxN Verilog multiplier module generator. CTAGenerator creates mult_MxN_lut6.v multiplier module and gpc_modules.v Generalized Parallel Counter (GPC) library.

It is designed to take advantage of 6 input LUTs of modern FPGAs to create high performance multipliers. 

An OpenCV based visualizer added to the project. It now generates a jpg file to visualize compression by GPCs.

Tha basic usage ./CTAGenerator M N k
where:
M : multiplier
N : multiplicand
k : final adder input size

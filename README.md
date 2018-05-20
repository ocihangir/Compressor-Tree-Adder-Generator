# Compressor-Tree-Adder-Generator

This project is an implementation of an MxN Verilog multiplier module generator. CTAGenerator creates mult_MxN_lut6.v multiplier module and gpc_modules.v Generalized Parallel Counter (GPC) library.

It is designed to take advantage of 6 input LUTs of modern FPGAs to create high performance multipliers. 

Tha basic usage ./CTAGenerator M N k
where:
M : multiplier (e.g. 8)
N : multiplicand (e.g. 8)
k : final adder input size (e.g. 9)

./CTAGenerator 8 8 9

Build :

It uses CMake to build. In the top folder :
mkdir build
cd build
cmake ..
cmake build ..
make

./CTAGenerator 8 8 9

See mult_8x8_lut6.v and gpc_modules.v files.



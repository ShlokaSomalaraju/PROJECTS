main.c converts Machine code(only 0's and 1's) into RiSC-V Instructions acting as a RISC-V Disassembler.

main.c  :  Main code file (containing the C code to convert the Machine level code into 
           disassembled code).

Example Input files : input.txt , input1.txt, input2.txt (To check all the corner case)

The Input file contains 8 hex digits (without 0x), resembling a 32 bit instruction each in a line.

If the input file is not in the same directory as the main.c file, then give the entire path of the file from root directory in the code file. 

By default, the input file I am taking is "input.txt".

Now to compile the file main.c ,open a terminal and go to the directory of the code file and type
"gcc main.c -lm"

Execution command : "./a.out"

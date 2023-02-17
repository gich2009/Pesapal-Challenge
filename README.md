# Instruction Set for the Assembler

This project involves creating an assembler and a simulator for a simple machine with the following instruction set:

| Opcode | Instruction | Description |
| ------ | ----------- | ----------- |
| 0x00   | halt        | Terminate program |
| 0x01   | nop         | Do nothing |
| 0x02   | li          | Load Immediate: `li R1 0x00000000` Load `0x00000000` into `R1` |
| 0x03   | lw          | Load Word: `lw R1 R2` Load the contents of the memory location pointed to by `R2` into `R1` |
| 0x04   | sw          | Store Word: `sw R1 R2` Store the contents of `R2` in the memory location pointed to by `R1` |
| 0x05   | add         | Add: `add R3 R1 R2` Add `R1` to `R2` and store the result in `R3` |
| 0x06   | sub         | Subtract: `sub R3 R1 R2` Subtract `R2` from `R1` and store the result in `R3` |
| 0x07   | mult        | Multiply: `mult R3 R1 R2` Multiply `R1` by `R2` and store the result in `R3` |
| 0x08   | div         | Divide: `div R3 R1 R2` Divide `R1` by `R2` and store the result in `R3` |
| 0x09   | j           | Unconditional Jump: `j 0x00000000` Jump to memory location `0x00000000` |
| 0x0A   | jr          | Unconditional Jump (Register): `jr R1` Jump to memory location stored in `R1` |
| 0x0B   | beq         | Branch if Equal: `beq R1 R2 R3` Branch to memory location stored in `R3` if `R1` and `R2` are equal |
| 0x0C   | bne         | Branch if Not Equal: `bne R1 R2 R3` Branch to memory location stored in `R3` if `R1` and `R2` are not equal |
| 0x0D   | inc         | Increment Register: `inc R1` Increment `R1` |
| 0x0E   | dec         | Decrement Register: `dec R1` Decrement `R1` |

## Assembler

The assembler takes a text assembly program and produces the program as a set of 32-bit numbers.

## Simulator

The simulator takes the output file of the assembler and executes it. Programs are loaded from `0x0000CFFF` to the end of memory, so that any memory before that is usable by the programmer.

The simulator logs the register values after every cycle (upon execution of every instruction).

For example, the following program should be able to be run using the simulator:

; a simple counter program.
li R1 0x00000000
; end
li R2 0x0000FFFF
; memory location of loop start
li R3 loop
loop:
  ; store the contents of R1 at the memory location pointed by R1
  sw R1 R1
  ; increment the counter
  inc R1
  ; loop if the counter hasn't yet reached the end
  bne R1 R2 R3
  ; end program
  halt

## How to setup and use

For now, the application can only work for windows systems because of file management differences with other operating systems. Pretty funny since I'm writing this on Ubuntu but that's the irony of a developer's life!

1. Download the Pesapal folder. 
2. Write your assembly language code with the instruction set provided in a .txt file and save it in the .txt subfolder of the Pesapal folder. 
3. Go to the assembler folder and run assembler.exe. It will prompt you for the name of your .txt assembly file e.g. "myAssembly.txt". Once it executes successfully, a binary file will be created in the .binary.txt folder titled "myAssembly.binary.txt". 
4. Go to the simulator folder and run simulator.exe. It will prompt you for the name of the .binary.txt file you would like to simulate. Once you enter the file, wait for execution.

NOTE: testBench assembly programs have been provided in the .txt sub-folder for users who prefer not to write any assembly code. Use the testBenches to try out the functionality. The testBench binary files in the .binary.txt file may also be loaded into the simulator or deleted since they can be regenerated from their source files in the .txt sub-folder.

## Shortcomings

The exception handlers for the application have not been completed so crashes may occur when unexpected input is given.


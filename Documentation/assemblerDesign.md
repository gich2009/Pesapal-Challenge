# Assembler Design

## Rules for the Assembler

The assembler should:

1. Discard all empty lines.
2. Discard any lines starting with the comment character ‘;’.
3. Ignore leading white space characters.
4. Replace any identifiers with their addresses as immediate values and store the identifiers in a map for future reference.
5. Keep track of both the absolute line count which is the line number of the .txt document the assembler is currently compiling.
6. Keep track of the line count which is the line number of the .txt document the assembler is currently compiling ignoring the empty lines and the comment lines. This line count represents the address of an instruction.
7. Encode ‘don’t care’ bits as 0s.
8. Put each encoded byte on a separate line in little-endian, that is, from low byte to high byte.
9. Be case insensitive.

Additional rules:

10. For the instruction word, each high 16-bit half word of an instruction will be don’t care bits since all instructions can be encoded in the low 16-bit half word with one exception whose case is described in [12].

11. Of the low 16 bits of the instruction word, the first 4 bits (nibble) will contain the instruction number, which can be anything from 0x0 to 0xE, while the second, third and fourth nibbles contain register numbers. The register nibbles will be represented as follows:

| Register | Hex representation | Binary representation |
|----------|--------------------|-----------------------|
| R1       | 0x1                | 0001                  |
| R2       | 0x2                | 0010                  |
| R3       | 0x3                | 0011                  |

12. For some instructions that take an immediate operand like li, the high half word will contain a 16-bit immediate value.

13. The instruction nibbles will be mapped as follows:

| Instruction | Hex representation | Binary representation |
|-------------|--------------------|-----------------------|
| halt        | 0x0                | 0000                  |
| nop         | 0x1                | 0001                  |
| li          | 0x2                | 0010                  |
| lw          | 0x3                | 0011                  |
| sw          | 0x4                | 0100                  |
| add         | 0x5                | 0101                  |
| sub         | 0x6                | 0110                  |
| mult        | 0x7                | 0111                  |
| div         | 0x8                | 1000                  |
| j           | 0x9                | 1001                  |
| jr          | 0xA                | 1010                  |
| beq         | 0xB                | 1011                  |
| bne         | 0xC                | 1100                  |
| inc         | 0xD                | 1101                  |
| dec         | 0xE                | 1100                  |

14. If the immediate address is an identifier, add the new identifier to a map that keeps track of all the identifiers of the program together with its address which is given as the address (line count) of the current statement +2. This is because the next statement (+1) is syntactically guaranteed to be non-actionable since it will mark the beginning of the identifier using the syntax “identifier_name: ”

15. If any non-key word is encountered in a statement in the code:  
    i. If it has a ‘:’ character to signify an identifier tail, then check the identifier map to ascertain that it is an identifier, else throw an invalid statement exception.  
    ii. If it does not have a ‘:’ character, then throw an invalid statement exception.If any non-key word is encountered in a statement in the code:  

Example Assembly Program
------------------------
01:	; a simple counter program.  
02:	li R1 0x00000000  
03:	; end  
04:	li R2 0x0000FFFF  
05:	; memory location of loop start  
06:	li R3 loop  
07:	loop:  
08:	; store the contents of R1 at the location pointed by R1  
09:	sw R1 R1  
10:	; increment the counter  
11:	inc R1  
12:	; loop if the counter hasn't yet reached the end  
13:	bne R1 R2 R3  
14:  
15:	; end program  
16:	HALT  

| Instruction                                   | Instruction part actioned | Absolute line count | Line count (Address) | Byte                  | Rule            |
|-----------------------------------------------|---------------------------|---------------------|----------------------|-----------------------|----------------|
| ; a simple counter program.                   | Ignored                   | 1                   | Invalid              | Not Applicable        | #2             |
|                                               |                           |                     |                      |                       |                |
| li R1 0x00000000                              |                           |                     |                      |                       |                |
| First byte                                    | Li R1                     | 2                   | 0                    | 0001 (R1) + 0010 (li) | #11 & #13      |
| Second byte                                   | Don’t care (Xs)           | 2                   | 0                    | 0000 + 0000           | #7             |
| Third byte                                    | Immediate value low byte:  | 2                   | 0                    | 0000 + 0000           | #12            |
|                                               | 0x00                      |                     |                      |                       |                |
| Fourth byte                                   | Immediate value high byte: | 2                   | 0                    | 0000 + 0000           | #12            |
|                                               | 0x00                      |                     |                      |                       |                |
|                                               |                           |                     |                      |                       |                |
| ; end                                         | Ignored                   | 3                   | 0                    | Not Applicable        | #2             |
|                                               |                           |                     |                      |                       |                |
| li R2 0x0000FFFF                              |                           |                     |                      |                       |                |
| First byte                                    | Li R2                     | 4                   | 1                    | 0010 (R2) + 0010 (li) | #11 & #13      |
| Second byte                                   | Don’t care (Xs)           | 4                   | 1                    | 0000 + 0000           | #7             |
| Third byte                                    | Immediate value low byte:  | 4                   | 1                    | 1111 + 1111           | #12            |
|                                               | 0xFF                      |                     |                      |                       |                |
| Fourth byte                                   | Immediate value high byte: | 4                   | 1                    | 1111 + 1111           | #12            |
|                                               | 0xFF                      |                     |                      |                       |                |
|                                               |                           |                     |                      |                       |                |
| ; memory location of loop start               | Ignored                   | 5                   | 1                    | Not Applicable        | #2             |
|                                               |                           |                     |                      |                       |                |
| li R3 loop                                    |                           |                     |                      |                       |                |
| First byte                                    | Li R3                     | 6                   | 2                    | 0011 (R3) + 0010 (li) | #11 & #13      |
| Second byte                                   | Don’t care (Xs)           | 6                   | 2                    | 0000 + 0000           | #7             |
| Third byte                                    | Immediate value given as an identifier (low byte) | 6 | 2 | 0000 + 0100 | #4, #6, #13   |
| Fourth byte                                   | Immediate value given as an identifier (high byte) | 6 | 2 | 0000 + 0000 | #4, #6, #14

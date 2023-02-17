#ifndef ALU_H
#define ALU_H

#include "register.h"
#include "memory.h"

//A class that defines an Arithmetic and Logic Unit interface for an instruction set with 15 instructions. It only implements 13 of 15
//instructions as the other 2 are HALT and NOP(no operation) which both require no work to be done by the ALU. Has the following members:
//   i. loadImmediate(): Returns a bool as a flag for whether the operation was successful or not. Takes a general register 
//                       and loads it with the an immediate operand word.
//
//  ii. loadWord(): Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register 
//                  and loads it with the contents of the memory location pointed to by the address in the source general register. 
//                  Also takes a sourceIndex register for indexing the source and the memory object to be accessed.
//
// iii. storeWord(): Returns a bool as a flag for whether the operation was successful or not. Takes a source general register and 
//                   loads its contents into the memory location pointed to by the address in the destination general register. Also 
//                   takes a destinationIndex register for indexing the destination and the memory object to be accessed.
//
//  iv. add(): Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and loads 
//             it with the result of the addition of the contents of operand1 with the contents operand2 which are both general registers. 
//             i.e. operand1 + operand2.
//
//   v. subtract(): Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and 
//                  loads it with the result of the difference between the contents of operand1 with the contents of operand2 which are 
//                  both general registers. i.e. operand1 - operand2.
//
//  vi. multiply(): Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and 
//                  loads it with the result of the multiplication of the contents of operand1 with the contents of operand2 which are 
//                  both general registers. i.e operand1 * operand2.
//
// vii. divide(): Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and 
//                loads it with the result of the division of the contents of operand1 with the contents of operand2 which are both 
//                general registers. i.e operand1 / operand2.
//
//viii. unconditionalJumpImmediate(): Returns a bool as a flag for whether the operation was successful or not. Takes a programCounter 
//                                    general register and loads it with the word in the immediate operand argument.
//
//  ix. unconditinalJumpRegister(): Returns a bool as a flag for whether the operation was successful or not. Takes a programCounter 
//                                  general register and loads it with the contents of the destination general register.
//
//   x. branchIfEqual(): Returns a bool as a flag for whether the operation was successful or not. Compares the contents of operand1 to 
//                       the contents of operand2 and branches to the address pointed to by the source general register if the contents 
//                       are equal. The program counter must be passed in order to load it with the address from the source general 
//                       register.
//
//  xi. branchIFNotEqual(): Returns a bool as a flag for whether the operation was successful or not. Compares the contents of operand1 
//                          to the contents of operand2 and branches to the address pointed to by the source general register if the 
//                          contents are NOT equal. The program counter must be passed in order to load it with the address from the 
//                          source general register.
//
// xii. incrementRegister(): Returns a bool as a flag for whether the operation was successful or not. Takes a general register and 
//                           increments its contents by one.
//
//xiii. decrementRegister(): Returns a bool as a flag for whether the operation was successful or not. Takes a general register and 
//                           decrements its contents by one.
class ALU{
    public:
    using word = std::string;

    //Returns a bool as a flag for whether the operation was successful or not. Takes a general register and loads it
    //with the an immediate operand word.
    bool loadImmediate(GeneralRegister &operand1, const word &immediateOperand);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and loads it
    //with the contents of the memory location pointed to by the address in the source general register. Also takes a sourceIndex
    //register for indexing the source and the memory object to be accessed.
    bool loadWord(GeneralRegister &destination, GeneralRegister &source, GeneralRegister &sourceIndex, Memory &RAM);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a source general register and loads its
    //contents into the memory location pointed to by the address in the destination general register. Also takes a destinationIndex
    //register for indexing the destination and the memory object to be accessed.
    bool storeWord(GeneralRegister &destination, GeneralRegister &source, GeneralRegister &destinationIndex, Memory &RAM);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and loads it
    //with the result of the addition of the contents of operand1 with the contents operand2 which are both general registers. 
    //i.e. operand1 + operand2.
    bool add(GeneralRegister &destination, GeneralRegister &operand1, GeneralRegister &operand2);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and loads it
    //with the result of the difference between the contents of operand1 with the contents of operand2 which are both general registers.
    //i.e. operand1 - operand2.
    bool subtract(GeneralRegister &destination, GeneralRegister &operand1, GeneralRegister &operand2);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and loads it
    //with the result of the multiplication of the contents of operand1 with the contents of operand2 which are both general registers.
    //i.e operand1 * operand2.
    bool multiply(GeneralRegister &destination, GeneralRegister &operand1, GeneralRegister &operand2);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and loads it
    //with the result of the division of the contents of operand1 with the contents of operand2 which are both general registers.
    //i.e operand1 / operand2.
    bool divide(GeneralRegister &destination, GeneralRegister &operand1, GeneralRegister &operand2);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a programCounter general register and loads it
    //with the word in the immediate operand argument.
    bool unconditionalJumpImmediate(GeneralRegister &programCounter, const word &immediateOperand);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a programCounter general register and loads it
    //with the contents of the source general register.
    bool unconditionalJumpRegister(GeneralRegister &source, GeneralRegister &programCounter);


    //Returns a bool as a flag for whether the operation was successful or not. Compares the contents of operand1 to the contents of
    //operand2 and branches to the address pointed to by the source general register if the contents are equal. The program counter must
    //be passed in order to load it with the address from the source general register.
    bool branchIfEqual(GeneralRegister &operand1, GeneralRegister &operand2, GeneralRegister &source, GeneralRegister &programCounter);


    //Returns a bool as a flag for whether the operation was successful or not. Compares the contents of operand1 to the contents of
    //operand2 and branches to the address pointed to by the source general register if the contents are NOT equal. The program counter 
    //must be passed in order to load it with the address from the source general register.
    bool branchIfNotEqual(GeneralRegister &operand1, GeneralRegister &operand2, GeneralRegister &source, GeneralRegister &programCounter);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a general register and increments its contents
    //by one
    bool incrementRegister(GeneralRegister &operand1);


    //Returns a bool as a flag for whether the operation was successful or not. Takes a general register and decrements its contents
    //by one
    bool decrementRegister(GeneralRegister &operand1);
};

#endif
#ifndef computer_H
#define computer_H


#include "register.h"
#include "memory.h"
#include "ALU.h"


//A class that defines a computer with 3 "general" registers and 4 special purpose registers. The "general" registers are not necessarily
//objects of type GeneralRegister but rather registers that may be used by any programmer user of the computer. All "general" registers
//are objects of type GeneralRegister but not all objects of type GeneralRegister are "general" registers. Some are used for special
//functions. The "general" registers are R1, R2 and R3. The GeneralRegisters meant for special functions (and are therefore special
//purpose registers) are the programCounter, sourceIndex, destinationIndex and the codeSegment. The class also  provides one more special
//purpose register which is not a GeneralRegister and is instead a FlagRegister known as the conditionRegister. Has the following members:
// public:
//
//   i. hi(): Initializes the computer by prompting the user for their name, its name(if it is a new computer object that has an empty name member)
//            and prompting the user to specify the program to be run.  
//
//  ii. run(): Runs the loaded program
//
// iii. getComputerName(): Gets the computer name member of the computer object.
//
//  iv. getUserName(): Gets the user name member of the computer object.
//
//   v. getCreator(): Gets the creator name member of the computer object.
//
//private:
//
//  vi. loadProgram(): Loads the program to be run by the computer.
//
// vii. fetch(): Returns a bool as a flag for whether the operation was successful or not. Takes a destination general register and 
//               loads it with the result of the division of the contents of operand1 with the contents of operand2 which are both 
//               general registers. i.e operand1 / operand2.
//
//viii. decode(): Returns a bool as a flag for whether the operation was successful or not. Takes a programCounter 
//                general register and loads it with the word in the immediate operand argument.
//
//  ix. execute(): Returns a bool as a flag for whether the operation was successful or not. Takes a programCounter 
//                 general register and loads it with the contents of the destination general register.
//
//   x. log(): Logs the process by printing out the contents of all the registers. The log is done in both binary and hexadecimal. Takes the
//             execution status flag from the execute member.
class Computer{
    public:
    using word = std::string;
    using address = Memory::address;
    enum NUMSYSTEM{BINARY = 1 , HEX = 2, BINARY_AND_HEX = 3} numberSystem = HEX; 

    Computer() = default;
    explicit Computer(const std::string &initcomputerName) : computerName(initcomputerName){}
    Computer(const std::string &initcomputerName, const std::string &initfileName) : computerName(initcomputerName), fileName(initfileName){}
    Computer(const std::string &initcomputerName, const std::string &initfileName, const std::string &inituserName) : 
            computerName(initcomputerName), fileName(initfileName), userName(inituserName){}

    //Initializes the computer by prompting the user for their name, its name(if it is a new computer object that has an empty name member)
    //and prompting the user to specify the program to be run.   
    void hi(const char *creatorInitializer = "Amos Wanene");


    //Runs the loaded program
    void run(const NUMSYSTEM myNumberSystem = BINARY_AND_HEX);
    
    //Gets the computer name member of the computer object.
    std::string getComputerName(){
        return this->computerName;
    }

    //Gets the user name member of the computer object.
    std::string getUserName(){
        return this->userName;
    }

    //Gets the creator name member of the computer object.
    std::string getCreator(){
        return this->creator;
    }

    // void memoryLog (const std::size_t &startAddress, const std::size_t &stopAddress);
    void memoryLog (const address &startAddress, const address &stopAddress);


    private:
    //Loads the program to be run by the computer.
    std::size_t loadProgram (std::string &fileName);

    //Fetches an instruction from memory. Returns it as a word.
    word fetch();

    //Decodes the instruction. Returns a word with an immediate operand, else returns an empty word.
    word decode(const word &instructionWord);

    //Executes the instruction. Takes an immediate operand returned by the decode member. Returns a bool to indicate the state of the 
    //process.
    bool execute(const word &immediateOperand);

    //Logs the process by printing out the contents of all the registers. The log is done in both binary and hexadecimal. Takes the
    //execution status flag from the execute member.
    void log(const bool &executionStatus);

    std::string computerName, userName, fileName, creator;
    ALU    processor;
    Memory RAM;
    GeneralRegister R1, R2, R3, programCounter, sourceIndex, destinationIndex, codeSegment{codeSegmentInitial};
    FlagRegister conditionRegister;

 };

 extern Computer Murife;

#endif
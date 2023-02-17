#include "computer.h"
#include <unistd.h>
#include <direct.h>
#include <algorithm>

 
Computer Murife ("Murife");

constexpr std::size_t fileNameMax = 300;

void Computer::hi(const char *creatorInitializer){
    this->creator = creatorInitializer;
    std::size_t bytesLoaded = 0;

    if (!this->userName.empty() && !this->computerName.empty() && !this->fileName.empty()){
        std::cout << "Hi " << this->getUserName() << ". I'm " << this->getComputerName() << "please wait as the program loads.\n" << std::flush;
        bytesLoaded = this->loadProgram(this->fileName);
        std::cout << "Number of bytes loaded to RAM: " << bytesLoaded << '\n' << std::flush;
        return;
    }

    while (this->userName.empty()){
        std::cout << "Hi there! What's your name? " << std::flush;
        getline(std::cin, this->userName);
    }


    if (!this->computerName.empty()){
        while (this->fileName.empty()){
            std::cout << "Hi " << this->getUserName() << ". I'm " << this->getComputerName() << ", a computer simulator created by " 
                      << this->getCreator() << ". What file would you like me to run? " << std::flush;
            getline (std::cin, this->fileName);
        }

        bytesLoaded = this->loadProgram(this->fileName);
        std::cout << "Number of bytes loaded to RAM: " << bytesLoaded << '\n' << std::flush;
        return;
    }


    while (this->computerName.empty()){
        std::cout << "What's my name? " << std::flush;
        getline(std::cin, this->computerName);
    }

  
    while (this->fileName.empty()){
        std::cout << "Alright " << getUserName() << ", what file would you like me to run? " << std::flush;
        getline (std::cin, this->fileName);
    }

    bytesLoaded = this->loadProgram(this->fileName);

    std::cout << "Number of bytes loaded to RAM: " << bytesLoaded << '\n' << std::flush;
}


std::size_t Computer::loadProgram (std::string &fileName){
    chdir("..");
    char currentWorkingDirectory[fileNameMax];
    getcwd(currentWorkingDirectory, fileNameMax);

    std::string::const_iterator found = std::find(fileName.cbegin(), fileName.cend(), '.');

    //Remove the extension if it exists
    while (found != fileName.cend()){
        for (std::string::iterator i = (fileName.end() - 1) ; *i != '.' ; --i){
            fileName.pop_back();
        }
        fileName.pop_back();
        found = std::find(fileName.cbegin(), fileName.cend(), '.');
    }
    std::cout << "\n" << fileName << std::endl;


    std::string  binaryTxtDirectory = "\\.binary.txt\\";
    const char  *extension1 = ".binary";
    const char  *extension2 = ".txt"; 

    std::ifstream binFile(currentWorkingDirectory + binaryTxtDirectory + fileName + extension1 + extension2, std::ios_base::in);

    std::string byte{""};

    //print something or set error bit.
    if(!binFile.is_open()){
        std::cout << "File not found." << std::endl;
        throw std::runtime_error("Could not find the .binary.txt file.");
    }

    binFile.seekg(std::ios_base::beg);

    while(binFile >> byte){
        std::size_t effectiveAddress = codeSegment.readInt() + destinationIndex.readInt();
        RAM.writeAt(byte, effectiveAddress);
        destinationIndex.increment();
    }

    std::size_t bytesLoaded = destinationIndex.readInt();

    destinationIndex.clear();

    binFile.close();

    return bytesLoaded;
}



void Computer::run(const NUMSYSTEM myNumberSystem){
    numberSystem = myNumberSystem;

    std::string instructionWord, immediateOperand;
    bool executionStatus;

    while(true){

        if (conditionRegister.checkBit(HALTFLAG)){   
            std::cout << "Program Halt.\n" << std::endl;
            break;
        }

        conditionRegister.clear();

        instructionWord  = this->fetch();

        immediateOperand = this->decode(instructionWord);

        executionStatus  = this->execute(immediateOperand);

        this->log(executionStatus);
    }
}



Computer::word Computer::fetch(){
    
    word instructionWord = "";

    std::size_t thereAreBytesToReadForThisInstruction = (wordSize / byteSize);    
   
    while (thereAreBytesToReadForThisInstruction){
        std::size_t effectiveAddress = codeSegment.readInt() + programCounter.readInt();
        instructionWord += this->RAM.readAt(effectiveAddress);
        programCounter.increment();
        --thereAreBytesToReadForThisInstruction;
    }

    return instructionWord;
}



Computer::word Computer::decode(const word &instructionWord){
    //DECODE OPCODE FIELD
    word nibble = instructionWord.substr((byteSize/2), byteSize/2);
    std::map<const word, FLAG> operationCodeDecoder{{"0000", HALTFLAG}, {"0001", NOPFLAG}, {"0010", LIFLAG}, {"0011", LWFLAG}, {"0100", SWFLAG},
                                              {"0101", ADDFLAG}, {"0110", SUBFLAG}, {"0111", MULTFLAG}, {"1000", DIVFLAG}, {"1001", JFLAG},
                                              {"1010", JRFLAG}, {"1011", BEQFLAG}, {"1100", BNEFLAG}, {"1101", INCFLAG}, {"1110", DECFLAG}};

    //no need to have the FLAG variable. You can load the operationCode directly into the conditionRegister.
    FLAG operationCode = operationCodeDecoder[nibble];
    conditionRegister.setBit(operationCode);
    

    //May also execute as conditionRegister.checkBit(HALTFLAG) || conditionRegister.checkBit(NOPFLAG)
    if (operationCode == HALTFLAG || operationCode == NOPFLAG){
        return "";
    }

    if (operationCode == JFLAG){
        return instructionWord.substr(0, wordSize/2);
    }


    //DECODE ADDRESS FIELD 1
    nibble = instructionWord.substr(0, byteSize/2);
    std::map<const word, FLAG> addressfield1Decoder{{"0001", ADDRESSFIELD1_R1}, {"0010", ADDRESSFIELD1_R2}, {"0011", ADDRESSFIELD1_R3}};
    FLAG addressField1 = addressfield1Decoder[nibble];
    conditionRegister.setBit(addressField1);

    if (operationCode == JRFLAG || operationCode == INCFLAG || operationCode == DECFLAG){
        return "";
    }

    if (operationCode == LIFLAG){
        nibble = instructionWord.substr((wordSize - byteSize), byteSize);
        nibble += instructionWord.substr((wordSize - byteSize * 2), byteSize);
        return nibble;
    } 

    
    //DECODE ADDRESS FIELD 2
    nibble = instructionWord.substr(((wordSize / 2) - (byteSize / 2)), byteSize/2);
    std::map<const word, FLAG> addressfield2Decoder{{"0001", ADDRESSFIELD2_R1}, {"0010", ADDRESSFIELD2_R2}, {"0011", ADDRESSFIELD2_R3}};
    FLAG addressField2 = addressfield2Decoder[nibble];
    conditionRegister.setBit(addressField2);

    if (operationCode == LWFLAG || operationCode == SWFLAG){
        return "";
    }


    //DECODE ADDRESS FIELD 3
    nibble = instructionWord.substr(((wordSize / 2) - byteSize), byteSize/2);
    std::map<const word, FLAG> addressfield3Decoder{{"0001", ADDRESSFIELD3_R1}, {"0010", ADDRESSFIELD3_R2}, {"0011", ADDRESSFIELD3_R3}};
    FLAG addressField3 = addressfield3Decoder[nibble];
    conditionRegister.setBit(addressField3);

    return "";
}


bool Computer::execute(const word &immediateOperand){
    bool successState = false;
    
    if (!immediateOperand.empty()){
        if (conditionRegister.checkBit(LIFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1)){
                successState = processor.loadImmediate(R1, immediateOperand);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2)){
                successState = processor.loadImmediate(R2, immediateOperand);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3)){
                successState = processor.loadImmediate(R3, immediateOperand);
                return successState;
            }
        }

        if (conditionRegister.checkBit(JFLAG)){
            successState = processor.unconditionalJumpImmediate(programCounter, immediateOperand);
            return successState;
        }
    }
    
    if (immediateOperand.empty()){                     //This is a dead check, kinda, unless something returns a non-empty operand. Guess its safe
        if (conditionRegister.checkBit(HALTFLAG)){
            successState = true;
            return successState;
        }

        if (conditionRegister.checkBit(NOPFLAG)){
            successState = true;
            return successState;
        }

        if (conditionRegister.checkBit(LWFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1)){
                successState = processor.loadWord(R1, R1, sourceIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2)){
                successState = processor.loadWord(R2, R2, sourceIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3)){
                successState = processor.loadWord(R3, R3, sourceIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2)){
                successState = processor.loadWord(R1, R2, sourceIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1)){
                successState = processor.loadWord(R2, R1, sourceIndex, RAM);
                return successState;
            }

            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3)){
                successState = processor.loadWord(R1, R3, sourceIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1)){
                successState = processor.loadWord(R3, R1, sourceIndex, RAM);
                return successState;
            }

            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3)){
                successState = processor.loadWord(R2, R3, sourceIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2)){
                successState = processor.loadWord(R3, R2, sourceIndex, RAM);
                return successState;
            }
        }

        if (conditionRegister.checkBit(SWFLAG)){

            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1)){
                successState = processor.storeWord(R1, R1, destinationIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2)){
                successState = processor.storeWord(R2, R2, destinationIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3)){
                successState = processor.storeWord(R3, R3, destinationIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2)){
                successState = processor.storeWord(R1, R2, destinationIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1)){
                successState = processor.storeWord(R2, R1, destinationIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3)){
                successState = processor.storeWord(R1, R3, destinationIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1)){
                successState = processor.storeWord(R3, R1, destinationIndex, RAM);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3)){
                successState = processor.storeWord(R2, R3, destinationIndex, RAM);
                return true;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2)){
                successState = processor.storeWord(R3, R2, destinationIndex, RAM);
                return true;
            }
        }

        if (conditionRegister.checkBit(ADDFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R1, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R1, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R1, R2, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R1, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R2, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R2, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R2, R2, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R2, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R1, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R1, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R1, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R3, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R3, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R3, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R3, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R2, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R2, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R2, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R3, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R3, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R3, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R1, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R1, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.add(R2, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R2, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.add(R3, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.add(R3, R2, R1);
                return successState;
            }
        }
        
        if (conditionRegister.checkBit(SUBFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R1, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R1, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R1, R2, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R1, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R2, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R2, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R2, R2, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R2, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R1, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R1, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R1, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R3, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R3, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R3, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R3, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R2, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R2, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R2, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R3, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R3, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R3, R3, R2);
                return successState;
            }
             if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R1, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R1, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.subtract(R2, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R2, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.subtract(R3, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.subtract(R3, R2, R1);
                return successState;
            }

        }

        if (conditionRegister.checkBit(MULTFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R1, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R1, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R1, R2, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R1, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R2, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R2, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R2, R2, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R2, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R1, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R1, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R1, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R3, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R3, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R3, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R3, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R2, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R2, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R2, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R3, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R3, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R3, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R1, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R1, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.multiply(R2, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R2, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.multiply(R3, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.multiply(R3, R2, R1);
                return successState;
            }
        }

        if (conditionRegister.checkBit(DIVFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R1, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R1, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R1, R2, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R1, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R2, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R2, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R2, R2, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R2, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R1, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R1, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R1, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R3, R1, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R3, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R3, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R3, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R2, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R2, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R2, R3, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R3, R2, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R3, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R3, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R1, R2, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R1, R3, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.divide(R2, R1, R3);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R2, R3, R1);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.divide(R3, R1, R2);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.divide(R3, R2, R1);
                return successState;
            }
        }

        if (conditionRegister.checkBit(JRFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1)){
                successState = processor.unconditionalJumpRegister(R1, programCounter);
                return successState;
            }

            if (conditionRegister.checkBit(ADDRESSFIELD1_R2)){
                successState = processor.unconditionalJumpRegister(R2, programCounter);
                return successState;
            }

            if (conditionRegister.checkBit(ADDRESSFIELD1_R3)){
                successState = processor.unconditionalJumpRegister(R3, programCounter);
                return successState;
            }
        }

        if (conditionRegister.checkBit(BEQFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.branchIfEqual(R1, R2, R3, programCounter);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.branchIfEqual(R1, R3, R2, programCounter);
                return successState;
            }


            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.branchIfEqual(R2, R1, R3, programCounter);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.branchIfEqual(R2, R3, R1, programCounter);
                return successState;
            }


            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.branchIfEqual(R3, R1, R2, programCounter);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.branchIfEqual(R3, R2, R1, programCounter);
                return successState;
            }

        }

        if (conditionRegister.checkBit(BNEFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.branchIfNotEqual(R1, R2, R3, programCounter);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.branchIfNotEqual(R1, R3, R2, programCounter);
                return successState;
            }


            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R3)){
                successState = processor.branchIfNotEqual(R2, R1, R3, programCounter);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R2) && conditionRegister.checkBit(ADDRESSFIELD2_R3) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.branchIfNotEqual(R2, R3, R1, programCounter);
                return successState;
            }


            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R1) && conditionRegister.checkBit(ADDRESSFIELD3_R2)){
                successState = processor.branchIfNotEqual(R3, R1, R2, programCounter);
                return successState;
            }
            if (conditionRegister.checkBit(ADDRESSFIELD1_R3) && conditionRegister.checkBit(ADDRESSFIELD2_R2) && conditionRegister.checkBit(ADDRESSFIELD3_R1)){
                successState = processor.branchIfNotEqual(R3, R2, R1, programCounter);
                return successState;
            }
        }

        if (conditionRegister.checkBit(INCFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1)){
                successState = processor.incrementRegister(R1);
                return successState;
            }

            if (conditionRegister.checkBit(ADDRESSFIELD1_R2)){
                successState = processor.incrementRegister(R2);
                return successState;
            }

            if (conditionRegister.checkBit(ADDRESSFIELD1_R3)){
                successState = processor.incrementRegister(R3);
                return successState;
            }
        }

        if (conditionRegister.checkBit(DECFLAG)){
            if (conditionRegister.checkBit(ADDRESSFIELD1_R1)){
                successState = processor.incrementRegister(R1);
                return successState;
            }

            if (conditionRegister.checkBit(ADDRESSFIELD1_R2)){
                successState = processor.incrementRegister(R2);
                return successState;
            }

            if (conditionRegister.checkBit(ADDRESSFIELD1_R3)){
                successState = processor.incrementRegister(R3);
                return successState;
            }
        }
    }

    return successState;
}


void Computer::log(const bool &executionStatus){
    executionStatus? (std::cout << "Execution success") : (std::cout << "Execution failure");
    std:: cout << '\n' << std::flush;

    // R1, R2, R3, programCounter, sourceIndex, destinationIndex, codeSegment, conditionRegister.
    if (numberSystem == BINARY || numberSystem == BINARY_AND_HEX){
        std::cout << "R1                 : " << R1.makeWord().read()                << '\n'
                  << "R2                 : " << R2.makeWord().read()                << '\n'
                  << "R3                 : " << R3.makeWord().read()                << '\n'
                  << "Program counter    : " << programCounter.makeWord().read()    << '\n'
                //   << "Source index       : " << sourceIndex.makeWord().read()       << '\n'
                //   << "Destination index  : " << destinationIndex.makeWord().read()  << '\n'
                  << "Code segment       : " << codeSegment.makeWord().read()       << '\n'
                  << "Condition register : " << conditionRegister.makeWord().read() << '\n';
    }

    std::cout << '\n' << std::flush;

    if (numberSystem == HEX || numberSystem == BINARY_AND_HEX){
        std::cout << "R1                 : " << toHexadecimal(R1.makeWord().read())                << '\n'
                  << "R2                 : " << toHexadecimal(R2.makeWord().read())                << '\n'
                  << "R3                 : " << toHexadecimal(R3.makeWord().read())                << '\n'
                  << "Program counter    : " << toHexadecimal(programCounter.makeWord().read())    << '\n'
                //   << "Source index       : " << toHexadecimal(sourceIndex.makeWord().read())       << '\n'
                //   << "Destination index  : " << toHexadecimal(destinationIndex.makeWord().read())  << '\n'
                  << "Code segment       : " << toHexadecimal(codeSegment.makeWord().read())       << '\n'
                  << "Condition register : " << toHexadecimal(conditionRegister.makeWord().read()) << '\n';
    }

    std::cout << '\n' << '\n' << std::flush;
}


void Computer::memoryLog (const address &startAddress, const address &stopAddress){
    Murife.RAM.displayRange(startAddress, stopAddress);
}
#include "ALU.h"

bool ALU::loadImmediate(GeneralRegister &operand1, const word &immediateOperand){
    operand1.load(immediateOperand);
    return operand1.checkFlag();
}


bool ALU::loadWord(GeneralRegister &destination, GeneralRegister &source, GeneralRegister &sourceIndex, Memory &RAM){
    std::size_t thereAreBytesToLoad = (wordSize / byteSize);
    std::string tempBuffer = "";
    std::string buffer = "";
    std::size_t indexPos = (wordSize - byteSize);
    sourceIndex.clear();
    
    while (thereAreBytesToLoad){
        tempBuffer += RAM.readAt(source.readInt() + sourceIndex.readInt()); 
        sourceIndex.increment(); 
        --thereAreBytesToLoad;
    }   

    thereAreBytesToLoad = (wordSize / byteSize);
    while (thereAreBytesToLoad){
        buffer += tempBuffer.substr(indexPos, byteSize);
        indexPos -= byteSize; 
        --thereAreBytesToLoad;
    }
    
    destination.load(buffer);

    return destination.checkFlag();
}

bool ALU::storeWord(GeneralRegister &destination, GeneralRegister &source, GeneralRegister &destinationIndex, Memory &RAM){
    std::size_t thereAreBytesToStore = (wordSize / byteSize);
    source.makeWord();
    std::string buffer = source.read();
    destinationIndex.clear();
    int statusCondition  = 1;
    std::size_t indexPos = (wordSize - byteSize);

    while (thereAreBytesToStore){
        RAM.writeAt(buffer.substr(indexPos, byteSize), destination.readInt() + destinationIndex.readInt()); 
        indexPos -= byteSize;
        destinationIndex.increment(); 
        statusCondition = statusCondition & RAM.checkFlag();
        --thereAreBytesToStore;
    }

    // std::cout << '\n' << RAM.readAt(destination.readInt()) << std::endl;

    // indexPos = 24;
    // (buffer.substr(indexPos, byteSize) == RAM.readAt(source.readInt())) ? true : false;
    return statusCondition;
}


bool ALU::add(GeneralRegister &destination, GeneralRegister &operand1, GeneralRegister &operand2){
    std::size_t buffer = operand1.readInt() + operand2.readInt();
    destination.loadInt(buffer);
    return destination.checkFlag();
}


bool ALU::subtract(GeneralRegister &destination, GeneralRegister &operand1, GeneralRegister &operand2){
    std::size_t buffer = operand1.readInt() - operand2.readInt();
    destination.loadInt(buffer);
    return destination.checkFlag();
}


bool ALU::multiply(GeneralRegister &destination, GeneralRegister &operand1, GeneralRegister &operand2){
    std::size_t buffer = operand1.readInt() * operand2.readInt();
    destination.loadInt(buffer);
    return destination.checkFlag();
}


bool ALU::divide(GeneralRegister &destination, GeneralRegister &operand1, GeneralRegister &operand2){
    std::size_t buffer = operand1.readInt() / operand2.readInt();
    destination.loadInt(buffer);
    return destination.checkFlag();
}


bool ALU::unconditionalJumpImmediate(GeneralRegister &programCounter, const word &immediateOperand){
    programCounter.load(immediateOperand);
    return programCounter.checkFlag();
}


bool ALU::unconditionalJumpRegister(GeneralRegister &source, GeneralRegister &programCounter){
    programCounter.load(source.read());
    return programCounter.checkFlag();
}


bool ALU::branchIfEqual(GeneralRegister &operand1, GeneralRegister &operand2, GeneralRegister &source, GeneralRegister &programCounter){
    bool equal = (operand1.read() == operand2.read()); 

    if(!equal){
        return false;
    }

    programCounter.load(source.read());

    return programCounter.checkFlag();
}


bool ALU::branchIfNotEqual(GeneralRegister &operand1, GeneralRegister &operand2, GeneralRegister &source, GeneralRegister &programCounter){

    bool equal = (operand1.read() == operand2.read()); 

    if(equal){
        return false;
    }

    programCounter.load(source.read());

    return programCounter.checkFlag();
}

bool ALU::incrementRegister(GeneralRegister &operand1){
    operand1.increment();
    return operand1.checkFlag();
}

bool ALU::decrementRegister(GeneralRegister &operand1){
    operand1.decrement();
    return operand1.checkFlag();
}
#include "register.h"


/****************************************************************
                        REGISTER METHODS                         *
*****************************************************************/

Register::word Register::read(){
    this->setFlag();
    return this->contents;
}

std::size_t Register::readInt(){
    this->setFlag();
    return toNumber(this->contents);
}

Register &Register::load(const word &parameterWord){
    this->clearFlag();

    this->contents = parameterWord;

    bool success = (this->contents == parameterWord) ? true : false;

    if (success) this->setFlag();

    return *this;
}

Register &Register::loadInt(const std::size_t &word){
    this->clearFlag();
    std::string binaryWord = decimalToBinary(word);
    this->contents = binaryWord;

    bool success  = (this->contents == binaryWord) ? true : false;

    if (success) this->setFlag();

    return *this;
}

Register &Register::clear(){
    this->clearFlag();

    this->contents = zeroValue;

    if (this->contents == zeroValue); 

    return *this;
}


Register &Register::makeWord(){
    this->clearFlag();

    if(this->contents.size() >= wordSize){
        this->clearFlag();
        return *this;
    }

    std::string shortContents     = this->contents;
    std::size_t thereIsDifference = wordSize - this->contents.size();
    this->contents.clear();
    
    while(thereIsDifference){
        this->contents.append("0");  //or this->contents += "0";
        --thereIsDifference;
    }

    this->contents += shortContents;
    if (this->contents.size() == wordSize) this->setFlag(); 

    return *this;
}




/****************************************************************
                    GENERAL REGISTER METHODS                    *
*****************************************************************/

void GeneralRegister::increment(){
    this->clearFlag();

    std::size_t number = toNumber(this->contents);          //not sure whether to have it in base 10 or base 16

    //if true then set overflow flag
    if (number > 0xFFFFFFFF){
        this->clearFlag();
        return;
    }                                                       //Should we include overflows? think about it. for now no.
        
    ++number;

    this->contents = decimalToBinary(number);

    bool success = (toNumber(this->contents) == number) ? true : false;

    if (success) this->setFlag() ; 
}

void GeneralRegister::decrement(){
    this->clearFlag();
    
    std::size_t number = toNumber(this->contents);       

    //if true then set an underflow flag
    if (number == 0){
        this->clearFlag();
        return;
    }
        
    --number;

    this->contents = decimalToBinary(number);

    bool success = (toNumber(this->contents) == number) ? true : false;

    if (success) this->setFlag();
}



void GeneralRegister::fastIncrement(){

    std::size_t number = toNumber(this->contents);          
   
    ++number;

    this->contents = decimalToBinary(number);

}


void GeneralRegister::fastDecrement(){
    std::size_t number = toNumber(this->contents);         
        
    --number;

    this->contents = decimalToBinary(number);
}




/****************************************************************
                      FLAG REGISTER METHODS                     *
*****************************************************************/

bool FlagRegister::checkBit(const FLAG &statusBit){
    this->clearFlag();

    std::size_t flagBit  = statusBit;

    std::size_t checkContents = toNumber(this->contents);

    checkContents = checkContents & flagBit;

    this->setFlag();

      //you may static_cast this to bool but there is no need because it is implicitly casted.
    return checkContents; 
}

FlagRegister &FlagRegister::setBit(const FLAG &statusBit){

    this->clearFlag();

    std::size_t flagBit  = statusBit;

    std::size_t processContents = toNumber(this->contents);

    processContents = processContents | flagBit;

    //WARNING! should probably convert to binary but if not know that contents are in decimal
    this->contents = decimalToBinary(processContents);

    bool success = this->checkBit(statusBit) ? true : false; 

    // this->makeWord();

    if (success) this->setFlag();

    return *this;
}


FlagRegister &FlagRegister::clearBit(const FLAG &statusBit){
    this->clearFlag();

    std::size_t flagBit  = statusBit;

    std::size_t processContents = toNumber(this->contents);

    processContents = processContents & ~flagBit;

    //WARNING! should probably convert to binary but if not know that contents are in decimal
    this->contents = decimalToBinary(processContents);

    bool success = this->checkBit(statusBit) ? false : true;

    // this->makeWord();

    // if (success && this->checkFlag()) this->setFlag();
    if (success) this->setFlag();

    return *this;
}
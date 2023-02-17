#ifndef REGISTER_H
#define REGISTER_H

#include "globalHelper.h"
#include "flagableObject.h"

//A class that defines a 32 bit register. Has the following members:
//  i. read()     returns the word currently stored in the register. May not necessarily be a full 32 bit word.
// ii. readInt()  returns the word currently stored in the register in decimal format.
//iii. load()     loads a new word into the register. Returns a reference to the register object on which it is called.
// iv. loadInt()  loads a new word that is in decimal format into the register. Returns a reference to the register object on which it is called.
//  v. clear()    clears the contents of the register to a zero value. Returns a refence to the register object on which it is called.
// vi. makeWord() makes the contents of the register object on which it is called a full word (32 bits).
class Register : public FlagableObject{
    public:

    using word    = std::string;
    
    using address = std::size_t;

    Register(const word &initializer) : contents(initializer){}

    //Returns the word currently stored in the register. May not necessarily be a full 32 bit word. 
    //Use the  makeWord() member to guarantee that the contents are 32 bits.
    word read();

    //Returns the word currently stored in the register in decimal format.
    std::size_t readInt();

    //Loads a new word into the register. Returns a reference to the register object on which it is called.
    Register &load(const word &parameterWord);

    //Loads a new word that is in unsigned decimal format into the register. Returns a reference to the register object on which it is called.
    Register &loadInt(const std::size_t &word);

    //Clears the contents of the register to a zero value. Returns a refence to the register object on which it is called.
    Register &clear();

    //Makes the contents of the register object on which it is called a full word (32 bits).
    Register &makeWord();
    
    protected:
    word contents;
};




//A class that extends the 32 bit register class and implements more member functions. Has the following members:
//  i. increment()      increments the contents of the register by one.
// ii. decrement()      decrements the contents of the register by one.
//iii. fastIncrement()  increments the contents of the register by one. It does not access the calling register's flag. It does not do bounds checking on the register size. 
// iv. fastDecrement()  decrements the contents of the register by one. It does not access the calling register's flag. It does not do bounds checking on the register size. 
class GeneralRegister : public Register{
    public:

    GeneralRegister(const word initializer = zeroValue) : Register(initializer){}

    //Increments the contents of the register by one.
    void increment();

    //Decrements the contents of the register by one.
    void decrement();

    //Increments the contents of the register by one. It does not access the calling register's flag. It does not do bounds checking on the register size.
    void fastIncrement();

    //Decrements the contents of the register by one. It does not access the calling register's flag. It does not do bounds checking on the register size.
    void fastDecrement();
};








//A class that extends the 32 bit register class and implements more member functions. It is a special register that cannot be incremented or decremented.
//Has the following members:
//  i. checkBit()  Takes an argument of type FLAG and checks whether the corresponding bit in its contents is set or not.
// ii. setBit()    returns a reference to the object on which it is called. Takes a binary argument and sets the corresponding bit(s) in its contents.
//iii. clearBit()  returns a reference to the object on which it is called. Takes a binary argument and clears the corresponding bit(s) in its contents.
class FlagRegister : public Register{
    public:

    FlagRegister(const word initializer = zeroValue) : Register(initializer){}
  
    //Takes an argument of type FLAG and checks whether the corresponding bit in its contents is set or not.
    bool checkBit(const FLAG &statusBit);

    //Returns a reference to the object on which it is called. Takes a binary argument and sets the corresponding bit(s) in its contents.
    FlagRegister &setBit(const FLAG &statusBit);

    //Returns a reference to the object on which it is called. Takes a binary argument and clears the corresponding bit(s) in its contents.
    FlagRegister &clearBit(const FLAG &statusBit);
};



#endif
#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <string>
#include <iostream>
#include "flagableObject.h"

//A class that defines memory of byte addressable type. Has the following members:
//  i. read() that takes an unsigned integer address and returns an unsigned integer byte stored in the memory. It can only read data in the memory stack.
// ii. write() that takes an unsigned integer as the byte to be stored.
//iii. readAt() that takes an effective address and returns an unsigned integer byte stored in the memory. Can read data from anywhere in the memory.
//iv.  writeAt() that takes an unsigned integer as the byte to be stored and an effective address specifying where to store it in memory.
class Memory : public FlagableObject{
    public:
    using byte = std::string;       //or typedef std::string byte;
    using address = std::size_t;

    private:
    std::vector<byte> contents;   
    const std::size_t capacity;

    public: 
    Memory() : capacity(0xFFFF){           //64KiloBYTES
        contents.reserve(capacity);     
    }             


    Memory(decltype(capacity) initCapacity) : capacity(initCapacity){
        contents.reserve(capacity);
    }       

    //Returns a byte from the memory object. Takes an address to read from the memory. The address must be non-negative.
    //It can only read data already in the memory stack.
    byte read(const address &myAddress);


    //Returns a byte from the memory object. Takes an address to read from the memory. The address must be non-negative.
    //It can read data from anywhere in the memory space including addresses which have not been accessed.
    byte readAt(const address &effectiveAddress);


    //Takes a byte and stores it in the memory object. The byte is stored on top of the memory stack.
    void write(const byte &parameterByte);


    //Takes a byte and an address and stores the memory object in the specified effective address.
    void writeAt(const byte &parameterByte, const address &effectiveAdrress);


    //Displays the bytes in the memory from address 0 to the maximum capacity.
    void displayRange(const address &begin, const address &end);
};


#endif
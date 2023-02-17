#include "memory.h"


Memory::byte Memory::read(const address &myAddress){
    this->clearFlag();
    //The condition below is an exception for when the user wants to access a part of memory that is not valid.
    if(this->contents.size() < myAddress)            
        return "";
 
    this->setFlag();
    return this->contents[myAddress];
}


Memory::byte Memory::readAt(const address &effectiveAddress){
    this->clearFlag();

    if(effectiveAddress > this->capacity){
        return "";
    }

    this->setFlag();
    return this->contents[effectiveAddress];
}


void Memory::write(const byte &parameterByte){
    this->clearFlag();

    if(this->contents.size() >= this->capacity)
        return;

    this->contents.push_back(parameterByte);

    if (this->contents.back() == parameterByte) this->setFlag();
}


void Memory::writeAt(const byte &parameterByte, const address &effectiveAddress){
    this->clearFlag();

    if(effectiveAddress > this->capacity)
        throw std::runtime_error("Memory full.");

    this->contents[effectiveAddress] = parameterByte;

    if (this->contents[effectiveAddress] == parameterByte) this->setFlag();
}

void Memory::displayRange(const address &start, const address &stop){
    if (start >= this->capacity || stop >= this->capacity){
        throw std::runtime_error("Out of range");
    }
    if (start > stop){
        throw std::runtime_error("Empty range. Check if start address is less than stop address.");
    }
    for (std::size_t i = start ; i <= stop ; ++i){
        std::cout << i << ":      " << this->contents[i] << '\n';
    }
}

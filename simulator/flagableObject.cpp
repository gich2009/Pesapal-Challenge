#include "flagableObject.h"

bool FlagableObject::checkFlag(){
    return this->flag;
}

void FlagableObject::setFlag(){
    this->flag = true;
}

void FlagableObject::clearFlag(){
    this->flag = false;
}
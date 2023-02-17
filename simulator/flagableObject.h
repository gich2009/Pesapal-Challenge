#ifndef FLAGABLEOBJECT_H
#define FLAGABLEOBJECT_H

//A class that forms the base for a flagable object. Has the following members:
//  i. checkFlag() that checks the current flag state.
// ii. clearFlag() that clears the flag.
//iii. setFlag()   that sets the flag.
class FlagableObject{
    public:

    //Returns the state of the flag of the object on which it is called. returns true if the flag is set and false if
    //the flag is clear.
    bool checkFlag();

    //Clears the flag of the object on which it is called.
    void clearFlag();

    //Clears the flag of the object on which it is called.
    void setFlag();

    private:
    bool flag = false;
};


#endif
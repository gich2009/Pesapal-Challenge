#ifndef GLOBALHELPER_H
#define GLOBALHELPER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cstring>

//Converts a string to an unsigned integer.
std::size_t toNumber(const std::string &string);

//Converts an unsigned integer to a string.
std::string toString(const std::size_t &number);

//Takes an integer and converts its value from decimal to binary. Returns the binary number as a string.
//Safest to use because conversion will be correct in the case of negative numbers as well.
std::string decimalToBinary(int n);

//Takes an integer and converts its value from decimal to binary. Returns the binary number as a string.
std::string decimalToBinary(std::size_t n);

//Takes an integer and converts its value from decimal to binary. Returns the binary number as an int.
int         decimalToBinaryInt(int n);

//Takes an integer and converts its value from decimal to binary. Returns the binary number as an unsigned int.
std::size_t decimalToBinaryInt(std::size_t n);

//Takes a word as a string and converts it to hexadecimal. Returns the hexadecimal result as a string.
std::string toHexadecimal(const std::string word);

//Takes a nibble of a word in form of a string converts it to a hexadecimal digit.
std::string hexDigit(const std::string nibble);


constexpr std::size_t byteSize = 8U;
constexpr std::size_t wordSize = 32U;
const std::string zeroValue = "0";
const std::string codeSegmentInitial = "1100111111111111";

enum {NODEBUG = 0, DEBUGRUN = 1} DEBUG = NODEBUG;

//HALTFLAG          = 00000000000000000000000000000001
//NOP               = 00000000000000000000000000000010
//LIFLAG            = 00000000000000000000000000000100
//LWFLAG            = 00000000000000000000000000001000
//SWFLAG            = 00000000000000000000000000010000
//ADDFLAG           = 00000000000000000000000000100000
//SUBFLAG           = 00000000000000000000000001000000
//MULTFLAG          = 00000000000000000000000010000000
//DIVFLAG           = 00000000000000000000000100000000
//JFLAG             = 00000000000000000000001000000000
//JRLAG             = 00000000000000000000010000000000
//BEQFLAG           = 00000000000000000000100000000000
//BNEFLAG           = 00000000000000000001000000000000
//INCFLAG           = 00000000000000000010000000000000
//DECFLAG           = 00000000000000000100000000000000
//ADDRESSFIELD1_R1  = 00000000000000001000000000000000
//ADDRESSFIELD1_R2  = 00000000000000010000000000000000
//ADDRESSFIELD1_R3  = 00000000000000100000000000000000
//ADDRESSFIELD2_R1  = 00000000000001000000000000000000
//ADDRESSFIELD2_R2  = 00000000000010000000000000000000
//ADDRESSFIELD2_R3  = 00000000000100000000000000000000
//ADDRESSFIELD3_R1  = 00000000001000000000000000000000
//ADDRESSFIELD3_R2  = 00000000010000000000000000000000
//ADDRESSFIELD3_R3  = 00000000100000000000000000000000
enum FLAG {HALTFLAG = 1, NOPFLAG = 2, LIFLAG = 4, LWFLAG = 8, SWFLAG = 16, ADDFLAG = 32, SUBFLAG = 64, MULTFLAG = 128, DIVFLAG = 256,
            JFLAG = 512, JRFLAG = 1024, BEQFLAG = 2048, BNEFLAG = 4096, INCFLAG = 8192, DECFLAG = 16384, ADDRESSFIELD1_R1 = 32768, 
            ADDRESSFIELD1_R2 = 65536, ADDRESSFIELD1_R3 = 131072, ADDRESSFIELD2_R1 = 262144, ADDRESSFIELD2_R2 = 524288, ADDRESSFIELD2_R3 = 1048576, 
            ADDRESSFIELD3_R1 = 2097152, ADDRESSFIELD3_R2 = 4194304, ADDRESSFIELD3_R3 = 8388608};

#endif
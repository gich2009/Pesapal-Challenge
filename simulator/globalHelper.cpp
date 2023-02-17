#include "globalHelper.h"

std::string toString(const std::size_t &number){
    std::ostringstream out;
    out << number;
    return out.str();
}

std::size_t toNumber(const std::string &string){
    std::size_t number = strtoul(string.c_str(), 0, 2);

    return number;
}


//Safest to use because conversion will be correct in the case of negative numbers as well.
std::string decimalToBinary(int n){
  std::ostringstream out;
  std::vector<int> binary;

  while (n > 0) {
    binary.push_back(n % 2);
    n /= 2;
  }

  for (int i = binary.size() - 1; i >= 0; --i) {
    out << binary[i];
  }
  
  return out.str();
}


//Safest to use because conversion will be correct in the case of negative numbers as well.
std::string decimalToBinary(std::size_t n){
  std::ostringstream out;
  std::vector<int> binary;

  while (n > 0) {
    binary.push_back(n % 2);
    n /= 2;
  }

  for (int i = binary.size() - 1; i >= 0; --i) {
    out << binary[i];
  }
  
  return out.str();
}


//Dangerous and undefined when the argument is negative.
int decimalToBinaryInt(int n){
  int binary = 0;
  int place_value = 1;

  while (n > 0) {
    int last_digit = n % 2;
    binary += last_digit * place_value;
    place_value *= 10;
    n /= 2;
  }

  return binary;
}


//Dangerous and undefined when the argument is negative.
std::size_t decimalToBinaryInt(std::size_t n){
  std::size_t binary = 0;
  std::size_t place_value = 1;

  while (n > 0) {
    std::size_t last_digit = n % 2;
    binary += last_digit * place_value;
    place_value *= 10;
    n /= 2;
  }

  return binary;
}


std::string toHexadecimal(const std::string word){
    std::string hexadecimal = "0x";
    std::size_t thereAreNibbles = (wordSize / (byteSize / 2));
    std::size_t indexPos = 0U;

    while (thereAreNibbles){
        hexadecimal += hexDigit(word.substr(indexPos, byteSize / 2));
        indexPos += byteSize / 2;
        --thereAreNibbles;
    }
    
    return hexadecimal;
}


std::string hexDigit(const std::string nibble){
    if (nibble == "0000"){
        return "0";
    } 
    if (nibble == "0001"){
        return "1";
    } 
    if (nibble == "0010"){
        return "2";
    } 
    if (nibble == "0011"){
        return "3";
    }
    if (nibble == "0100"){
        return "4";
    }
    if (nibble == "0101"){
        return "5";
    }
    if (nibble == "0110"){
        return "6";
    }
    if (nibble== "0111"){
        return "7";
    }
    if (nibble == "1000"){
        return "8";
    }
    if (nibble == "1001"){
        return "9";
    }
    if (nibble == "1010"){
        return "A";
    }
    if (nibble == "1011"){
        return "B";
    }
    if (nibble == "1100"){
        return "C";
    }
    if (nibble == "1101"){
        return "D";
    }
    if (nibble == "1110"){
        return "E";
    }
    if (nibble == "1111"){
        return "F";
    }

    return "";
}
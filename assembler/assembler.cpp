#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <map>
#include <unistd.h>
#include <direct.h>

void toLower (std::string &myString);
void createFile(const std::string &fileName);
void classA(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile);
void classB(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile);
void classC(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile);
void classD(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile);
void classE(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile);
void classF(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile);
bool makeByte(std::string &contents);
bool makeHalfWord(std::string &contents);
std::string hextoBinary(const std::string &string);
std::string decimalToBinary(std::size_t n);

constexpr std::size_t fileNameMax = 300;
constexpr std::size_t byteSize = 8U;
constexpr std::size_t wordSize = 32U;
const     std::string zeroValue(byteSize, '0');
std::size_t absoluteLineCount = 0;
std::map<const std::string, std::string> identifierMap;



int main(int argc, char *argv[]){
    chdir("..");
    char currentWorkingDirectory[fileNameMax];
    getcwd(currentWorkingDirectory, fileNameMax);

    std::string rootDirectory = currentWorkingDirectory;
    std::string fileName  = "";
    std::string txtDirectory = "\\.txt\\";
    std::string binaryTxtDirectory = "\\.binary.txt\\";
    const char  *extension1 = ".binary";
    const char  *extension2 = ".txt"; 
    int lineCount = -1;
    

    while (fileName.empty()){
        std::cout << "Enter the file name of the .txt file containing the "
                  << "assembly language: ";
        std::getline(std::cin, fileName);
    }

    //Check if the fileName has an extension such as .txt or .html or .css or .cpp or .doc
    std::string::const_iterator found = std::find(fileName.cbegin(), fileName.cend(), '.');

    //Remove the extension if it exists
    if (found != fileName.cend()){
        for (std::string::iterator i = (fileName.end() - 1) ; *i != '.' ; --i){
            fileName.pop_back();
        }
        fileName.pop_back();
    }
    std::cout << "\n" << fileName << std::endl;

    //Create a file with the same name but with a different extension.
    std::ofstream outputFile(currentWorkingDirectory + binaryTxtDirectory + fileName + extension1 + extension2, std::ios_base::out|std::ios_base::ate);

    if (!outputFile.is_open()){
        createFile(currentWorkingDirectory + binaryTxtDirectory + fileName + extension1 + extension2);
        outputFile.open(currentWorkingDirectory + binaryTxtDirectory + fileName + extension1 + extension2, std::ios_base::out|std::ios_base::ate);

        if (!outputFile.is_open()){
            std::cout << "Output file did not open" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
 
    std::ifstream inputFile(currentWorkingDirectory + txtDirectory + fileName  + extension2, std::ios_base::in);
    
    if (!inputFile.is_open()){
        std::cout << "Input file did not open." << std::endl;
        return 0;
    }

    outputFile.seekp(std::ios_base::beg);
    inputFile.seekg(std::ios_base::beg);


    for (std::string lineBuffer ; inputFile.peek() != EOF ; ){
        ++absoluteLineCount;
        std::getline(inputFile, lineBuffer);
        toLower(lineBuffer);
        std::cout << "\n" << lineBuffer << std::endl;
        std::istringstream readLine(lineBuffer);
        std::string instructionMnemonic = "";
        readLine >> instructionMnemonic;
        

        if (instructionMnemonic.empty() || instructionMnemonic[0] == ';'){
            continue;
        }


        if (instructionMnemonic == "halt" || instructionMnemonic == "nop"){
            // if (lineBuffer.size() != 1) throw std::runtime_error("Syntax error");
            classA(instructionMnemonic, lineCount, outputFile);          //its all the same to pass the linebuffer in this statement.

        } else if (instructionMnemonic == "j"){
            // if (lineBuffer.size() != 2) throw std::runtime_error("Syntax error");
            classB(lineBuffer, lineCount, outputFile);

        } else if (instructionMnemonic == "inc" || instructionMnemonic == "dec" || instructionMnemonic == "jr"){
            // if (lineBuffer.size() != 2) throw std::runtime_error("Syntax error");
            classC(lineBuffer, lineCount, outputFile);

        } else if (instructionMnemonic == "lw" || instructionMnemonic == "sw"){
            // if (lineBuffer.size() != 3) throw std::runtime_error("Syntax error");
            classD(lineBuffer, lineCount, outputFile);

        } else if (instructionMnemonic == "add" || instructionMnemonic == "sub" || instructionMnemonic == "mult" || 
                   instructionMnemonic == "div" || instructionMnemonic == "beq" || instructionMnemonic == "bne"){

            // if (lineBuffer.size() != 4) throw std::runtime_error("Syntax error");
            classE(lineBuffer, lineCount, outputFile);

        }else if (instructionMnemonic == "li"){
            // if (lineBuffer.size() != 3) throw std::runtime_error("Syntax error");
            classF(lineBuffer, lineCount, outputFile);

        } else{
            if (instructionMnemonic.back() == ':') instructionMnemonic.pop_back();

            if (identifierMap.find(instructionMnemonic) == identifierMap.cend()){
                throw std::runtime_error("Inalid instruction");      //exception case if the instructionMnemonic is not provided.
            }
        }
    }

    std::cout << "Press return to exit: " << std::flush;
    
    getchar();

    return 0;
}


void toLower (std::string &myString){
    for (auto i = 0U ; i != myString.size() ; ++i){
        myString[i] = tolower(myString[i]);
    }
}


void createFile(const std::string &fileName){
    std::ofstream newFile{fileName, std::ios_base::out};

    newFile.close(); 
}


void classA(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile){
    std::string byteBuffer = "0000";
    if (lineBuffer == "halt"){
        byteBuffer += "0000";
    } else if (lineBuffer == "nop"){
        byteBuffer += "0001";
    }

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    byteBuffer = zeroValue;

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";
}


void classB(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile){
    std::string byteBuffer = "0000";

    std::istringstream readLine(lineBuffer);
    readLine >> byteBuffer;

    if (byteBuffer == "li"){
        const char *temp = "0010";
        byteBuffer += temp;
    } 
   
    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    byteBuffer = zeroValue;

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";


    readLine >> byteBuffer;
    if (byteBuffer[0] == '0'){                // include the case where a number begins with '1' and the case where number is binary.
        std::string immediateValue = hextoBinary(byteBuffer);

        ++lineCount;
        outputFile << immediateValue.substr((wordSize / 2 - byteSize), (wordSize / 4));
        outputFile << "\n";

        ++lineCount;
        outputFile << immediateValue.substr(((wordSize / 2) - (2 * byteSize)), (wordSize / 4));
        outputFile << "\n";
    } else {
        std::size_t address = lineCount + 3U;
        if (identifierMap.find(byteBuffer) == identifierMap.end()){
            std::string addressString = decimalToBinary(address);
            makeHalfWord(addressString);
            identifierMap[byteBuffer] = addressString;
        }

        if (identifierMap.find(byteBuffer) != identifierMap.end()){
            byteBuffer = identifierMap[byteBuffer];
        }

        ++lineCount;
        outputFile << byteBuffer.substr((wordSize / 2 - byteSize), (wordSize / 4));
        outputFile << "\n";

        ++lineCount;
        outputFile << byteBuffer.substr(((wordSize / 2) - (2 * byteSize)), (wordSize / 4));
        outputFile << "\n";
    }

}

void classC(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile){
    std::string byteBuffer = "";

    std::istringstream readLine(lineBuffer);
    std::string instructionMnemonic = "";
    std::string addressfield1 = "";

    readLine >> instructionMnemonic;
    readLine >> addressfield1;

    if (addressfield1 == "r1"){
        byteBuffer += "0001";
    } else if (addressfield1 == "r2"){
        byteBuffer += "0010";
    } else if (addressfield1 == "r3"){
        byteBuffer += "0011";
    }


    if (instructionMnemonic == "jr"){
        byteBuffer += "1010";
    } else if (instructionMnemonic == "inc"){
        byteBuffer += "1101";
    } else if (instructionMnemonic == "dec"){
        byteBuffer += "1110";
    }
   
    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    byteBuffer = zeroValue;

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";
}


void classD(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile){
    std::string byteBuffer = "";

    std::istringstream readLine(lineBuffer);
    std::string instructionMnemonic = "";
    std::string addressfield1 = "";
    std::string addressfield2 = "";

    readLine >> instructionMnemonic;
    readLine >> addressfield1;
    readLine >> addressfield2;

    if (addressfield1 == "r1"){
        byteBuffer += "0001";
    } else if (addressfield1 == "r2"){
        byteBuffer += "0010";
    } else if (addressfield1 == "r3"){
        byteBuffer += "0011";
    }


    if (instructionMnemonic == "lw"){
        byteBuffer += "0011";
    } else if (instructionMnemonic == "sw"){
        byteBuffer += "0100";
    } 
   
    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";


    byteBuffer = "0000";

    if (addressfield2 == "r1"){
        byteBuffer += "0001";
    } else if (addressfield2 == "r2"){
        byteBuffer += "0010";
    } else if (addressfield2 == "r3"){
        byteBuffer += "0011";
    }

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";


    byteBuffer = zeroValue;

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";
}


void classE(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile){
    std::string byteBuffer = "";

    std::istringstream readLine(lineBuffer);
    std::string instructionMnemonic = "";
    std::string addressfield1 = "";
    std::string addressfield2 = "";
    std::string addressfield3 = "";

    readLine >> instructionMnemonic;
    readLine >> addressfield1;
    readLine >> addressfield2;
    readLine >> addressfield3;

    if (addressfield1 == "r1"){
        byteBuffer += "0001";
    } else if (addressfield1 == "r2"){
        byteBuffer += "0010";
    } else if (addressfield1 == "r3"){
        byteBuffer += "0011";
    }


    if (instructionMnemonic == "add"){
        byteBuffer += "0101";
    } else if (instructionMnemonic == "sub"){
        byteBuffer += "0110";
    } else if (instructionMnemonic == "mult"){
        byteBuffer += "0111";
    } else if (instructionMnemonic == "div"){
        byteBuffer += "1000";
    } else if (instructionMnemonic == "beq"){
        byteBuffer += "1011";
    } else if (instructionMnemonic == "bne"){
        byteBuffer += "1100";
    } 
   
    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";


    byteBuffer = "";

    if (addressfield3 == "r1"){
        byteBuffer += "0001";
    } else if (addressfield3 == "r2"){
        byteBuffer += "0010";
    } else if (addressfield3 == "r3"){
        byteBuffer += "0011";
    }

    if (addressfield2 == "r1"){
        byteBuffer += "0001";
    } else if (addressfield2 == "r2"){
        byteBuffer += "0010";
    } else if (addressfield2 == "r3"){
        byteBuffer += "0011";
    }

    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";


    byteBuffer = zeroValue;

    
    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    
    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";
}


void classF(const std::string &lineBuffer, int &lineCount, std::ostream &outputFile){
    std::string byteBuffer = "";

    std::istringstream readLine(lineBuffer);
    std::string instructionMnemonic = "";
    std::string addressfield1 = "";

    readLine >> instructionMnemonic;
    readLine >> addressfield1;

    if (addressfield1 == "r1"){
        byteBuffer += "0001";
    } else if (addressfield1 == "r2"){
        byteBuffer += "0010";
    } else if (addressfield1 == "r3"){
        byteBuffer += "0011";
    }

    if (instructionMnemonic == "li"){
        byteBuffer += "0010";
    } 
   
    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    byteBuffer = zeroValue;

    
    ++lineCount;
    outputFile << byteBuffer;
    outputFile << "\n";

    readLine >> byteBuffer;

    if (byteBuffer[0] == '0'){        // include the case where a number begins with '1' and the case where number is binary.
        std::string immediateValue = hextoBinary(byteBuffer);

        ++lineCount;
        outputFile << immediateValue.substr((wordSize / 2 - byteSize), (wordSize / 4));
        outputFile << "\n";

        ++lineCount;
        outputFile << immediateValue.substr(((wordSize / 2) - (2 * byteSize)), (wordSize / 4));
        outputFile << "\n";
    } else {
       
        std::size_t address = lineCount + 3U;
        if (identifierMap.find(byteBuffer) == identifierMap.end()){
            std::string addressString = decimalToBinary(address);
            makeHalfWord(addressString);
            identifierMap[byteBuffer] = addressString;
        }

        if (identifierMap.find(byteBuffer) != identifierMap.end()){
            byteBuffer = identifierMap[byteBuffer];

            ++lineCount;
            outputFile << byteBuffer.substr((wordSize / 2 - byteSize), (wordSize / 4));
            outputFile << "\n";

            ++lineCount;
            outputFile << byteBuffer.substr(((wordSize / 2) - (2 * byteSize)), (wordSize / 4));
            outputFile << "\n";
        }

        
    }
}


std::string hextoBinary(const std::string &string){
    std::string::const_iterator stringIterator = string.cbegin();
    std::string buffer = "";
    std::size_t thereAreNibbles = 4U;

    if(string.empty()) throw std::runtime_error("No immediate operand.");

    if (*(stringIterator + 1) != 'x' && *(stringIterator + 1) != 'X'){
            std::ostringstream errorMessage;
            errorMessage << "On line: " << absoluteLineCount << ". Immediate operand must be a hexadecimal value.";
            throw std::runtime_error(errorMessage.str());
            std::cout << "\n" << "Im in the stringIterator loop" << std::endl;
            return "";      //deal with an immediate operand that is not a hexadecimal.
    }

    stringIterator += 2;

    while (*stringIterator == '0' && stringIterator != (string.cend() - 4)) ++stringIterator;

    while (stringIterator != string.cend() && thereAreNibbles){
        if (*stringIterator == '0'){
            buffer += "0000";
        } else if (*stringIterator == '1'){
            buffer += "0001";
        } else if (*stringIterator == '2'){
            buffer += "0010";
        } else if (*stringIterator == '3'){
            buffer += "0011";
        } else if (*stringIterator == '4'){
            buffer += "0100";
        } else if (*stringIterator == '5'){
            buffer += "0101";
        } else if (*stringIterator == '6'){
            buffer += "0110";
        } else if (*stringIterator == '7'){
            buffer += "0111";
        } else if (*stringIterator == '8'){
            buffer += "1000";
        } else if (*stringIterator == '9'){
            buffer += "1001";
        } else if (*stringIterator == 'A' || *stringIterator == 'a'){
            buffer += "1010"; 
        } else if (*stringIterator == 'B' || *stringIterator == 'b'){
            buffer += "1011";
        } else if (*stringIterator == 'B' || *stringIterator == 'b'){
            buffer += "1011";
        } else if (*stringIterator == 'B' || *stringIterator == 'b'){
            buffer += "1011";
        } else if (*stringIterator == 'C' || *stringIterator == 'c'){
            buffer += "1100";
        } else if (*stringIterator == 'D' || *stringIterator == 'd'){
            buffer += "1101";
        } else if (*stringIterator == 'E' || *stringIterator == 'e'){
            buffer += "1110";
        } else if (*stringIterator == 'F' || *stringIterator == 'f'){
            buffer += "1111";
        } else {
            std::ostringstream errorMessage;
            errorMessage << "On line: " << absoluteLineCount << " invalid hexdigit " << *stringIterator << " found.";
            throw std::runtime_error(errorMessage.str());
        }
        ++stringIterator;
        --thereAreNibbles;
    }

    return buffer;
}


bool makeByte(std::string &contents){

    if(contents.size() >= byteSize){
        return false;
    }

    std::string shortContents     = contents;
    std::size_t thereIsDifference = byteSize - contents.size();
    contents.clear();
    
    while(thereIsDifference){
        contents.append("0");  //or this->contents += "0";
        --thereIsDifference;
    }

    contents += shortContents;
    
    return (contents.size() == byteSize)? true : false;
}


bool makeHalfWord(std::string &contents){

    if(contents.size() >= (wordSize / 2)){
        return false;
    }

    std::string shortContents     = contents;
    std::size_t thereIsDifference = (wordSize / 2) - contents.size();
    contents.clear();
    
    while(thereIsDifference){
        contents.append("0");  //or this->contents += "0";
        --thereIsDifference;
    }

    contents += shortContents;
    
    return (contents.size() == (wordSize / 2))? true : false;
}


std::string toString(const std::size_t &number){
    std::ostringstream out;
    out << number;
    return out.str();
}




std::string decimalToBinary(std::size_t n){
  std::size_t binary = 0;
  std::size_t place_value = 1;

  while (n > 0) {
    std::size_t last_digit = n % 2;
    binary += last_digit * place_value;
    place_value *= 10;
    n /= 2;
  }

  return toString(binary);
}
#include <exception>
#include <string>
#include <iostream>
#include <fstream>
struct SyntaxException
{
    std::string fileName_;
    SyntaxException(std::string fileName): fileName_(fileName){};

    void Throw(const std::string errorType_, const std::string token_, const std::string lexeme_)
    {
        std::ofstream outFile(fileName_);
        outFile << "SYNTAX ERROR: EXPECTED " << errorType_ << " RECEIVED: TOKEN: " << token_ << " | Lexeme: " << lexeme_ << "\n";
        outFile.close();
        std::exit(-1);
    }
};
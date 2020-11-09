#include <exception>
#include <string>
#include <iostream>
struct SyntaxException
{

    SyntaxException()=default;

    void Throw(const std::string errorType_, const std::string token_, const std::string lexeme_)
    {
        std::cout << "SYNTAX ERROR: EXPECTED " << errorType_ << " RECEIVED: TOKEN: " << token_ << " | Lexeme: " << lexeme_ << "\n";
        std::exit(-1);
    }
};
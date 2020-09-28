#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "FSM.cpp"
#include <unordered_map>
#include <set>
#include <string>

class Lexical_Analyzer
{
    public:
      Lexical_Analyzer();    

      // Main function to be called
      std::unordered_map<std::string,std::string> lexer();


    private:
        // Finite number of elements
        const std::set<std::string> _keywords {"if", "fi", "int", "boolean", "real","else", "return", "true", "false", "put","get", "while" };
        const std::set<std::string> _separators {"(", ")", ";", "{","}" ,"\\*", "*/", ","};
        const std::set<std::string> _operators {"=", "==", "<", ">", ">=", "<=", "+", "-", "/", "!=", "!","*"};
};
#endif
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

      // Main function, that will compute the lexemes
      std::vector<std::vector<std::string>> lexer(std::string);

      // Determines what type of Token the Lexeme is
      std::string whatTokenType(std::string&);

      // Helper functions for the WhatTokenType()
      std::string isKeyword(std::string&);
      std::string isDouble(std::string&);
      std::string isOperator(std::string&);

    private:
        // An instance of our FSM to be used
        Fsm mainMachine_;

        // Keep track of what our operators and keywords are
        const std::set<std::string> _keywords {"if", "fi", "int", "boolean", "real","else", "return", "true", "false", "put","get", "while" };
        const std::set<std::string> _operators {"=", "==", "<", ">", ">=", "<=", "+", "-", "/", "!=", "!","*"};
};
#endif
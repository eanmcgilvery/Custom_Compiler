#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "DFMS.cpp"
#include "NFSM.cpp"
#include <unordered_set>
#include <set>

class Lexical_Analyzer
{
    public:
      Lexical_Analyzer();    

      // Main function to be called
      std::unordered_set<std::string,std::string> lexer();


    private:
        DFSM _integer_DFSM;
        DFSM _real_DFSM;
        DFSM _identifier_DFSM;
        
        NFSM _integer_NFSM;
        NFSM _real_NFSM;
        NFSM _identifier_NFSM;


        // Finite number of elements
        const std::set<std::string> _keywords = {"if", "fi", "int", "boolean", "real",
                                                 "else", "return", "true", "false", "put","get", "while", };
                                                 
        const std::set<std::string> _separators = {"(", ")", ";", "{","}" "\\*", "*/", ","};

        const std::set<std::string> _operators = {"=", "==", "<", ">", ">=", "<=", "+", "-", "/",
                                                  "!=", "!" "*"};
};
#endif
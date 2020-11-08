#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <string>
#include <vector>
#include <queue>

class Syntax_Analyzer
{
    public:
        Syntax_Analyzer() = delete;
        Syntax_Analyzer(const std::vector<std::vector<std::string>>&);

        void Parse();
        std::queue<std::string> printStack;

    private:
        std::vector<std::vector<std::string>> _tableOfLexemes;

        size_t lexemeCounter;
        bool flagEmpty;
        bool deepestFlag;
        std::string tokenFromBelow;

        void nextLexeme(std::string&);
        std::string tempNextLexeme(std::string);
    /*==========================================================================================
    //
    //                                       Syntax Rules
    //
    //========================================================================================*/       
        bool Rat20F(std::string);
        bool OptFuncDef(std::string);
        bool FuncDef(std::string);
        bool Func(std::string);
        bool OptParamList(std::string);
        bool ParamList(std::string);
        bool Parameter(std::string);
        bool Qualifiers(std::string);
        bool Body(std::string);
        bool OptDeclarationList(std::string);
        bool DeclarationList(std::string);
        bool Decleration(std::string);
        bool Ids(std::string);
        bool StatementList(std::string);
        bool Statement(std::string);
        bool Compound(std::string);
        bool Assign(std::string);
        bool If(std::string);
        bool Return(std::string);
        bool Print(std::string);
        bool Scan(std::string);
        bool While(std::string);
        bool Condition(std::string);
        bool Relop(std::string);
        bool Expression(std::string);
        bool ExpressionPrime(std::string);
        bool Term(std::string);
        bool TermPrime(std::string);
        bool Factor(std::string);
        bool Primary(std::string);
        bool Empty(std::string);
};
#endif
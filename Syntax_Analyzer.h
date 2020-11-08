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
        bool FuncDefV1(std::string);
        bool FuncDefV2(std::string);
        bool Func(std::string);
        bool OptParamList(std::string);
        bool ParamListV1(std::string);
        bool ParamListV2(std::string);
        bool Parameter(std::string);
        bool Qualifiers(std::string);
        bool Body(std::string);
        bool OptDeclarationList(std::string);
        bool DeclarationListV1(std::string);
        bool DeclarationListV2(std::string);
        bool Decleration(std::string);
        bool IdsV1(std::string);
        bool IdsV2(std::string);
        bool StatementListV1(std::string);
        bool StatementListV2(std::string);
        bool Statement(std::string);
        bool Compound(std::string);
        bool Assign(std::string);
        bool IfV1(std::string);
        bool IfV2(std::string);
        bool ReturnV1(std::string);
        bool ReturnV2(std::string);
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
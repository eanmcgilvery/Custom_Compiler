#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include <string>
#include <vector>
#include <queue>
#include "MyException.cpp"
#include "ObjCodeTable.cpp"
#include <stack>
class Syntax_Analyzer
{
public:
    Syntax_Analyzer() = delete;
    Syntax_Analyzer(const std::vector<std::vector<std::string>>&, const std::string &);

    void Parse();
    std::queue<std::string> printStack;

    int instrAddress = 1;
    int addr = 0;
    int memAddr = 5000;
    int symbCount = 0;

    std::stack<int> jumpstack;

    ObjCodeTable instrTable[1000];
    std::string symbolTable[1000];
    std::string symbolType[1000];

private:
    std::vector<std::vector<std::string>> _tableOfLexemes;

    size_t lexemeCounter;
    std::string fileName_;

    SyntaxException error_;
    void        nextLexeme(std::string&);
    void        gen_instr(std::string, int);
    void        back_patch(int);
    int         get_addr(std::string);
    bool        is_duplicate(std::string input);
    std::string findNearestKeyword(std::string);
    void        printInstr();
    /*==========================================================================================
    //
    //                                       Syntax Rules
    //
    //========================================================================================*/
    bool Rat20F(std::string&);
    bool OptFuncDef(std::string&);
    bool FuncDef(std::string&);
    bool Func(std::string&);
    bool OptParamList(std::string&);
    bool ParamList(std::string&);
    bool Parameter(std::string&);
    bool Qualifiers(std::string&);
    bool Body(std::string&);
    bool OptDeclarationList(std::string&);
    bool DeclarationList(std::string&);
    bool Decleration(std::string&);
    bool Ids(std::string&, bool, bool);
    bool StatementList(std::string&);
    bool Statement(std::string&);
    bool Compound(std::string&);
    bool Assign(std::string&);
    bool If(std::string&);
    bool Return(std::string&);
    bool Print(std::string&);
    bool Scan(std::string&);
    bool While(std::string&);
    bool Condition(std::string&);
    bool Relop(std::string&);
    bool Expression(std::string&, bool);
    bool ExpressionPrime(std::string&);
    bool Term(std::string&, bool);
    bool TermPrime(std::string&);
    bool Factor(std::string&, bool);
    bool Primary(std::string&, bool);
    bool Empty(std::string&);
};
#endif
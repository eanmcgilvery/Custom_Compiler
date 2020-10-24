#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

class Syntax_Analyzer
{
    public:
        Syntax_Analyzer();
    private:

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
        bool Term(std::string);
        bool Factor(std::string);
        bool Primary(std::string);
        bool Empty(std::string);
};
#endif
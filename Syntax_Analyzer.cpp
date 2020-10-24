#include "Syntax_Analyzer.h"
#include "Lexical_Analyzer.h"
#include <iostream>
#include <string>

/*==========================================================================================
//
//                                       Syntax Rules
//
//========================================================================================*/


/*==========================================================================================
//
//                                          TO DO
//
// - Modify Lexical_Analyzer to be able to grab a token within each function call in the Syntax
//    analyzer rule functions. Perhaps need to have a static wrapper for lexer()?
// - 
//
//========================================================================================*/ 




bool Syntax_Analyzer::Rat20F(std::string token)
{
    std::cout << "Rat20F -> <Opt Function Definitions>\n";
    if(OptFuncDef(token))
    {
        std::string token;//LEXER CALL HERE    
        if (token == "$$")
        {
            if (OptDeclarationList(token));
            {
                if (StatementList(token))
                {
                    std::string token;//LEXER CALL HERE 
                     return token == "$$";
                }
            }
        }
    }
    return false;
}
bool Syntax_Analyzer::OptFuncDef(std::string token)
{

}
bool Syntax_Analyzer::FuncDef(std::string token)
{

}
bool Syntax_Analyzer::Func(std::string token)
{

}
bool Syntax_Analyzer::OptParamList(std::string token)
{

}
bool Syntax_Analyzer::ParamList(std::string token)
{

}
bool Syntax_Analyzer::Parameter(std::string token)
{

}
bool Syntax_Analyzer::Qualifiers(std::string token)
{

}
bool Syntax_Analyzer::Body(std::string token)
{

}
bool Syntax_Analyzer::OptDeclarationList(std::string token)
{

}
bool Syntax_Analyzer::DeclarationList(std::string token)
{

}
bool Syntax_Analyzer::Decleration(std::string token)
{

}
bool Syntax_Analyzer::Ids(std::string token)
{

}
bool Syntax_Analyzer::StatementList(std::string token)
{

}
bool Syntax_Analyzer::Statement(std::string token)
{

}
bool Syntax_Analyzer::Compound(std::string token)
{

}
bool Syntax_Analyzer::Assign(std::string token)
{

}
bool Syntax_Analyzer::If(std::string token)
{

}
bool Syntax_Analyzer::Return(std::string token)
{

}
bool Syntax_Analyzer::Print(std::string token)
{

}
bool Syntax_Analyzer::Scan(std::string token)
{

}
bool Syntax_Analyzer::While(std::string token)
{

}
bool Syntax_Analyzer::Condition(std::string token)
{

}
bool Syntax_Analyzer::Relop(std::string token)
{

}
bool Syntax_Analyzer::Expression(std::string token)
{

}
bool Syntax_Analyzer::Term(std::string token)
{

}
bool Syntax_Analyzer::Factor(std::string token)
{

}
bool Syntax_Analyzer::Primary(std::string token)
{

}
bool Syntax_Analyzer::Empty(std::string token)
{

}
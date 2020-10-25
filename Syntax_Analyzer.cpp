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
// - Answer to the above: In a non sophistcated way we can do the following -> In main, compute all 
//   the lexemes, as we already have done in our code. From here we can then copy this table within a 
//   constructor in the Syntax_Analyzer(). Once we have the table in our parse tree productions, we 
//   then "chunk" away off of it, grabbing lexeme by lexeme as we would normally want to.
// - Answer to the above: A more sophistcated way would be to break down our lexer() and find a way to
//   have it only return one Token at a time. As of now I'm not sure how to do this becuase the way
//   we handle splitting strings in the lexer isn't very modular. 
//
// - After deciding how to resolve the issue above, figure out where to place the lexer calls within the 
//    syntax rules functions.
//========================================================================================*/ 



//R1 
bool Syntax_Analyzer::Rat20F(std::string token)
{
    std::cout << "<Rat20F> -> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$\n";
    if(OptFuncDef(token))
    {
        //LEXER CALL HERE    
        if (token == "$$")
        {
            //LEXER CALL HERE 
            if (OptDeclarationList(token));
            {
                //LEXER CALL HERE 
                if (StatementList(token))
                {
                     //LEXER CALL HERE 
                     return token == "$$";
                }
            }
        }
    }
    return false;
}

//R2
bool Syntax_Analyzer::OptFuncDef(std::string token)
{
    std::cout << "<Opt Function Definitions> → <Function Definitions> | <Empty>\n";
    return FuncDef(token) || Empty(token); //QUESTIONABLE, COME BACK TO THIS B/C it will always be true
}

//R3
bool Syntax_Analyzer::FuncDef(std::string token)
{
    std::cout << "<Function Definitions> → <Function> | <Function> <Function Definitions>\n";
    if (Func(token))
    {
        FuncDef(token);
        return true; // We return true here because R3 says Func OR (Func and FuncDef)
    }
    return false;
}

//R4
bool Syntax_Analyzer::Func(std::string token)
{
    if(token == "function")
    {
        if(Ids(token))
        {
            if (token == "(")
            {
                if (OptParamList(token))
                {
                    if (token == ")")
                    {
                        if(OptDeclarationList(token))
                        {
                            return Body(token);
                        }
                    }
                }
            }
        }
    }
    return false;
}

//R5
bool Syntax_Analyzer::OptParamList(std::string token)
{
    return ParamList(token) || Empty(token);
}

//R6
bool Syntax_Analyzer::ParamList(std::string token)
{
    if(Parameter(token))
    {
        if(token == ",")
            ParamList(token);
        return true;
    }
    return false;
}

//R7
bool Syntax_Analyzer::Parameter(std::string token)
{
    if (Ids(token))
    {
        return Qualifiers(token);
    }
}

//R8
bool Syntax_Analyzer::Qualifiers(std::string token)
{

}

//R9
bool Syntax_Analyzer::Body(std::string token)
{

}

//R10
bool Syntax_Analyzer::OptDeclarationList(std::string token)
{

}

//R11
bool Syntax_Analyzer::DeclarationList(std::string token)
{

}

//R12
bool Syntax_Analyzer::Decleration(std::string token)
{

}

//R13
bool Syntax_Analyzer::Ids(std::string token)
{

}

//R14
bool Syntax_Analyzer::StatementList(std::string token)
{

}

//R15
bool Syntax_Analyzer::Statement(std::string token)
{

}
bool Syntax_Analyzer::Compound(std::string token)
{

}

//R16
bool Syntax_Analyzer::Assign(std::string token)
{

}

//R17
bool Syntax_Analyzer::If(std::string token)
{

}

//R18
bool Syntax_Analyzer::Return(std::string token)
{

}

//R19
bool Syntax_Analyzer::Print(std::string token)
{

}

//R20
bool Syntax_Analyzer::Scan(std::string token)
{

}

//R21
bool Syntax_Analyzer::While(std::string token)
{

}

//R22
bool Syntax_Analyzer::Condition(std::string token)
{

}

//R23
bool Syntax_Analyzer::Relop(std::string token)
{

}

//R24
bool Syntax_Analyzer::Expression(std::string token)
{

}

//R25
bool Syntax_Analyzer::Term(std::string token)
{

}

//R26
bool Syntax_Analyzer::Factor(std::string token)
{

}

//R27
bool Syntax_Analyzer::Primary(std::string token)
{

}

//R28
bool Syntax_Analyzer::Empty(std::string token)
{
    std::cout << "<Empty> → _____\n";
    return true;
}
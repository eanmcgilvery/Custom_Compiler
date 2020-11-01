#include "Syntax_Analyzer.h"
#include "Lexical_Analyzer.h"
#include <iostream>
#include <string>
#include <stdexcept>

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
    if(OptFuncDef(token))
    {
        //LEXER CALL HERE    
        if (token == "$$")
        {
            //LEXER CALL HERE 
            if (OptDeclarationList(token))
            {
                //LEXER CALL HERE 
                if (StatementList(token))
                {
                     //LEXER CALL HERE 
                     
                     if(token == "$$")
                     {
                        printStack.push("<Rat20F> --> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$");
                        return true;
                     }
                }
            }
        }
    }
    throw std::range_error("ERROR: Invalide Syntax!\n");
    return false;
}

//R2
bool Syntax_Analyzer::OptFuncDef(std::string token)
{
    
    if(FuncDef(token) || Empty(token)) //QUESTIONABLE, COME BACK TO THIS B/C it will always be true
    {
        printStack.push("<Opt Function Definitions> → <Function Definitions> | <Empty>");
        return true;
    }
    return 0;
}   

//R3
bool Syntax_Analyzer::FuncDef(std::string token)
{
    if (Func(token))
    {
        FuncDef(token);
        printStack.push("<Function Definitions> → <Function> | <Function> <Function Definitions>");
        return true; // We return true here because R3 says Func OR (Func and FuncDef)
    }
    return false;
}

//R4
bool Syntax_Analyzer::Func(std::string token)
{
    if(token == "function")
    {
        if(Ids(token)) //FIX THIS: this is regarding our  identifiers 
        {
            if (token == "(")
            {
                if (OptParamList(token))
                {
                    if (token == ")")
                    {
                        if(OptDeclarationList(token))
                        {
                            if(Body(token))
                            {
                                printStack.push("<Function> → function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
                                return true;
                            }
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
    
    if(ParamList(token) || Empty(token))
    {
        printStack.push("<Opt Parameter List> → <Parameter List> | <Empty>");
        return true;
    }
    return 0;
}

//R6
bool Syntax_Analyzer::ParamList(std::string token)
{
    if(Parameter(token))
    {
        if(token == ",")
            ParamList(token);
        printStack.push("<Parameter List> → <Parameter> | <Parameter> , <Parameter List>");
        return true;
    }
    return false;
}

//R7
bool Syntax_Analyzer::Parameter(std::string token)
{
    if (Ids(token))
    {
        if(Qualifiers(token)){
            printStack.push("<Parameter> → <IDs > <Qualifier>");
            return true;
        }
    }
    return false;
}

//R8
bool Syntax_Analyzer::Qualifiers(std::string token)
{
    
    if(token =="int" || token == "boolean" || token == "real")
    {
        printStack.push("<Qualifier> → int | boolean | real");
        return true;
    }
    return false;
}

//R9
bool Syntax_Analyzer::Body(std::string token)
{
    if(token == "{"){
        if(StatementList(token)){
            
            if(token == "}")
            {
                printStack.push("<Body> → { < Statement List> }");
                return true;
            }
        }
    }
    return false;
}

//R10
bool Syntax_Analyzer::OptDeclarationList(std::string token)
{
    
    if(DeclarationList(token)  || Empty(token))
    {
        printStack.push("<Opt Declaration List> → <Declaration List> | <Empty>");
        return true;
    }
    return false;
}

//R11
bool Syntax_Analyzer::DeclarationList(std::string token)
{
     if (Decleration(token))
    {
        if(token == ";"){
            DeclarationList(token);
            printStack.push("<Declaration List> → <Declaration> ; | <Declaration> ; <Declaration List>");
            return true; // We return true here because R11 says Func OR (Declaration and DeclarationList)
        }
    }
    return false;
}

//R12
bool Syntax_Analyzer::Decleration(std::string token)
{
     if (Qualifiers(token))
    {
        if(Ids(token))
        {
            printStack.push("<Declaration> → <Qualifier > <IDs>");
            return true;
        }
    }
    return false;
}

//R13
bool Syntax_Analyzer::Ids(std::string token)
{
    if(true){//FIX THIS: this is regarding our  identifiers 
        if(token == ","){
            Ids(token);
        }
        printStack.push("<IDs> → <Identifier> | <Identifier>, <IDs>");
        return true;
    }
    return false;
}

//R14
bool Syntax_Analyzer::StatementList(std::string token)
{
    if (Statement(token))
    {
        StatementList(token);
        printStack.push("<Statement List> → <Statement> | <Statement> <Statement List>");
        return true; // We return true here because R3 says Func OR (Func and FuncDef)
    }
    return false;
}

//R15
bool Syntax_Analyzer::Statement(std::string token)
{
    if(Compound(token) || Assign(token) || If(token) || Return(token) || Print(token) ||Scan(token) ||While(token))
    {
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        return true;
    }
    return false;
}
//R16
bool Syntax_Analyzer::Compound(std::string token)
{
    if(token == "{")
    {
        if(StatementList(token))
        {
            if(token == "}")
            {
                printStack.push("<Compound> → { <Statement List> }");
                return true;
            }
        }
    }
    return false;
}

//R17
bool Syntax_Analyzer::Assign(std::string token)
{
    if(Ids(token)){ //FIX THIS: change Ids to Identifier 
        if(token=="="){
            if(Expression(token))
            {
                printStack.push("<Assign> → <Identifier> = <Expression> ;");
                return true;
            }
        }
    }
    return false;
}

//R18
bool Syntax_Analyzer::If(std::string token)
{
    if(token == "if"){
        if(token == "("){
            if(Condition(token)){
                if(token == ")"){
                    if(token == "fi"){
                        printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
                        return true;
                    }
                    if(Statement(token)){
                        if(token == "else"){
                            if(Statement(token)){
                                if(token=="fi")
                                {
                                    printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
                                    return true;
                                }
                                    
                            }
                        }

                    }
                }
            }
        }
    }
    return false;
}

//R19
bool Syntax_Analyzer::Return(std::string token)
{
    if(token == "return"){
        return token==";";
        if(Expression(token)){
            if(token == ";"){
                printStack.push("<Return> → return ; | return <Expression> ;");
                return true;
            }
            if(Expression(token)){
                if(token==";")
                {
                    printStack.push("<Return> → return ; | return <Expression> ;");
                    return true;
                }
            }
        }
    }
    return false;
}

//R20
bool Syntax_Analyzer::Print(std::string token)
{
    if (token == "put")
    {
        if(token == "(")
        {
            if(Expression(token))
            {
                if(token == ")")
                {
                    if(token == ";")
                    {
                        printStack.push("<Print> → put ( <Expression>);");
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//R21
bool Syntax_Analyzer::Scan(std::string token)
{
    if (token == "get")
    {
        if(token == "(")
        {
            if(Ids(token))
            {
                if (token == ")")
                {
                    if(token == ";")
                    {
                        printStack.push("<Scan> → get ( <IDs> );");
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//R22
bool Syntax_Analyzer::While(std::string token)
{
    if (token == "while")
    {
        if (token == "(")
        {
            if(Condition(token))
            {
                if(token == ")")
                {
                    if(Statement(token))
                    {
                        printStack.push("<While> → while ( <Condition> ) <Statement>");
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//R23
bool Syntax_Analyzer::Condition(std::string token)
{
    if(Expression(token))
    {
        if(Relop(token))
        {
            if(Expression(token))
            {
                printStack.push("<Condition> → <Expression> <Relop> <Expression>");
                return 1;
            }
        }
    }
    return false;
}

//R24
bool Syntax_Analyzer::Relop(std::string token)
{
    if(token == "==" || token == "!=" ||token == ">" ||token == "<" ||token == "<=" ||token == "=>")
    {
        printStack.push("<Relop> →  == | !=  | >  | < | <= |  =>");
        return 1;
    }
    return false;
}

//R25
bool Syntax_Analyzer::Expression(std::string token)
{
    if(Term(token))
    {
        if(ExpressionPrime(token))
        {
            printStack.push("<Expression> → <Term><ExpressionPrime>");
            return 1;
        }
    }
    return false;
}

//R25
bool Syntax_Analyzer::ExpressionPrime(std::string token)
{
    if(token == "+")
    {
        if(Term(token))
        {
            if (ExpressionPrime(token))
            {
                printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");
                return 1;
            }
        }
    }
    else if(token == "-")
    {
        if(Term(token))
        {
            if (ExpressionPrime(token))
            {
                printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");
                return 1;
            }        
        }
    }
    return Empty(token);
}

//R28
bool Syntax_Analyzer::Term(std::string token)
{
    if(Factor(token))
    {
        if(TermPrime(token))
        {
            printStack.push("<Term> → <Factor><TermPrime>");
            return 1;
        }
    }
    return false;
}

//R29
bool Syntax_Analyzer::TermPrime(std::string token)
{
    if(token == "*")
    {
        if(Factor(token))
        {
            if(TermPrime(token))
            {
                printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");
                return 1;
            }
        }
    }
    else if(token == "/")
    {
        if(Factor(token))
        {
            if(TermPrime(token))
            {
                printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");
                return 1;
            }
        }
    }
    return Empty(token);
}

//R30
bool Syntax_Analyzer::Factor(std::string token)
{
   if(token == "-")
    {
        if(Primary(token))
        {
            printStack.push("<Factor> → - <Primary> | <Primary>");
            return 1;
        }
    }
    else 
        if(Primary(token))
        {
            printStack.push("<Factor> → - <Primary> | <Primary>");
            return 1;
        }
    return false;
}

//R31
bool Syntax_Analyzer::Primary(std::string token)
{
    if(Ids(token))//FIX THIS SHIT -- should be identifier
    {
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        return true;
    }
    else if(true) // FIX THIS: Should be Integer
    {
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        return true;
    }
    else if(true) // FIX : should be identifier 
    {
        if(token == "(")
        {
            if(Ids(token))
            {
                if( token == ")")
                {
                    printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                    return 1;
                }
            }
        }
    }
    else if (token == "(")
    {
        if(Expression(token))
        {
            if( token == ")")
            {
                printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                return 1;
            }
        }
    }
    else if(true) // identifier 
    {
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        return true;
    }
    else if (token == "true")
    {
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        return true;
    }
    else if (token == "false")
    {
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");    
        return true;
    }
    return false;
}

//R32
bool Syntax_Analyzer::Empty(std::string token)
{
    printStack.push("<Empty> → _____");
    return true;
}
#include "Syntax_Analyzer.h"
#include "Lexical_Analyzer.h"
#include <iostream>
#include <string>
#include <stdexcept>

/*==========================================================================================
//
//                                       CONSTRUCTORS & HELPERS
//
//========================================================================================*/

    Syntax_Analyzer::Syntax_Analyzer(const std::vector<std::vector<std::string>>& tableOfLexemes) : _tableOfLexemes(tableOfLexemes),
            lexemeCounter(0) {}

    void Syntax_Analyzer::nextLexeme(std::string& token)
    {
        lexemeCounter++;
        token = _tableOfLexemes[lexemeCounter][1];
        std::cout << lexemeCounter << std::endl;
        
    }

    void Syntax_Analyzer::Parse()
    {
        Rat20F(_tableOfLexemes[lexemeCounter][1]);
    }
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
        nextLexeme(token); 
        if (token == "$$")
        {
            nextLexeme(token); 
            if (OptDeclarationList(token))
            {
                nextLexeme(token); 
                if (StatementList(token))
                { 
                    nextLexeme(token);    
                    if(token == "$$")
                    {
                        
                        printStack.push("<Rat20F> --> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$");
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        return true;
                    }
                }
            }
        }
    }
    //throw std::range_error("ERROR: Invalide Syntax!\n");
    return false;
}

//R2
bool Syntax_Analyzer::OptFuncDef(std::string token)
{
    if(FuncDef(token) || Empty(token)) 
    {
        
        printStack.push("<Opt Function Definitions> → <Function Definitions> | <Empty>");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    return 0;
}   

//R3
bool Syntax_Analyzer::FuncDef(std::string token)
{
    if (Func(token))
    {
        nextLexeme(token);
        FuncDef(token);
        
        printStack.push("<Function Definitions> → <Function> | <Function> <Function Definitions>");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true; // We return true here because R3 says Func OR (Func and FuncDef)
    }
    return false;
}

//R4
bool Syntax_Analyzer::Func(std::string token)
{
    if(token == "function")
    {
        nextLexeme(token);
        if(_tableOfLexemes[lexemeCounter][0] == "identifier") 
        {
            nextLexeme(token);
            if (token == "(")
            {
                nextLexeme(token);
                if (OptParamList(token))
                {
                    nextLexeme(token);
                    if (token == ")")
                    {
                        nextLexeme(token);
                        if(OptDeclarationList(token))
                        {
                            nextLexeme(token);
                            if(Body(token))
                            {
                                
                                printStack.push("<Function> → function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
                                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                                printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    return 0;
}

//R6
bool Syntax_Analyzer::ParamList(std::string token)
{
    if(Parameter(token))
    {
        nextLexeme(token);
        if(token == ",")
        {
            nextLexeme(token);
            ParamList(token);
        }
       
        printStack.push("<Parameter List> → <Parameter> | <Parameter> , <Parameter List>");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    return false;
}

//R7
bool Syntax_Analyzer::Parameter(std::string token)
{
    if (Ids(token))
    {
        nextLexeme(token);
        if(Qualifiers(token)){
            
            printStack.push("<Parameter> → <IDs > <Qualifier>");
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    return false;
}

//R9
bool Syntax_Analyzer::Body(std::string token)
{
    if(token == "{"){
        nextLexeme(token);
        if(StatementList(token))
        {
            nextLexeme(token);   
            if(token == "}")
            {
                
                printStack.push("<Body> → { < Statement List> }");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    return false;
}

//R11
bool Syntax_Analyzer::DeclarationList(std::string token)
{
     if (Decleration(token))
    {
        nextLexeme(token);
        if(token == ";"){
            nextLexeme(token);
            DeclarationList(token);
            
            printStack.push("<Declaration List> → <Declaration> ; | <Declaration> ; <Declaration List>");
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if(Ids(token))
        {
           
            printStack.push("<Declaration> → <Qualifier > <IDs>");
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            return true;
        }
    }
    return false;
}

//R13
bool Syntax_Analyzer::Ids(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier"){
        nextLexeme(token);
        if(token == ",")
        {
            nextLexeme(token);
            Ids(token);
        }
        
        printStack.push("<IDs> → <Identifier> | <Identifier>, <IDs>");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    return false;
}

//R14
bool Syntax_Analyzer::StatementList(std::string token)
{
    if (Statement(token))
    {
        nextLexeme(token);
        StatementList(token);
       
        printStack.push("<Statement List> → <Statement> | <Statement> <Statement List>");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    return false;
}
//R16
bool Syntax_Analyzer::Compound(std::string token)
{
    if(token == "{")
    {
        nextLexeme(token);
        if(StatementList(token))
        {
            nextLexeme(token);
            if(token == "}")
            {
               
                printStack.push("<Compound> → { <Statement List> }");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                return true;
            }
        }
    }
    return false;
}

//R17
bool Syntax_Analyzer::Assign(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier"){ 
        nextLexeme(token);
        if(token=="=")
        {
            nextLexeme(token);
            if(Expression(token))
            {
                
                printStack.push("<Assign> → <Identifier> = <Expression> ;");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                return true;
            }
        }
    }
    return false;
}

//R18
bool Syntax_Analyzer::If(std::string token)
{
    if(token == "if")
    {
        nextLexeme(token);
        if(token == "(")
        {
            nextLexeme(token);
            if(Condition(token))
            {
                nextLexeme(token);
                if(token == ")")
                {
                    nextLexeme(token);
                    if(token == "fi")
                    {
                        
                        printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        return true;
                    }
                    if(Statement(token))
                    {
                        nextLexeme(token);
                        if(token == "else")
                        {
                            nextLexeme(token);
                            if(Statement(token))
                            {
                                nextLexeme(token);
                                if(token=="fi")
                                {
                                    
                                    printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
                                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
    if(token == "return")
    {
        nextLexeme(token);
        if(token==";")
        {
            
            printStack.push("<Return> → return ; | return <Expression> ;");
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            return true;
        }
        if(Expression(token))
        {
            nextLexeme(token);
            if(token == ";")
            {
                
                printStack.push("<Return> → return ; | return <Expression> ;");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                return true;
            }
            if(Expression(token))
            {
                nextLexeme(token);
                if(token==";")
                {
                    
                    printStack.push("<Return> → return ; | return <Expression> ;");
                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if(token == "(")
        {
            nextLexeme(token);
            if(Expression(token))
            {
                nextLexeme(token);
                if(token == ")")
                {
                    nextLexeme(token);
                    if(token == ";")
                    {
                        
                        printStack.push("<Print> → put ( <Expression>);");
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if(token == "(")
        {
            nextLexeme(token);
            if(Ids(token))
            {
                nextLexeme(token);
                if (token == ")")
                {
                    nextLexeme(token);
                    if(token == ";")
                    {
                        
                        printStack.push("<Scan> → get ( <IDs> );");
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if (token == "(")
        {
            nextLexeme(token);
            if(Condition(token))
            {
                nextLexeme(token);
                if(token == ")")
                {
                    nextLexeme(token);
                    if(Statement(token))
                    {
                       
                        printStack.push("<While> → while ( <Condition> ) <Statement>");
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if(Relop(token))
        {
            nextLexeme(token);
            if(Expression(token))
            {
                
                printStack.push("<Condition> → <Expression> <Relop> <Expression>");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return 1;
    }
    return false;
}

//R25
bool Syntax_Analyzer::Expression(std::string token)
{
    if(Term(token))
    {
        nextLexeme(token);
        if(ExpressionPrime(token))
        {
            
            printStack.push("<Expression> → <Term><ExpressionPrime>");
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if(Term(token))
        {
            nextLexeme(token);
            if (ExpressionPrime(token))
            {
                
                printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                return 1;
            }
        }
    }
    else if(token == "-")
    {
        nextLexeme(token);
        if(Term(token))
        {
            nextLexeme(token);
            if (ExpressionPrime(token))
            {
                
                printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if(TermPrime(token))
        {
            
            printStack.push("<Term> → <Factor><TermPrime>");
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if(Factor(token))
        {
            nextLexeme(token);
            if(TermPrime(token))
            {
                
                printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                return 1;
            }
        }
    }
    else if(token == "/")
    {
        nextLexeme(token);
        if(Factor(token))
        {
            nextLexeme(token);
            if(TermPrime(token))
            {
                
                printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        nextLexeme(token);
        if(Primary(token))
        {
           
            printStack.push("<Factor> → - <Primary> | <Primary>");
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            return 1;
        }
    }
    else 
        if(Primary(token))
        {
            
            printStack.push("<Factor> → - <Primary> | <Primary>");
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            return 1;
        }
    return false;
}

//R31
bool Syntax_Analyzer::Primary(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "integer")
    {
        
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        if(token == "(")
        {
            nextLexeme(token);
            if(Ids(token))
            {
                nextLexeme(token);
                if( token == ")")
                {
                    
                    printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
                    return 1;
                }
            }
        }
    }
    else if (token == "(")
    {
        nextLexeme(token);
        if(Expression(token))
        {
            nextLexeme(token);
            if( token == ")")
            {
                
                printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                return 1;
            }
        }
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    else if (token == "true")
    {
        
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    else if (token == "false")
    {
        
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");    
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        return true;
    }
    return false;
}

//R32
bool Syntax_Analyzer::Empty(std::string token)
{
    
    printStack.push("<Empty> → _____");
    printStack.push(_tableOfLexemes[lexemeCounter][1]);
    printStack.push(_tableOfLexemes[lexemeCounter][0]);
    return true;
}
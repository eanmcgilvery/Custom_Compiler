#include "Syntax_Analyzer.h"
#include "Lexical_Analyzer.h"
#include <string>
#include <stdexcept>

/*==========================================================================================
//
//                                       CONSTRUCTORS & HELPERS
//
//========================================================================================*/

    Syntax_Analyzer::Syntax_Analyzer(const std::vector<std::vector<std::string>>& tableOfLexemes) :
      _tableOfLexemes(tableOfLexemes) , flagEmpty(true), deepestFlag(true) {lexemeCounter = 0;}

    void Syntax_Analyzer::nextLexeme(std::string& token)
    {
        if(lexemeCounter + 1  >= _tableOfLexemes.size())
            return;
        else
        {
            lexemeCounter++;
            token = _tableOfLexemes[lexemeCounter][1];
        }
    }

    void Syntax_Analyzer::Parse()
    {
        Rat20F(_tableOfLexemes[0][1]);
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
    if((OptFuncDef(token)))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Rat20F> --> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$");
    }
    if (token == "$$")
    {
        nextLexeme(token);
        if(OptDeclarationList(token))
        {
            if (StatementList(token))
            {
                if (token == "$$")
                {
                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                    printStack.push("<Rat20F> --> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$");
                    return true;
                }
            }
        }
    }
    //throw std::range_error("ERROR: Invalid Syntax!\n");
    return false;
}

//R2
bool Syntax_Analyzer::OptFuncDef(std::string token)
{

    if(FuncDef(token) || Empty(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Opt Function Definitions> → <Function Definitions> | <Empty>");
        return true;
    }

    return false;
}   

//R3
bool Syntax_Analyzer::FuncDef(std::string token)
{
    if (Func(token))
    {
        FuncDef(token);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
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
        nextLexeme(token);
        if(_tableOfLexemes[lexemeCounter][0] == "identifier") 
        {
            nextLexeme(token);
            if (token == "(")
            {
                nextLexeme(token);
                if (OptParamList(token))
                {
                    if (token == ")")
                    {
                        nextLexeme(token);
                        if(OptDeclarationList(token))
                        {
                            if(Body(token))
                            {
                                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                                printStack.push(_tableOfLexemes[lexemeCounter][0]);
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
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Opt Parameter List> → <Parameter List> | <Empty>");

        return true;
    }
    return false;
}

//R6
bool Syntax_Analyzer::ParamList(std::string token)
{
    if (Parameter(token)) {
        if (token == ",") {
            nextLexeme(token);
            ParamList(token);
        }
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
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
        if(Qualifiers(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
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
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Qualifier> → int | boolean | real");
        nextLexeme(token);
        return true;
    }
    return false;
}

//R9
bool Syntax_Analyzer::Body(std::string token)
{
    if(token == "{")
    {
        nextLexeme(token);
        if(StatementList(token))
        {
            if (token == "}")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Body> → { < Statement List> }");
                nextLexeme(token);
                return true;
            }
        }
    }
    return false;
}

//R10
bool Syntax_Analyzer::OptDeclarationList(std::string token)
{
    if(DeclarationList(token) || Empty(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
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
        if(token == ";")
        {
            nextLexeme(token);
            DeclarationList(token);
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Declaration List> → <Declaration> ; | <Declaration> ; <Declaration List>");
            return true;
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
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Declaration> → <Qualifier > <IDs>");

            return true;
        }
    }
    return false;
}

//R13
bool Syntax_Analyzer::Ids(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        nextLexeme(token);
        if(token == ",")
        {
            Ids(token);
        }
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<IDs> → <Identifier> | <Identifier>, <IDs>");
        return true;
    }
    return false;
}

//R14V1
bool Syntax_Analyzer::StatementList(std::string token)
{
    if (Statement(token))
    {
        StatementList(token);
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement List> → <Statement> | <Statement> <Statement List>");
        return true; // We return true here because R3 says Func OR (Func and FuncDef)
    }
    return false;
}

//R15
bool Syntax_Analyzer::Statement(std::string token)
{

    if(Compound(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");

        return true;
    }

    if(Assign(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");

        return true;
    }

    if(If(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");

        return true;
    }


    if(Return(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");

        return true;
    }
    if(Print(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");

        return true;
    }

    if(Scan(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");

        return true;
    }

    if(While(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
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
        nextLexeme(token);
        if(StatementList(token))
        {
            if(token == "}")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Compound> → { <Statement List> }");
                nextLexeme(token);
                return true;
            }

        }

    }
    return false;
}

//R17
bool Syntax_Analyzer::Assign(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        nextLexeme(token);
        if(token=="=")
        {
            nextLexeme(token);
            if(Expression(token))
            {
                if(token == ";")
                {
                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                    printStack.push("<Assign> → <Identifier> = <Expression> ;");
                    nextLexeme(token);
                    return true;
                }
            }
        }
    }
    return false;
}

//R18V1
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
                if(token == ")")
                {
                    nextLexeme(token);
                    if(Statement(token))
                    {
                        if(token == "fi")
                        {
                            printStack.push(_tableOfLexemes[lexemeCounter][0]);
                            printStack.push(_tableOfLexemes[lexemeCounter][1]);
                            printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
                            nextLexeme(token);
                            return true;
                        }
                        else if (token == "else")
                        {
                            nextLexeme(token);
                            if(Statement(token))
                            {
                                if(token == "fi")
                                {
                                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
                                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                                    printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
                                    nextLexeme(token);
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
        Expression(token);
        if(token==";")
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Return> → return ; | return <Expression> ;");
            nextLexeme(token);
            return true;
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
                if(token == ")")
                {
                    nextLexeme(token);
                    if(token == ";")
                    {
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push("<Print> → put ( <Expression>);");
                        nextLexeme(token);
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
            if (Ids(token)) {
                if (token == ")") {
                    nextLexeme(token);
                    if (token == ";") {
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push("<Scan> → get ( <IDs> );");
                        nextLexeme(token);
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
                if(token == ")")
                {
                    nextLexeme(token);
                    if(Statement(token))
                    {
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
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
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
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
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Relop> →  == | !=  | >  | < | <= |  =>");
        nextLexeme(token);
        return true;
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
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Expression> → <Term><ExpressionPrime>");
            return true;
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
            if (ExpressionPrime(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");
                return true;
            }
        }
    }
    else if(token == "-")
    {
        nextLexeme(token);
        if(Term(token))
        {
            if (ExpressionPrime(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");
                return true;
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
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Term> → <Factor><TermPrime>");
            return true;
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
            if(TermPrime(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");
                return true;
            }

        }

    }
    else if(token == "/")
    {
        nextLexeme(token);
        if(Factor(token))
        {
            if(TermPrime(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");

                return true;
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
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Factor> → - <Primary> | <Primary>");

            return true;
        }

    }
    else
    {
       if(Primary(token))
       {
           printStack.push(_tableOfLexemes[lexemeCounter][0]);
           printStack.push(_tableOfLexemes[lexemeCounter][1]);
           printStack.push("<Factor> → - <Primary> | <Primary>");

           return true;
       }
    }
    return false;
}

//R31
bool Syntax_Analyzer::Primary(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        nextLexeme(token);
        return true;
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "int")
    {

        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        nextLexeme(token);
        return true;
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        nextLexeme(token);
        if(token == "(")
        {
            nextLexeme(token);
            if(Ids(token))
            {
                if( token == ")")
                {
                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                    printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                    nextLexeme(token);
                    return true;
                }
            }
        }
    }
    else if (token == "(")
    {
        nextLexeme(token);
        if(Expression(token))
        {
            if( token == ")")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                nextLexeme(token);
                return true;
            }

        }

    }
    else if(_tableOfLexemes[lexemeCounter][0] == "real")
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        nextLexeme(token);
        return true;
    }
    else if (token == "true")
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        nextLexeme(token);
        return true;
    }
    else if (token == "false")
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        nextLexeme(token);
        return true;
    }
    return false;
}

//R32
bool Syntax_Analyzer::Empty(std::string token)
{
    flagEmpty = false;
    printStack.push(_tableOfLexemes[lexemeCounter][0]);
    printStack.push(_tableOfLexemes[lexemeCounter][1]);
    printStack.push("<Empty> → _____");
    return true;
}
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
    (OptFuncDef(token));
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if (token == "$$")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if (OptDeclarationList(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                std::size_t tempLexCount = lexemeCounter;
                if (StatementListV1(token))
                {
                    token = tokenFromBelow;
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if(token == "$$")
                    {
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push("<Rat20F> --> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$");
                        return true;
                    }
                }
                lexemeCounter = tempLexCount;
                if (StatementListV2(token))
                {
                    token = tokenFromBelow;
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if(token == "$$")
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
    std::size_t tempLexCount = lexemeCounter;
    if(FuncDefV1(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Opt Function Definitions> → <Function Definitions> | <Empty>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(FuncDefV2(token) || Empty(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Opt Function Definitions> → <Function Definitions> | <Empty>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }

    return false;
}   

//R3
bool Syntax_Analyzer::FuncDefV1(std::string token)
{
    if (Func(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Function Definitions> → <Function> | <Function> <Function Definitions>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true; // We return true here because R3 says Func OR (Func and FuncDef)
    }
    return false;
}

bool Syntax_Analyzer::FuncDefV2(std::string token)
{
    if (Func(token))
    {
        std::size_t tempLexCount = lexemeCounter;
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(FuncDefV1(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Function Definitions> → <Function> | <Function> <Function Definitions>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true; // We return true here because R3 says Func OR (Func and FuncDef)
        }
        lexemeCounter = tempLexCount;
        if(FuncDefV2(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Function Definitions> → <Function> | <Function> <Function Definitions>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true; // We return true here because R3 says Func OR (Func and FuncDef)
        }
        lexemeCounter--;
    }
    return false;
}

//R4
bool Syntax_Analyzer::Func(std::string token)
{
    if(token == "function")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(_tableOfLexemes[lexemeCounter][0] == "identifier") 
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if (token == "(")
            {
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if (OptParamList(token))
                {
                    token = tokenFromBelow;
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if (token == ")")
                    {
                        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                        if(OptDeclarationList(token))
                        {
                            token = tokenFromBelow;
                            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                            if(Body(token))
                            {
                                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                                printStack.push("<Function> → function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
                                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                                return true;
                            }
                            lexemeCounter--;
                        }
                        lexemeCounter--;
                    }
                    lexemeCounter--;
                }
                lexemeCounter--;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R5
bool Syntax_Analyzer::OptParamList(std::string token)
{
    std::size_t tempLexCount = lexemeCounter;
    if(ParamListV1(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Opt Parameter List> → <Parameter List> | <Empty>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if (ParamListV2(token) || Empty(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Opt Parameter List> → <Parameter List> | <Empty>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    return false;
}

//R6
bool Syntax_Analyzer::ParamListV1(std::string token)
{
    if(Parameter(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Parameter List> → <Parameter> | <Parameter> , <Parameter List>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    return false;
}

bool Syntax_Analyzer::ParamListV2(std::string token)
{
    if(Parameter(token))
    {
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token == ",")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            std::size_t tempLexCount = lexemeCounter;
            if(ParamListV1(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Parameter List> → <Parameter> | <Parameter> , <Parameter List>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter = tempLexCount;
            if(ParamListV2(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Parameter List> → <Parameter> | <Parameter> , <Parameter List>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R7
bool Syntax_Analyzer::Parameter(std::string token)
{
    std::size_t tempLexCount = lexemeCounter;
    if (IdsV1(token))
    {
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Qualifiers(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Parameter> → <IDs > <Qualifier>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true;
        }
        lexemeCounter--;
    }
    lexemeCounter = tempLexCount;
    if (IdsV2(token))
    {
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Qualifiers(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Parameter> → <IDs > <Qualifier>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true;
        }
        lexemeCounter--;
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
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    return false;
}

//R9
bool Syntax_Analyzer::Body(std::string token)
{
    if(token == "{")
    {
        std::size_t tempLexCount = lexemeCounter;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(StatementListV1(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(token == "}")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Body> → { < Statement List> }");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter = tempLexCount;
        if(StatementListV2(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(token == "}")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Body> → { < Statement List> }");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R10
bool Syntax_Analyzer::OptDeclarationList(std::string token)
{
    std::size_t tempLexCount = lexemeCounter;
    if(DeclarationListV1(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Opt Declaration List> → <Declaration List> | <Empty>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(DeclarationListV2(token) || Empty(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Opt Declaration List> → <Declaration List> | <Empty>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    return false;
}

//R11V1
bool Syntax_Analyzer::DeclarationListV1(std::string token)
{
    if (Decleration(token))
    {
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token == ";")
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Declaration List> → <Declaration> ; | <Declaration> ; <Declaration List>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true; // We return true here because R11 says Func OR (Declaration and DeclarationList)
        }
        lexemeCounter--;
    }
    return false;
}

//R11V2
bool Syntax_Analyzer::DeclarationListV2(std::string token)
{
     if (Decleration(token))
    {
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token == ";")
        {
            std::size_t tempLexCount = lexemeCounter;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(DeclarationListV1(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Declaration List> → <Declaration> ; | <Declaration> ; <Declaration List>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true; // We return true here because R11 says Func OR (Declaration and DeclarationList)
            }
            lexemeCounter = tempLexCount;
            if(DeclarationListV2(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Declaration List> → <Declaration> ; | <Declaration> ; <Declaration List>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true; // We return true here because R11 says Func OR (Declaration and DeclarationList)
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R12
bool Syntax_Analyzer::Decleration(std::string token)
{
     if (Qualifiers(token))
    {
        token = tokenFromBelow;
        std::size_t tempLexCount = lexemeCounter;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(IdsV1(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Declaration> → <Qualifier > <IDs>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true;
        }
        lexemeCounter = tempLexCount;
        if(IdsV2(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Declaration> → <Qualifier > <IDs>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true;
        }
        lexemeCounter--;
    }
    return false;
}

//R13
bool Syntax_Analyzer::IdsV1(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<IDs> → <Identifier> | <Identifier>, <IDs>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    return false;
}

bool Syntax_Analyzer::IdsV2(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token == ",")
        {
            std::size_t tempLexCount = lexemeCounter;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(IdsV1(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<IDs> → <Identifier> | <Identifier>, <IDs>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter = tempLexCount;
            if(IdsV2(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<IDs> → <Identifier> | <Identifier>, <IDs>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R14V1
bool Syntax_Analyzer::StatementListV1(std::string token)
{
    if (Statement(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement List> → <Statement> | <Statement> <Statement List>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true; // We return true here because R3 says Func OR (Func and FuncDef)
    }
    return false;
}

//R14V2
bool Syntax_Analyzer::StatementListV2(std::string token)
{
    if (Statement(token))
    {
        std::size_t tempLexCount = lexemeCounter;
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(StatementListV1(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Statement List> → <Statement> | <Statement> <Statement List>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true; // We return true here because R3 says Func OR (Func and FuncDef)
        }
        lexemeCounter = tempLexCount;
        if(StatementListV2(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Statement List> → <Statement> | <Statement> <Statement List>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true; // We return true here because R3 says Func OR (Func and FuncDef)
        }
        lexemeCounter--;
    }
    return false;
}

//R15
bool Syntax_Analyzer::Statement(std::string token)
{
    std::size_t tempLexCount = lexemeCounter;
    if(Compound(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(Assign(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(IfV1(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(IfV2(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(ReturnV1(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(ReturnV2(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(Print(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(Scan(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    lexemeCounter = tempLexCount;
    if(While(token))
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    return false;
}
//R16
bool Syntax_Analyzer::Compound(std::string token)
{
    if(token == "{")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        std::size_t tempLexCount = lexemeCounter;
        if(StatementListV1(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(token == "}")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Compound> → { <Statement List> }");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter = tempLexCount;
        if(StatementListV2(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(token == "}")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Compound> → { <Statement List> }");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R17
bool Syntax_Analyzer::Assign(std::string token)
{
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token=="=")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(Expression(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Assign> → <Identifier> = <Expression> ;");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R18V1
bool Syntax_Analyzer::IfV1(std::string token)
{
    if(token == "if")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token == "(")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(Condition(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if(token == ")")
                {
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if(Statement(token))
                    {
                        token = tokenFromBelow;
                        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                        if(token == "fi")
                        {
                            printStack.push(_tableOfLexemes[lexemeCounter][0]);
                            printStack.push(_tableOfLexemes[lexemeCounter][1]);
                            printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
                            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                            return true;
                        }
                        lexemeCounter--;
                    }
                    lexemeCounter--;
                }
                lexemeCounter--;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R18V2
bool Syntax_Analyzer::IfV2(std::string token)
{
    if(token == "if")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token == "(")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(Condition(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if(token == ")")
                {
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if(Statement(token))
                    {
                        token = tokenFromBelow;
                        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                        if(token == "else")
                        {
                            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                            if(Statement(token))
                            {
                                token = tokenFromBelow;
                                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                                if(token == "fi"){
                                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
                                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                                    printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
                                    if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                                    return true;
                                }
                                lexemeCounter--;
                            }
                            lexemeCounter--;
                        }
                        lexemeCounter--;
                    }
                    lexemeCounter--;
                }
                lexemeCounter--;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R19
bool Syntax_Analyzer::ReturnV1(std::string token)
{
    if(token == "return")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token==";")
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Return> → return ; | return <Expression> ;");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true;
        }
        lexemeCounter--;
    }
    return false;
}

bool Syntax_Analyzer::ReturnV2(std::string token)
{
    if(token == "return")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Expression(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(token==";")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Return> → return ; | return <Expression> ;");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R20
bool Syntax_Analyzer::Print(std::string token)
{
    if (token == "put")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token == "(")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(Expression(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if(token == ")")
                {
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if(token == ";")
                    {
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push("<Print> → put ( <Expression>);");
                        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                        return true;
                    }
                    lexemeCounter--;
                }
                lexemeCounter--;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R21
bool Syntax_Analyzer::Scan(std::string token)
{
    if (token == "get")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(token == "(")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            std::size_t tempLexCount = lexemeCounter;
            if(IdsV1(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if (token == ")")
                {
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if(token == ";")
                    {
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push("<Scan> → get ( <IDs> );");
                        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                        return true;
                    }
                    lexemeCounter--;
                }
                lexemeCounter--;
            }
            lexemeCounter = tempLexCount;
            if(IdsV2(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if (token == ")")
                {
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if(token == ";")
                    {
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push("<Scan> → get ( <IDs> );");
                        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                        return true;
                    }
                    lexemeCounter--;
                }
                lexemeCounter--;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R22
bool Syntax_Analyzer::While(std::string token)
{
    if (token == "while")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if (token == "(")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(Condition(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if(token == ")")
                {
                    if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                    if(Statement(token))
                    {
                        printStack.push(_tableOfLexemes[lexemeCounter][0]);
                        printStack.push(_tableOfLexemes[lexemeCounter][1]);
                        printStack.push("<While> → while ( <Condition> ) <Statement>");
                        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                        return true;
                    }
                    lexemeCounter--;
                }
                lexemeCounter--;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return false;
}

//R23
bool Syntax_Analyzer::Condition(std::string token)
{
    if(Expression(token))
    {
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Relop(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(Expression(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Condition> → <Expression> <Relop> <Expression>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return 1;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
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
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    return false;
}

//R25
bool Syntax_Analyzer::Expression(std::string token)
{
    if(Term(token))
    {
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(ExpressionPrime(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Expression> → <Term><ExpressionPrime>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true;
        }
        lexemeCounter--;
    }
    return false;
}

//R25
bool Syntax_Analyzer::ExpressionPrime(std::string token)
{
    if(token == "+")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Term(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if (ExpressionPrime(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    else if(token == "-")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Term(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if (ExpressionPrime(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return Empty(token);
}

//R28
bool Syntax_Analyzer::Term(std::string token)
{
    if(Factor(token))
    {
        token = tokenFromBelow;
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(TermPrime(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Term> → <Factor><TermPrime>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true;
        }
        lexemeCounter--;
    }
    return false;
}

//R29
bool Syntax_Analyzer::TermPrime(std::string token)
{
    if(token == "*")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Factor(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(TermPrime(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    else if(token == "/")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Factor(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if(TermPrime(token))
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    return Empty(token);
}

//R30
bool Syntax_Analyzer::Factor(std::string token)
{
   if(token == "-")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Primary(token))
        {
            printStack.push(_tableOfLexemes[lexemeCounter][0]);
            printStack.push(_tableOfLexemes[lexemeCounter][1]);
            printStack.push("<Factor> → - <Primary> | <Primary>");
            if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
            return true;
        }
        lexemeCounter--;
    }
    else
    {
       if(Primary(token))
       {
           printStack.push(_tableOfLexemes[lexemeCounter][0]);
           printStack.push(_tableOfLexemes[lexemeCounter][1]);
           printStack.push("<Factor> → - <Primary> | <Primary>");
           if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
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
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "int")
    {

        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        if(token == "(")
        {
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            std::size_t tempLexCount = lexemeCounter;
            if(IdsV1(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if( token == ")")
                {
                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                    printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                    if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                    return true;
                }
                lexemeCounter--;
            }
            lexemeCounter = tempLexCount;
            if(IdsV2(token))
            {
                token = tokenFromBelow;
                if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
                if( token == ")")
                {
                    printStack.push(_tableOfLexemes[lexemeCounter][0]);
                    printStack.push(_tableOfLexemes[lexemeCounter][1]);
                    printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                    if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                    return true;
                }
                lexemeCounter--;
            }
            lexemeCounter--;
        }
    }
    else if (token == "(")
    {
        if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
        if(Expression(token))
        {
            token = tokenFromBelow;
            if(flagEmpty){nextLexeme(token); deepestFlag = true;} else{flagEmpty = true;}
            if( token == ")")
            {
                printStack.push(_tableOfLexemes[lexemeCounter][0]);
                printStack.push(_tableOfLexemes[lexemeCounter][1]);
                printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
                if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
                return true;
            }
            lexemeCounter--;
        }
        lexemeCounter--;
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    else if (token == "true")
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
        return true;
    }
    else if (token == "false")
    {
        printStack.push(_tableOfLexemes[lexemeCounter][0]);
        printStack.push(_tableOfLexemes[lexemeCounter][1]);
        printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");
        if(deepestFlag) {tokenFromBelow = token;  deepestFlag = false;}
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
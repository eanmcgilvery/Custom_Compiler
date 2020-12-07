#include "Syntax_Analyzer.h"
#include "Lexical_Analyzer.h"
#include <string>
#include <stdexcept>

/*==========================================================================================
//
//                                       CONSTRUCTORS & HELPERS
//
//========================================================================================*/

Syntax_Analyzer::Syntax_Analyzer(const std::vector<std::vector<std::string>>& tableOfLexemes, const std::string& fileName) :
        _tableOfLexemes(tableOfLexemes), fileName_(fileName), error_(fileName_) {lexemeCounter = -1;}

void Syntax_Analyzer::nextLexeme(std::string& token)
{
    if(lexemeCounter + 1  >= _tableOfLexemes.size())
        return;
    else
    {
        lexemeCounter++;
        token = _tableOfLexemes[lexemeCounter][1];
    }
    printStack.push(" ");
    printStack.push(_tableOfLexemes[lexemeCounter][0]);
    printStack.push(_tableOfLexemes[lexemeCounter][1]);
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
bool Syntax_Analyzer::Rat20F(std::string& token)
{
    nextLexeme(token);
    printStack.push("<Rat20F> --> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List> $$");
    OptFuncDef(token);
    if (token == "$$") {
       nextLexeme(token);
       OptDeclarationList(token);
       if (!StatementList(token)) {
           return false;
       }
    }
    else
    {
       error_.Throw("Missing $$", _tableOfLexemes[lexemeCounter][0] ,token);
       return false;
    }
    //else throw an error
    if (token == "$$")
    {
        printInstr();
        return true;
    }
    else
    {
        error_.Throw("Missing $$", _tableOfLexemes[lexemeCounter][0] ,token);
        return false;
    }

}

//R2
bool Syntax_Analyzer::OptFuncDef(std::string& token)
{
    printStack.push("<Opt Function Definitions> → <Function Definitions> | <Empty>");
    if(FuncDef(token))
    {
        return true;
    }
    return Empty(token);
}

//R3
bool Syntax_Analyzer::FuncDef(std::string& token)
{
    printStack.push("<Function Definitions> → <Function> | <Function> <Function Definitions>");
    if (Func(token))
    {
        if(FuncDef(token))
        {
            return true; // We return true here because R3 says Func OR (Func and FuncDef)
        }
        else{
            return true; // We return true here because R3 says Func OR (Func and FuncDef)
        }
    }
    else{
        return false;
    }
}

//R4
bool Syntax_Analyzer::Func(std::string& token)
{
    printStack.push("<Function> → function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
    if(token == "function")
    {
        nextLexeme(token);
        if(_tableOfLexemes[lexemeCounter][0] == "identifier")
        {
            nextLexeme(token);
            if (token == "(")
            {
                nextLexeme(token);
                OptParamList(token);
                if (token == ")")
                {
                    nextLexeme(token);
                    OptDeclarationList(token);
                    if(Body(token))
                    {
                        return true;
                    }
                    else{
                        return false;
                    }
                }
                error_.Throw("the following token: "+ token, _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
            error_.Throw("the following token: "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
        error_.Throw("the following Lexeme: identifier", _tableOfLexemes[lexemeCounter][0] ,token);
        return false;
    }
    return false;


}

//R5
bool Syntax_Analyzer::OptParamList(std::string& token)
{
    printStack.push("<Opt Parameter List> → <Parameter List> | <Empty>");
    if(ParamList(token))
    {
        return true;
    }
    return Empty(token);
}

//R6
bool Syntax_Analyzer::ParamList(std::string& token)
{
    printStack.push("<Parameter List> → <Parameter> | <Parameter> , <Parameter List>");
    if (Parameter(token)) {
        if (token == ",") {
            nextLexeme(token);
            if(ParamList(token)){
                return true;
            }
            else{
                error_.Throw("the following Lexeme: "+token, _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    return false;
}
//R7
bool Syntax_Analyzer::Parameter(std::string& token)
{
    printStack.push("<Parameter> → <IDs > <Qualifier>");
    if (Ids(token, false, false))
    {
        if(Qualifiers(token))
        {
            return true;
        }
    }
    return false;
}

//R8
bool Syntax_Analyzer::Qualifiers(std::string& token)
{
    printStack.push("<Qualifier> → int | boolean | real");
    if(token =="int" || token == "boolean" || token == "real")
    {
        nextLexeme(token);
        return true;
    }
    return false;
}

//R9
bool Syntax_Analyzer::Body(std::string& token)
{
    printStack.push("<Body> → { < Statement List> }");
    if(token == "{")
    {
        nextLexeme(token);
        if(StatementList(token)){
            if (token == "}")
            {
                nextLexeme(token);
                return true;
            }
            else{
                error_.Throw("the following token: }", _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else{
            return false;
        }
    }
    else
    {
        error_.Throw("the following token: {", _tableOfLexemes[lexemeCounter][0] ,token);
        return false;
    }

}

//R10
bool Syntax_Analyzer::OptDeclarationList(std::string& token)
{
    printStack.push("<Opt Declaration List> → <Declaration List> | <Empty>");
    if(DeclarationList(token))
    {
        return true;
    }
    return Empty(token);
}

//R11
bool Syntax_Analyzer::DeclarationList(std::string& token)
{
    printStack.push("<Declaration List> → <Declaration> ; | <Declaration> ; <Declaration List>");
    if (Decleration(token))
    {
        if(token == ";")
        {
            nextLexeme(token);
            if(DeclarationList(token)){
                return true;
            }
            else{
                return true;
            }
        }
        else
        {
            error_.Throw("the following token: }", _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R12
bool Syntax_Analyzer::Decleration(std::string& token)
{
    printStack.push("<Declaration> → <Qualifier > <IDs>");
    if (Qualifiers(token))
    {
        if(Ids(token,true, false))
        {
            return true;
        }
    }
    return false;
}

//R13
bool Syntax_Analyzer::Ids(std::string& token, bool from_dec, bool from_scan)
{
    printStack.push("<IDs> → <Identifier> | <Identifier>, <IDs>");
    printStack.push("<IDs> → <Identifier> | <Identifier>, <IDs>");
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        if(from_scan)
        {
            if (is_duplicate(token)) {
                gen_instr("STDIN", -1111);
                gen_instr("POPM", get_addr(token));
            }
        }
        if(from_dec)
        {
                symbolTable[symbCount] = token;
                symbolType[symbCount] =  findNearestKeyword(token);
                symbCount++;
        }


        nextLexeme(token);
        if(token == ",")
        {
            nextLexeme(token);
            if(Ids(token,from_dec, from_scan))
            {
                return true;
            }
            else{
                error_.Throw("the following token: "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            }
        }
        else
        {
            return true;
        }
    }
    return false;
}

//R14V1
bool Syntax_Analyzer::StatementList(std::string& token)
{
    printStack.push("<Statement List> → <Statement> | <Statement> <Statement List>");
    if (Statement(token))
    {
        if(StatementList(token))
        {
            return true; // We return true here because R3 says Func OR (Func and FuncDef)
        }
        else
        {
           return true; // We return true here because R3 says Func OR (Func and FuncDef)
        }
    }
    return false;
}

//R15
bool Syntax_Analyzer::Statement(std::string& token)
{
    printStack.push("<Statement> → <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
    if(Compound(token)||Assign(token)||If(token) ||Return(token) ||Print(token)||Scan(token)||While(token))
    {
        return true;
    }
    return false;
}
//R16
bool Syntax_Analyzer::Compound(std::string& token)
{
    printStack.push("<Body> → { < Statement List> }");
    if(token == "{")
    {
        nextLexeme(token);
        if(StatementList(token)){
            if (token == "}")
            {
                nextLexeme(token);
                return true;
            }
            else{
                error_.Throw("the following token: }", _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else{
            error_.Throw("the following token: {", _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    else{
        return false;
    }

}

//R17
bool Syntax_Analyzer::Assign(std::string& token)
{
    printStack.push("<Assign> → <Identifier> = <Expression> ;");
    if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        std::string saveMe = _tableOfLexemes[lexemeCounter][1];

        // Check to see if the data type was a boolean
        bool b_assign = false;
        if(_tableOfLexemes[lexemeCounter][1] == "boolean")
            b_assign = true;

        nextLexeme(token);
        if(token=="=")
        {
            nextLexeme(token);
            if(Expression(token, b_assign))
            {
                if(token == ";")
                {
                    gen_instr("POPM", get_addr(saveMe));
                    nextLexeme(token);
                    return true;
                }
                else{
                    error_.Throw("the following token: ;", _tableOfLexemes[lexemeCounter][0] ,token);
                    return false;
                }
            }
            error_.Throw("Expected expression after "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
        else{
            error_.Throw("the following token: =", _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R18
bool Syntax_Analyzer::If(std::string& token)
{
    printStack.push("<If> → if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> else <Statement> fi");
    if(token == "if")
    {
        addr = instrAddress;
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
                        back_patch(instrAddress);
                        if(token == "fi")
                        {
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
                                    nextLexeme(token);
                                    return true;
                                }
                                else
                                {
                                    error_.Throw("the following token: fi", _tableOfLexemes[lexemeCounter][0] ,token);
                                    return false;
                                }
                            }
                            else
                            {
                                error_.Throw("the following token: "+token, _tableOfLexemes[lexemeCounter][0] ,token);
                                return false;
                            }

                        }
                        else
                        {
                            error_.Throw("the following token: " + token, _tableOfLexemes[lexemeCounter][0] ,token);
                            return false;
                        }
                    }
                    else
                    {
                        error_.Throw("the following token: "+token, _tableOfLexemes[lexemeCounter][0] ,token);
                        return false;
                    }
                }
                else
                {
                    error_.Throw("the following token: )", _tableOfLexemes[lexemeCounter][0] ,token);
                    return false;
                }
            }
            else
            {
                error_.Throw("Expected condition not found ", _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else
        {
            error_.Throw("the following token: (", _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R19
bool Syntax_Analyzer::Return(std::string& token)
{
    printStack.push("<Return> → return ; | return <Expression> ;");
    if(token == "return")
    {
        nextLexeme(token);
        if(Expression(token,false)){
            if(token==";")
            {
                nextLexeme(token);
                return true;
            }
            else
            {
                error_.Throw("the following token: ;", _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        if(token==";")
        {
            nextLexeme(token);
            return true;
        }
        else
        {
            error_.Throw("the following token: ;", _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R20
bool Syntax_Analyzer::Print(std::string& token)
{
    printStack.push("<Print> → put ( <Expression>);");

    if (token == "put")
    {
        nextLexeme(token);
        if(token == "(")
        {
            nextLexeme(token);
            if(Expression(token, false))
            {

                if(token == ")")
                {
                    nextLexeme(token);
                    if(token == ";")
                    {
                        gen_instr("STDOUT", -1111);
                        nextLexeme(token);
                        return true;
                    }
                    else
                    {
                        error_.Throw("the following token: ;", _tableOfLexemes[lexemeCounter][0] ,token);
                        return false;
                    }
                }
                else
                {
                    error_.Throw("the following token: )", _tableOfLexemes[lexemeCounter][0] ,token);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            error_.Throw("the following token: (", _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R21
bool Syntax_Analyzer::Scan(std::string& token)
{
    printStack.push("<Scan> → get ( <IDs> );");

    if (token == "get")
    {
        nextLexeme(token);
        if(token == "(")
        {
            nextLexeme(token);
            if (Ids(token, false, true)) {
                if (token == ")") {
                    nextLexeme(token);
                    if (token == ";") {
                       nextLexeme(token);
                        return true;
                    }
                    else
                    {
                        error_.Throw("the following token: ;", _tableOfLexemes[lexemeCounter][0] ,token);
                        return false;
                    }
                }
                else
                {
                    error_.Throw("the following token: )", _tableOfLexemes[lexemeCounter][0] ,token);
                    return false;
                }
            }
            else
            {
                error_.Throw("an Identifier, but received a  "+token, _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else
        {
            error_.Throw("the following token: (", _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R22
bool Syntax_Analyzer::While(std::string& token)
{
    printStack.push("<While> → while ( <Condition> ) <Statement>");

    if (token == "while")
    {
        addr = instrAddress;
        gen_instr("LABEL", -1111);
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
                        gen_instr("JUMP", addr);
                        back_patch(instrAddress);
                        return true;
                    }
                    else
                    {
                        error_.Throw(" a statement but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
                        return false;
                    }
                }
                else
                {
                    error_.Throw("the following token: )", _tableOfLexemes[lexemeCounter][0] ,token);
                    return false;
                }
            }
            else
            {
                error_.Throw("a condition, but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else
        {
            error_.Throw("the following token: (", _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R23
bool Syntax_Analyzer::Condition(std::string& token)
{
    printStack.push("<Condition> → <Expression> <Relop> <Expression>");

    if(Expression(token, false))
    {
        std::string op = token;
        if(Relop(token))
        {
            if(Expression(token, false))
            {
                if(op == "<")
                {
                    gen_instr("LES",-1111);
                }
                if(op == ">")
                {
                    gen_instr("GRT",-1111);
                }
                if(op == "==")
                {
                    gen_instr("EQU",-1111);
                }
                if(op == "!=")
                {
                    gen_instr("NEQ",-1111);
                }
                if(op == "=>")
                {
                    gen_instr("GEQ",-1111);
                }
                if(op == "<=")
                {
                    gen_instr("LEQ",-1111);
                }
                jumpstack.push(instrAddress);
                gen_instr("JUMPZ", -1111);
                return true;
            }
            else
            {
                error_.Throw(" an Expression, but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else
        {
            error_.Throw("a Relop, but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R24
bool Syntax_Analyzer::Relop(std::string& token)
{
    printStack.push("<Relop> →  == | !=  | >  | < | <= |  =>");

    if(token == "==" || token == "!=" ||token == ">" ||token == "<" ||token == "<=" ||token == "=>")
    {
        nextLexeme(token);
        return true;
    }
    return false;
}

//R25
bool Syntax_Analyzer::Expression(std::string& token, bool b_assign=false)
{
    printStack.push("<Expression> → <Term><ExpressionPrime>");

    if(Term(token, b_assign))
    {
        if(ExpressionPrime(token))
        {
            return true;
        }
        else{
            error_.Throw("an expression, but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return false;
}

//R25
bool Syntax_Analyzer::ExpressionPrime(std::string& token)
{
    printStack.push("<ExpressionPrime> → + <Term><ExpressionPrime> |  - <Term><ExpressionPrime> |<epsilon>");

    if(token == "+" || token == "-")
    {
        std::string op = token;
        nextLexeme(token);
        if(Term(token,false))
        {
            if (op == "+")
                gen_instr("ADD", -1111);
            if (op == "-")
                gen_instr("SUB", -1111);

            if(ExpressionPrime(token))
            {
                return true;
            }
        }
        else
        {
            error_.Throw("a term, but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return Empty(token);
}

//R28
bool Syntax_Analyzer::Term(std::string& token, bool b_assign = false)
{
    printStack.push("<Term> → <Factor><TermPrime>");

    if(Factor(token, b_assign))
    {
        if(TermPrime(token))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

//R29
bool Syntax_Analyzer::TermPrime(std::string& token)
{
    printStack.push("<TermPrime> → <Empty> | *<Factor><TermPrime> | / <Factor><TermPrime>");
    if(token == "*" || token == "/")
    {
        std::string op = token;
        nextLexeme(token);
        if(Factor(token, false))
        {
            if (op == "*")
                gen_instr("MUL", -1111);

            if (op == "/")
                gen_instr("DIV", -1111);

            if(TermPrime(token))
               return true;
        }
        else
        {
            error_.Throw("a Factor but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    return Empty(token);
}

//R30
bool Syntax_Analyzer::Factor(std::string& token, bool b_assign = false)
{
    printStack.push("<Factor> → - <Primary> | <Primary>");

    if(token == "-")
    {
        nextLexeme(token);
        if(Primary(token, b_assign))
        {
            return true;
        }
        else
        {
            error_.Throw("a Primary but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    else
    {
        if(Primary(token, b_assign))
        {
            return true;
        }
        else{
            error_.Throw("a Primary but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
}

//R31
bool Syntax_Analyzer::Primary(std::string& token, bool b_assign = false)
{
    printStack.push("<Primary> → <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");

    if(_tableOfLexemes[lexemeCounter][0] == "int" || _tableOfLexemes[lexemeCounter][0] == "real"|| token == "true" || token == "false")
    {
        if(_tableOfLexemes[lexemeCounter][0] == "int")
        {
            gen_instr("PUSHI", stoi(_tableOfLexemes[lexemeCounter][1]));
        }

        nextLexeme(token);
        return true;
    }
    else if(_tableOfLexemes[lexemeCounter][0] == "identifier")
    {
        gen_instr("PUSHM", get_addr(_tableOfLexemes[lexemeCounter][1]));
        nextLexeme(token);
        if(token == "(")
        {
            nextLexeme(token);
            if(Ids(token,false, false))
            {
                if( token == ")")
                {
                    nextLexeme(token);
                    return true;
                }
                else
                {
                    error_.Throw("the following token: )", _tableOfLexemes[lexemeCounter][0] ,token);
                    return false;
                }
            }
            else
            {
                error_.Throw("a ID but received a  "+token, _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    else if (token == "(")
    {
        nextLexeme(token);
        if(Expression(token))
        {
            if( token == ")")
            {
                nextLexeme(token);
                return true;
            }
            else
            {
                error_.Throw(" ) ", _tableOfLexemes[lexemeCounter][0] ,token);
                return false;
            }
        }
        else
        {
            error_.Throw("a Expression but received a "+token, _tableOfLexemes[lexemeCounter][0] ,token);
            return false;
        }
    }
    else{
        return false;
    }
}

//R32
bool Syntax_Analyzer::Empty(std::string& token)
{
    printStack.push("<Empty> → _____");
    return true;
}

void Syntax_Analyzer::gen_instr(std::string op, int oper)
{
    instrTable[instrAddress].address = instrAddress;
    instrTable[instrAddress].operand = oper;
    instrTable[instrAddress].op = op;

    instrAddress++;
}

void Syntax_Analyzer::back_patch(int jump_addr)
{
    if (!jumpstack.empty())
    {
        addr = jumpstack.top();
        jumpstack.pop();
        instrTable[addr].operand = jump_addr;
    }
    else
    {
        std::cout << "ERROR: JUMP STACK IS EMPTY";
    }
}

int Syntax_Analyzer::get_addr(std::string symb)
{
    int addr = 0;

    for(int i = 0; i < symbCount + 1; i++)
    {
        if(symbolTable[i] == symb)
        {
            addr = i + memAddr;
        }
    }
    return addr;
}

bool Syntax_Analyzer::is_duplicate(std::string input)
{
    for (int i =0; i < _tableOfLexemes.size(); i++)
    {
        if (_tableOfLexemes[i][1] == input)
            return true;
    }
    return false;
}

std::string Syntax_Analyzer::findNearestKeyword(std::string token)
{
    int nearestKeyword=0;
        for(int j = 0; j!= lexemeCounter ;j++)
        {
            if(_tableOfLexemes[j][0] == "keyword")
            {
                nearestKeyword = j;
            }
        }

    return _tableOfLexemes[nearestKeyword][1];
}

void Syntax_Analyzer::printInstr() {

    std::ofstream fout("fileName.txt");

    for(int i = 1; i < instrAddress;i++){
        fout << i << ". " << instrTable[i].op << " ";
        if(instrTable[i].operand != -1111) {
            fout << instrTable[i].operand << std::endl;
        }
        else {
            fout << std::endl;
        }
    }

    fout << "\nIDENTIFIER" << "               " << "MEMORY LOCATION" << "               " << "TYPE\n";

    for (int i =0; i < symbCount; i++)
    {
        fout << symbolTable[i] << "                      " << get_addr(symbolTable[i]) <<  "                      " << symbolType[i] << std::endl;
    }
    fout.close();
}
#include "Lexical_Analyzer.h"

Lexical_Analyzer::Lexical_Analyzer(){} 

std::unordered_map<std::string,std::string> Lexical_Analyzer::lexer(std::string& line)
{
    // Take line and chop into tokens
    while(line.substr(mainMachine_.WhereAreWe()).size() == 0)
    {
        // NOTE FOR EAN: GO BACK AND CHECK THIS... suspiciion on the substr
        // Check the arguments for compute Token...
        mainMachine_.computeToken(line.substr(mainMachine_.WhereAreWe()));
    }

}

std::string Lexical_Analyzer::isDouble(std::string& lexeme)
{
 if(lexeme.find('.')!= std::string::npos)
    return "real";
  return "integer"; 
}
std::string Lexical_Analyzer::isKeyword(std::string& lexeme)
{
  if(_keywords.find(lexeme)!=_keywords.end())
    return "keyword";
  return "identifier";
}

std::string Lexical_Analyzer::isOperator(std::string& lexeme)
{
  if(_operators.find(lexeme)!=_operators.end())
    return "operator";
  return "separator";
}

std::string Lexical_Analyzer::whatTokenType(std::string& lexeme)
{
    if(isalpha(lexeme[0]))
        return isKeyword(lexeme);
    else if(isdigit(lexeme[0]))
        return isDouble(lexeme);
    else
    {
        return isOperator(lexeme);
    }
}
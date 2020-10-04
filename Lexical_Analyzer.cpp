#include "Lexical_Analyzer.h"

Lexical_Analyzer::Lexical_Analyzer(){} 

std::vector<std::vector<std::string>> Lexical_Analyzer::lexer(std::string line)
{
  std::vector<std::vector<std::string>>  outputTable;
  
    // Take line and chop into tokens
    while(!line.substr(mainMachine_.whereAreWe_).empty())
    {
      //  std::cout << line << "\n";
//        std::cout << line.substr(mainMachine_.whereAreWe_).size() << std::endl;
//        std::cout << mainMachine_.whereAreWe_ << std::endl;
        //tokenLexeme = [token,lexeme]
      std::vector<std::string> tokenLexeme;
      std::string tempLexeme;
      tempLexeme = mainMachine_.computeToken(line.substr(mainMachine_.whereAreWe_)); // Grab the Lexeme
        if(tempLexeme!=" ")
        {
          if(mainMachine_.flag)
              continue;
          tokenLexeme.push_back(whatTokenType(tempLexeme));  //push back Token to table
          tokenLexeme.push_back(tempLexeme); //Push back Lexeme into table
          outputTable.push_back(tokenLexeme); //Push back Token and Lexeme into table
        }
    }
  mainMachine_.whereAreWe_=0;
  return  outputTable;
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
    if(isalpha(lexeme[0])|| lexeme[0] == '_')
        return isKeyword(lexeme);
    else if(isdigit(lexeme[0]))
        return isDouble(lexeme);
    else
    {
        return isOperator(lexeme);
    }
}
#include "Lexical_Analyzer.h"

Lexical_Analyzer::Lexical_Analyzer(){}

/*
 * PURPOSE: Feed a line of input into our FSM and detmine it's respective token type.
 *          Store the pair into a Vector to be used later
 */
std::vector<std::vector<std::string>> Lexical_Analyzer::lexer(std::string line)
{
  // Table of pairs to be returned
  std::vector<std::vector<std::string>>  outputTable;
  
    // Take line and chop into tokens until theres nothing left
    while(!line.substr(mainMachine_.whereAreWe_).empty())
    {
      std::vector<std::string> tokenLexeme;
      std::string tempLexeme;

      // Grab the Lexeme
      tempLexeme = mainMachine_.computeToken(line.substr(mainMachine_.whereAreWe_));

      // If its not a space that was returned, push the lexeme back into our table
        if(tempLexeme!=" ")
        {
          // If we're in a comment block, ignore the lexeme
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

/*
 * PURPOSE: Determine if the lexeme is Real or Integer
 */
std::string Lexical_Analyzer::isDouble(std::string& lexeme)
{
 if(lexeme.find('.')!= std::string::npos)
    return "real";
  return "int";
}

/*
 * PURPOSE: Determine if the lexeme is a keyword or Identifier
 */
std::string Lexical_Analyzer::isKeyword(std::string& lexeme)
{
  if(_keywords.find(lexeme)!=_keywords.end())
    return "keyword";
  return "identifier";
}

/*
 * PURPOSE: Determine if the lexeme is an Operator or Seperator
 */
std::string Lexical_Analyzer::isOperator(std::string& lexeme)
{
  if(_operators.find(lexeme)!=_operators.end())
    return "operator";
  return "separator";
}

/*
 * PURPOSE: Determine what token type the lexeme is
 */
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
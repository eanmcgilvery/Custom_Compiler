/*
    CPSC 323 Project, Made by Janeen Yamak & Ean McGilvery
*/

#include "Lexical_Analyzer.cpp"
#include <iostream>
#include <fstream>
#include <exception>
#include <vector>
#include <unordered_map>

int main(int argc, char* argv[])
{
    Lexical_Analyzer lexAnalyzer;

    // Sample Files 
    std::ifstream fin("test.txt");

    // Container for the Tokens that are made
    std::vector<std::unordered_map<std::string,std::string>> tokenList;

    // Go line by line till end of file, grabbing a token from our lexer()
    // and storing it into a vector for retreval later. 
    try
    {
        while(!fin.eof())
        {
          //tokenList.push_back(lexAnalyzer.lexer()); //this will push a token from the lexer to the vector
        }
    }
    catch(const std::ifstream::failure& e)
    {
        std::cout << "ERROR: Failed to Open File.\n";
    }

    return 0;
}
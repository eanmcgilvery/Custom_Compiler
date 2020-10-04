/*
    CPSC 323 Project, Made by Janeen Yamak & Ean McGilvery
*/

#include "Lexical_Analyzer.cpp"
#include <iostream>
#include <fstream>
#include <exception>
#include <vector>

int main()
{
    Lexical_Analyzer lexAnalyzer;

    // Sample Files
    std::vector<std::vector<std::string>> listOfTokenLexemePairs;

    std::string line;
    std::string inputFile;
    std::string outputFile;
    // Go line by line till end of file, grabbing a token from our lexer()
    // and storing it into a vector for retreval later.
    std::cout << "Please Enter the complete name of your input File: \n";
    std::cin >> inputFile;

    std::cout << "Please Enter the complete name of your output File: \n";
    std::cin >> outputFile;
	try
	{
		
		std::ifstream fin(inputFile);
		
		std::ofstream fout(outputFile);
		
        fout << "========================================\n";
        fout << "Token" << "                "<<  "Lexeme \n";
        fout << "========================================\n\n";

        while(std::getline(fin, line))
		{
			// std::cout << line << "\n";
			std::vector<std::vector<std::string>> temp = lexAnalyzer.lexer(line);
 			for(size_t index = 0; index < temp.size(); index++)
 			{ //this will push a token from the lexer to the vector
 				listOfTokenLexemePairs.push_back(temp[index]);
 			}
		}	
		fin.close();
 		for(size_t i = 0; i < listOfTokenLexemePairs.size(); i++){
 			fout << listOfTokenLexemePairs[i][0] << "                " <<  listOfTokenLexemePairs[i][1] << "\n";
 		}
 		//close file that we wrote t0.
		fout.close();
				
	}
	catch(const std::ifstream::failure& e)
	{
		std::cout << "ERROR: Failed to Open File.\n";
	}

    return 0;
}
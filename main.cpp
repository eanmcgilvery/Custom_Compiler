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
    // Create an Instance of our Lex Analyzer
    Lexical_Analyzer lexAnalyzer;

    // Our table that will store the Token/Lexeme Pairs
    std::vector<std::vector<std::string>> listOfTokenLexemePairs;

    // Initalize strings for the line of output, and I/O files
    std::string line;
    std::string inputFile;
    std::string outputFile;

    std::cout << "Please Enter the complete name of your input File: \n";
    std::cin >> inputFile;

    std::cout << "Please Enter the complete name of your output File: \n";
    std::cin >> outputFile;
	try
	{
		// Open Files from user input.
		std::ifstream fin(inputFile);
		
		std::ofstream fout(outputFile);

		// Formatting Purposes of the Output file
        fout << "========================================\n";
        fout << "Token" << "                "<<  "Lexeme \n";
        fout << "========================================\n\n";

        // Go line by line till end of file, grabbing a token from our lexer()
        // and storing it into a vector for retreval later.
        while(std::getline(fin, line))
		{

			std::vector<std::vector<std::string>> temp = lexAnalyzer.lexer(line);
 			for(size_t index = 0; index < temp.size(); index++)
 			{ //this will push a token from the lexer to the vector
 				listOfTokenLexemePairs.push_back(temp[index]);
 			}
		}	
		fin.close();
        // Loop through our container and print the LExeme/Token Pairs out to the file
 		for(size_t i = 0; i < listOfTokenLexemePairs.size(); i++){
 			fout << listOfTokenLexemePairs[i][0] << "                " <<  listOfTokenLexemePairs[i][1] << "\n";
 		}

		fout.close();
	}
	catch(const std::ifstream::failure& e)
	{
		std::cout << "ERROR: Failed to Open File.\n";
	}

    return 0;
}
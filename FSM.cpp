#include "FSM.h"
#include <exception>
#include <iostream>
Fsm::Fsm()
{
    whereAreWe_ = 0;
    columnInput_ = 0;
    currentState_ = 1;

    // Load in contents of 2D Vector
    mainTable_.push_back(columns_);
    mainTable_.push_back(starting_);
    mainTable_.push_back(identifier_);
    mainTable_.push_back(endIdetifier_);
    mainTable_.push_back(number_);
    mainTable_.push_back(endNumber_);
    mainTable_.push_back(seperators_);
    mainTable_.push_back(astrisckOptions_);
    mainTable_.push_back(forwardSlashOptions_);
    mainTable_.push_back(beginComment_);
    mainTable_.push_back(endComment_);
    mainTable_.push_back(nonCompoundOperators_);
    mainTable_.push_back(notOperatorOption);
    mainTable_.push_back(greaterThanOperatorOption);
    mainTable_.push_back(LessThanOperatorOption); 
    mainTable_.push_back(equalOperatorOption);
    mainTable_.push_back(doubleEqualOperationOption);
}

std::string Fsm::computeToken(std::string inputString) // "if(a > b)"
{
    currentState_ = 1; 
    std::string lexemeToReturn = "";
    // Loop until we hit an accepting state
    for(size_t index = 0; acceptingStates_.find(currentState_) == acceptingStates_.end(); index++)
    {   
        
        // CASE: 'd'
        if(isdigit(inputString[index])){// Pass in 'd' to machine & push_back the value into lexemeToReturn
            columnInput_ = isValidInput('d'); // Grab the column
        }
        // CASE: 'l'
        else if(isalpha(inputString[index])){
            columnInput_ = isValidInput('l'); // Grab the column
        }
        else{
            if(inputString[index]==' '){ //taking care of spaces issue
                whereAreWe_++;
                break;
            }
            if(inputString[index]=='\0'){break;}

            columnInput_ = isValidInput(inputString[index]);
        }
        currentState_ = stoi(mainTable_[currentState_][columnInput_]); // Grab the next state to go to based of the input

        if(acceptingStates_.find(currentState_)==acceptingStates_.end())
        {
            lexemeToReturn.push_back(inputString[index]); //Push back charinputString[index]   
            // Log what it is ex.) real, int, keyword/identifier etc...
            whereAreWe_++;
        }   
    }
    if(lexemeToReturn.size() == 0)
    {
        whereAreWe_++;
        lexemeToReturn.push_back(inputString[0]);
    }
    return lexemeToReturn;
}

size_t Fsm::isValidInput(char inputChar)
{
    std::cout << inputChar<< "\n";
    try
    {
        std::string tempStr;
        tempStr.push_back(inputChar);
        if(inputChar=='\0'){return 1;}
        for (size_t i = 0; i < columns_.size(); i++) {
            if (columns_[i] == tempStr)
                return i;

        }
        throw -1;
    }
    catch (int e)
    {
        whereAreWe_ = 0;
    }
    return 1;
}

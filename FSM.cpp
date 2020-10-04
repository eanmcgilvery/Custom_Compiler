#include "FSM.h"
#include <exception>

// Default Contr. to initalize table and initial states
Fsm::Fsm()
{
    whereAreWe_ = 0;
    columnInput_ = 0;
    currentState_ = 1;

    flag = false;

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

/*
 * Purpose: Generate a lexeme based of a give line of input.
 */
std::string Fsm::computeToken(std::string inputString) // "if(a > b)"
{
    // Check if we need to reset the current state of the FSM, or keep it if its in the
    // "Comments" state
    if(flag)
        currentState_ = 9;
    else
        currentState_ = 1;

    // Intialize the string that we will return
    std::string lexemeToReturn="";

    // Loop until we hit an accepting state
    for(size_t index = 0; acceptingStates_.find(currentState_) == acceptingStates_.end(); index++)
    {
        // CASE: 'd' (Integer/Real/Identifier)
        if(isdigit(inputString[index])){// Pass in 'd' to machine & push_back the value into lexemeToReturn
            columnInput_ = isValidInput('d'); // Grab the column
        }
        // CASE: 'l' (Identifier)
        else if(isalpha(inputString[index])){
            columnInput_ = isValidInput('l'); // Grab the column
        }
        // CASE: Operator/Symbol
        else{
            // CASE: Hits a space, we want to ignore it
            if(inputString[index]==' ')
            {
                whereAreWe_++;
                break;
            }

            // CASE: Hits end of the string thats null terminated.
            if(inputString[index]=='\0'){break;}

            columnInput_ = isValidInput(inputString[index]); 
        }

        // Update the current state based off the input and the precious state
        currentState_ = stoi(mainTable_[currentState_][columnInput_]); // Grab the next state to go to based of the input

        // CASE: Begin Comment
        if(currentState_ == 9)
            flag = true;
        else
            flag = false;

        // CASE: We're in acomment block, Continue looping until we reach end comment
        if(flag)
        {
            whereAreWe_++;
            continue;
        }

        // CASE: Our current state isn't an accepting state, so we pushback the char
        //       and continue to loop through the string
        if(acceptingStates_.find(currentState_)==acceptingStates_.end())
        {
            lexemeToReturn.push_back(inputString[index]); //Push back charinputString[index]   
            // Log what it is ex.) real, int, keyword/identifier etc...
            whereAreWe_++;
        }   
    }

    // Eliminates accidentally returning an empty string
    if(lexemeToReturn.size() == 0 && !flag)
    {
        whereAreWe_++;
        lexemeToReturn.push_back(inputString[0]);
    }

    // CASE: We are returning the comment block, and will return it as an space
    // which will get removed in later code.
    if (currentState_ == 10)
    {
        lexemeToReturn = " ";
        whereAreWe_++;
    }

    // Return the lexeme!
    return lexemeToReturn;
}


/*
 * Purpose: verify that the given char to pushback in our lexeme is valid
 */
size_t Fsm::isValidInput(char inputChar)
{
    try
    {
        // Make the char into a string
        std::string tempStr;
        tempStr.push_back(inputChar);

        // Search in the Column of Inputs to see if its valid
        for (size_t i = 0; i < columns_.size(); i++) {
            if (columns_[i] == tempStr)
                return i;

        }
        throw -1;
    }
    catch (int e)
    {
        std::range_error("ERROR: Unknown Input!!");
    }
    return 1;
}

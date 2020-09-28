#include "FSM.h"


Fsm::Fsm()
{
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


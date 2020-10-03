#ifndef FSM_H 
#define FSM_H 
#include <set>
#include <string>
#include <vector>

class Fsm
{
  public:
    Fsm();

    std::string computeToken(std::string);
    
    size_t isValidInput(char);
    size_t whereAreWe_; // Keeps track of our position within the current string

  private:
    std::set<size_t> acceptingStates_{3,5,6,10,11};
    
    // Pass in substr(whereAreWe)
    size_t columnInput_; // Column to Navigate through
    size_t currentState_; // Row to Navigate through
    std::vector<std::vector<std::string>> mainTable_;
    
    // Rows of States: 19 columns exist, 17 Rows
    /* 0 */ const std::vector<std::string> columns_ {"l", "d", "{", "}","*", "/", ".", "(", ")", ";", "+", ",", "-","!",">","<","="," ", "_"};
    /* 1 */ const std::vector<std::string> starting_ {"2","4","6","6","7","8","1","6","6","6","11","6","11","12","13","14","15","1", "2"};
    /* 2 */ const std::vector<std::string> identifier_ {"2","2","3","3","3","3","3","3","3","3","3","3","3","3","3","3","3","3","2"};
    /* 3 */ const std::vector<std::string> endIdetifier_{"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    /* 4 */ const std::vector<std::string> number_{"5","4","5","5","5","5","4","5","5","5","5","5","5","5","5","5","5","5","5"};
    /* 5 */ const std::vector<std::string> endNumber_{"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    /* 6 */ const std::vector<std::string> seperators_{"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    /* 7 */ const std::vector<std::string> astrisckOptions_ {"11","11","11","11","11","10","11","11","11","11","11","11","11","11","11","11","11","11","11"};
    /* 8 */ const std::vector<std::string> forwardSlashOptions_ {"11","11","11","11","9","11","11","11","11","11","11","11","11","11","11","11","11","11","11"};
    /* 9 */ const std::vector<std::string> beginComment_ {"9","9","9","9","7","9","9","9","9","9","9","9","9","9","9","9","9","9","9"};
    /* 10 */ const std::vector<std::string> endComment_ {"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    /* 11 */ const std::vector<std::string> nonCompoundOperators_ {"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    /* 12 */ const std::vector<std::string> notOperatorOption {"11","11","11","11","11","10","11","11","11","11","11","11","11","11","11","11","15","11","11"};
    /* 13 */ const std::vector<std::string> greaterThanOperatorOption {"11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","15","11","11"};
    /* 14 */ const std::vector<std::string> LessThanOperatorOption {"11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","15","11","11"};
    /* 15 */ const std::vector<std::string> equalOperatorOption {"11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","16","11","11"};
    /* 16 */ const std::vector<std::string> doubleEqualOperationOption {"11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","11","11"};
};
#endif
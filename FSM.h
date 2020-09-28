#ifndef FSM_H 
#define FSM_H 
#include <set>
#include <string>
#include <vector>

class Fsm
{
  public:
    Fsm();
  private:
    std::string startingState;
    std::set<std::string> acceptingState;
    std::set<std::string> inputSymbols;
    std::set<std::string> setStates;
    std::set<std::string> epsilon;

    std::vector<std::vector<std::string>> mainTable_;
    
    // Rows of States: 18 columns exist, 7 Rows
    std::vector<std::string> columns_ {"l", "d", "{", "}","*", "/", ".", "(", ")", ";", "+", ",", "-","!",">","<","="," "};
    std::vector<std::string> starting_ {"2","4","6","6","7","8","1","6","6","6","11","6","11","12","13","14","15","1"};
    std::vector<std::string> identifier_ {"2","2","3","3","3","3","3","3","3","3","3","3","3","3","3","3","3","3"};
    std::vector<std::string> endIdetifier_{"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    std::vector<std::string> number_{"5","4","5","5","5","5","4","5","5","5","5","5","5","5","5","5","5","5"};
    std::vector<std::string> endNumber_{"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    std::vector<std::string> seperators_{"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    std::vector<std::string> astrisckOptions_ {"11","11","11","11","11","10","11","11","11","11","11","11","11","11","11","11","11","11"};
    std::vector<std::string> forwardSlashOptions_ {"11","11","11","11","9","11","11","11","11","11","11","11","11","11","11","11","11","11"};
    std::vector<std::string> beginComment_ {"9","9","9","9","7","9","9","9","9","9","9","9","9","9","9","9","9","9"};
    std::vector<std::string> endComment_ {"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    std::vector<std::string> nonCompoundOperators_ {"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};
    std::vector<std::string> notOperatorOption {"11","11","11","11","11","10","11","11","11","11","11","11","11","11","11","11","15","11"};
    std::vector<std::string> greaterThanOperatorOption {"11","11","11","11","11","10","11","11","11","11","11","11","11","11","11","11","15","11"};
    std::vector<std::string> LessThanOperatorOption {"11","11","11","11","11","10","11","11","11","11","11","11","11","11","11","11","15","11"};
    std::vector<std::string> equalOperatorOption {"11","11","11","11","11","10","11","11","11","11","11","11","11","11","11","11","16","11"};
    std::vector<std::string> doubleEqualOperationOption {"1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1","1"};

    

};
#endif
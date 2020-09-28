#ifndef FSM_H 
#define FSM_H 
#include <set>
#include <string>

class Fsm{
  public:
    
  private:
    std::string startingState;
    std::set<std::string> acceptingState;
    std::set<std::string> inputSymbols;
    std::set<std::string> setStates;
    std::set<std::string> epsilon;

};

#endif
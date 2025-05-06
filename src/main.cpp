#include <map>
#include <tuple>
#include <iostream>

#include "fsm.hpp"
#include "parameters.hpp"

int main() {
  trTable trans;
  trans.emplace(std::make_tuple(0, 0), std::make_tuple(1, 0));
  trans.emplace(std::make_tuple(0, 1), std::make_tuple(1, 1));
  trans.emplace(std::make_tuple(1, 0), std::make_tuple(0, 0));
  trans.emplace(std::make_tuple(1, 1), std::make_tuple(1, 1));

  FiniteStateMachine fsm(trans, 0);

  codeOutput result;
    
  if(fsm.tryUpdate(1, &result)) {
    std::cout << static_cast<int>(result) << fsm.getState() << std::endl;
  } else {
    std::cout << "error." << std::endl;
  }
  
  return 0;
}

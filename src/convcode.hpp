#ifndef CONVCODE
#define CONVCODE

#include <vector>

#include "fsm.hpp"
#include "parameters.hpp"

class ConvCoder {
private:
  std::vector<codeOutput> output;
  FiniteStateMachine fsm;
public:
  ConvCoder(trTable transitionTable);
  bool step(codeInput input);
  bool encode(codeInput* input, int inputlen);
  std::vector<codeOutput> getOutput();
};

#endif

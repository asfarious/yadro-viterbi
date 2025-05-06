#include <vector>

#include "fsm.hpp"
#include "parameters.hpp"
#include "convcode.hpp"

ConvCoder::ConvCoder(trTable transitionTable): fsm(&transitionTable, 0) {
  this->output = {};
}

std::vector<codeOutput> ConvCoder::getOutput() {
  std::vector<codeOutput> result(output);
  return result;
}

bool ConvCoder::step(codeInput input) {
  codeOutput nextSymbol;
  if(this->fsm.tryUpdate(input, &nextSymbol)) {
    output.emplace_back(nextSymbol);
    return true;
  } else {
    return false;
  }
}

bool ConvCoder::encode(codeInput* input, int inputlen) {
  for (int j = 0; j < inputlen; j++) {
    if(! step(input[j])) {
      return false;
    }
  }
  return true;
}


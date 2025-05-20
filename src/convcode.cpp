#include <vector>
#include <tuple>

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

void impulse2transitionTable(codeInput *impulseResponse, int constraintLength, trTable *result) {
  trTable transitionMatrix = {};
  int inputSize = __builtin_popcount(max_input);
  int outputSize = __builtin_popcount(max_output);
  // This assumes max_input and max_output to be of form 2^n-1 !!!
  
  fsmState maxState = 0;
  for(int j = 0; j < constraintLength - 1; j++) {
    maxState = (maxState << inputSize) + max_input;
  }
  /*
    i.e. for max_input = 11, K = 3
    maxState is 0 -> 11 -> 1111
  */
  
  for(fsmState state = 0; state < maxState+1; state++) {
    for(codeInput input = 0; input < max_input + 1; input++) {
      
      fsmState nextState = (state >> inputSize) + (input * (maxState + 1) >> inputSize);
      /*
	i.e., for constraint_length = 4, state ABC and input I the next state is IAB
       */

      codeInput coderRegister = input * (maxState + 1) + state;
      // for the previous example, the register will be IABC.
      
      codeOutput output = 0;
      codeOutput nextBit = 0;
      for(int j = 0; j < outputSize; j++) {
	nextBit = __builtin_popcount(impulseResponse[j] & coderRegister) % 2;
	// popcount(x) % 2 xors all the bits of x (adds them modulo 2) 
	output = (output << 1) + nextBit;
      }
      transitionMatrix.emplace(std::make_tuple(state, input), std::make_tuple(nextState, output));
    }
  }
  *result = transitionMatrix;
  return;
}

#include <vector>
#include <tuple>

#include <iostream>

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
    std::cout << (int) input << " : " << (int) nextSymbol << std::endl;
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

void impulse2transitionTable(const codeInput *impulseResponse, const int constraintLength,
			     trTable *result) {
  trTable transitionMatrix = {};
  
  fsmState maxState = (1  << (input_size * (constraintLength - 1))) - 1;
  /*
    2^{n(K-1)} - 1 is a sequence of n(K-1) ones, which is the maximum state attainable.
   */
  
  for(fsmState state = 0; state < maxState+1; state++) {
    for(codeInput input = 0; input < max_input + 1; input++) {
      
      fsmState nextState = (state >> input_size) + (input * (maxState + 1) >> input_size);
      /*
	i.e., for constraint_length = 4, state ABC and input I the next state is IAB
       */

      codeInput coderRegister = input * (maxState + 1) + state;
      // for the previous example, the register will be IABC.
      
      codeOutput output = 0;
      codeOutput nextBit = 0;
      for(int j = 0; j < output_size; j++) {
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

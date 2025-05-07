#include <climits>
#include <vector>
#include <tuple>
#include <map>

#include "parameters.hpp"
#include "viterbi.hpp"

int hammingDistance(codeOutput x1, codeOutput x2) {
  int count = __builtin_popcount(x1 ^ x2);
  return count;
}

ViterbiDecoder::ViterbiDecoder(int constraintLength, trTable transitionMatrix) {
  this->output = {};
  
  this->truncationLength = constraintLength*truncationFactor;
  this->states = constraintLength * (max_input+1);
  this->curStep = 0;
  
  this->sequences = new codeInput*[states];
  this->next_sequences = new codeInput*[states];
  for(fsmState j = 0; j < states; j++) {
    this->distances[j] = 0;
    (this->sequences)[j] = new codeInput[truncationLength+1];
    (this->next_sequences)[j] = new codeInput[truncationLength+1];
  }
  this->transitionTable = &transitionMatrix;
}

ViterbiDecoder::~ViterbiDecoder() {
  for(fsmState j = 0; j < states; j++) {
    delete (this->sequences)[j];
    delete (this->next_sequences)[j];
  }
  delete[] this->sequences;
  delete[] this->next_sequences;
}

bool ViterbiDecoder::step(codeOutput input) {
  fsmState *candidates = new fsmState[this->states];
  codeInput *newSymbol = new codeInput[this->states];
  int *new_distances = new int[this->states];
  for(fsmState j = 0; j < this->states; j++) { //sensible initial values to mitigate errors
    candidates[j] = j;
    new_distances[j] = distances[j];
    newSymbol[j] = 0;
  }
  for(fsmState j = 0; j < this->states; j++) {
    for(codeInput k = 0; k < max_input+1; k++) {
      auto maybeResult = this->transitionTable->find(std::make_tuple(j, k));
      if (maybeResult != this->transitionTable->end()) {
	fsmState destination;
	codeOutput symbol;
	std::tie(destination, symbol) = maybeResult->second;
	int new_distance = hammingDistance(symbol, input) + this->distances[j];
	if(j == 0 || new_distance < new_distances[destination]) {
	  candidates[destination] = j;
	  new_distances[destination] = new_distance;
	  newSymbol[j] = k;
	}
      } else {
	delete[] candidates;
	delete[] newSymbol;
	delete[] new_distances;
	return false;
      }
    }
  }
  
  for(fsmState j = 0; j < this->states; j++) {
    this->next_sequences[j] = this->sequences[candidates[j]];
    this->next_sequences[j][curStep % (truncationLength + 1)] = newSymbol[j];
    this->distances[j] = new_distances[j];
  }
  codeInput **temp = this->sequences;
  this->sequences = this->next_sequences;
  this->next_sequences = temp;
  delete[] candidates;
  delete[] newSymbol;
  delete[] new_distances;
  
  if(curStep >= truncationLength) {
    this->output.emplace_back(sequences[0][(curStep + 1) % (truncationLength + 1)]);
    return true;
  }

  return false;
}

bool ViterbiDecoder::decode(codeInput* input, int inputlen) {
  for (int j = 0; j < inputlen; j++) {
    if(! step(input[j])) {
      return false;
    }
  }
  return true;
}

std::vector<codeInput> ViterbiDecoder::getOutput() {
  std::vector<codeInput> result(output);
  return result;
}

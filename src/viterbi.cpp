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
  this->states = (max_input+1) << (constraintLength - 1);
  // Assumming max_input is 2^n-1 there are 2^{n(K-1)} = 2^n 2^{K-1} states total
  this->curStep = 0;
  this->flushedSteps = 0;
  
  this->sequences = new codeInput*[states];
  this->next_sequences = new codeInput*[states];
  for(fsmState j = 0; j < states; j++) {
    this->distances[j] = 0;
    (this->sequences)[j] = new codeInput[truncationLength+1];
    (this->next_sequences)[j] = new codeInput[truncationLength+1];
  }
  
  this->wasReached = new bool[states] {};
  wasReached[0] = true;
  
  this->transitionTable = transitionMatrix;
}

ViterbiDecoder::~ViterbiDecoder() {
  for(fsmState j = 0; j < states; j++) {
    delete (this->sequences)[j];
    delete (this->next_sequences)[j];
  }
  delete[] this->sequences;
  delete[] this->next_sequences;
}

/*
  we keep track of up to "states" sequences, each 
 */
bool ViterbiDecoder::step(codeOutput input) {
  fsmState *candidates = new fsmState[this->states];
  codeInput *newSymbol = new codeInput[this->states];
  int *new_distances = new int[this->states];

  for(fsmState j = 0; j < this->states; j++) { //sensible initial values to mitigate errors
    candidates[j] = j;
    new_distances[j] = INT_MAX;
    newSymbol[j] = 0;
  }
  for(fsmState j = 0; j < this->states; j++) {
    if(!wasReached[j]) { // only states that were reached are propagated
      continue;
    }
    for(codeInput k = 0; k < max_input+1; k++) {
      auto maybeResult = this->transitionTable.find(std::make_tuple(j, k));
      if (maybeResult != this->transitionTable.end()) {
	fsmState destination;
	codeOutput symbol;
	std::tie(destination, symbol) = maybeResult->second;
	int new_distance = hammingDistance(symbol, input) + this->distances[j];
	if(!wasReached[destination] ||  new_distance < new_distances[destination]) {
	  wasReached[destination] = true;
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
    for(int k = 0; k < truncationLength + 1; k++) {
      this->next_sequences[j][k] = this->sequences[candidates[j]][k];
    }
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
    if(flushedSteps > 0) {
      --flushedSteps;
    } else {
      this->output.emplace_back(sequences[0][(curStep + 1) % (truncationLength + 1)]);
    }
    return true;
  }

  return false;
}

bool ViterbiDecoder::decode(codeInput* input, int inputlen) {
  for(int j = 0; j < inputlen; j++) {
    if(! step(input[j])) {
      return false;
    }
  }
  return true;
}

void ViterbiDecoder::flushOutput() {
  int candidate = 0;
  int distance = distances[candidate];
  for(fsmState j = 0; j < this->states; j++) {
    if(distances[j] < distance) {
      candidate = j;
      distance = distances[candidate];
    }
  }
  for(int k = 1; k < truncationLength + 1; k++) { // SIC! INITAL K IS 1!
    this->output.emplace_back(sequences[candidate][(curStep + 1 + k) % (truncationLength + 1)]);
  }
  flushedSteps = truncationLength;
}

std::vector<codeInput> ViterbiDecoder::getOutput() {
  std::vector<codeInput> result(output);
  return result;
}

#ifndef VITERBI
#define VITERBI

#include <vector>
#include "parameters.hpp"

const int truncationFactor = 5;

class ViterbiDecoder {
private:
  std::vector<codeInput> output;
  
  int truncationLength;
  bool isMature;
  int curStep;
  
  // calculated distances for different paths and corresponding sequences
  codeInput **sequences;
  codeInput **next_sequences;
  int *distances;
  
  fsmState states;
  trTable *transitionTable;
public:
  ViterbiDecoder(int constraintLength, trTable transitionMatrix);
  bool step(codeOutput input);
  bool decode(codeOutput input);
  ~ViterbiDecoder();
};

#endif

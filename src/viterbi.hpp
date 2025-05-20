#ifndef VITERBI
#define VITERBI

#include <vector>
#include "parameters.hpp"

const int truncationFactor = 5;

class ViterbiDecoder {
private:
  std::vector<codeInput> output;
  
  int truncationLength;
  int curStep;
  int flushedSteps;
  
  codeInput **sequences;
  // sequence[j] is the most probable sequence to currently be in state j

  codeInput **next_sequences; // the second buffer to swap with sequences

  int *distances;
  // distances[j] is the distance of the most probable sequence to currently be in state j

  bool *wasReached;
  // wasReached[j] is whether state j was reached by any sequence yet
  
  fsmState states;
  trTable transitionTable;
public:
  ViterbiDecoder(int constraintLength, trTable transitionMatrix);
  bool step(codeOutput input);
  bool decode(codeOutput* input, int inputlen);
  void flushOutput();
  std::vector<codeInput> getOutput();
  ~ViterbiDecoder();
};

#endif

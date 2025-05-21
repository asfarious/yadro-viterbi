#include <map>
#include <tuple>
#include <iostream>
#include <vector>
#include <bitset>
#include <random>

#include "parameters.hpp"
#include "convcode.hpp"
#include "viterbi.hpp"
#include "channel.hpp"

int main() {
  
  trTable transitionMatrix = {};

  transitionMatrix = impulse2transitionTable(impulseResponse, constraintLength); 
  
  std::minstd_rand sampleGenerator;
  std::uniform_int_distribution<codeInput> sampleDistribution(0, max_input);
  codeInput input[sample_size] = {};
  Channel channel(0);

  std::vector<codeOutput> code;
  
  double probability_step = 0.5/static_cast<double>(probability_granularity);

  std::cout << "probability, bit error rate" << std::endl;
  
  for(double p = 0; p < 0.5; p = p + probability_step) {
    ConvCoder coder(transitionMatrix);
    ViterbiDecoder decoder(constraintLength, transitionMatrix);
    channel.setProbability(p);

    for(int j = 0; j < sample_size; j++) {
      input[j] = static_cast<codeInput>(sampleDistribution(sampleGenerator));
    }
    if(!coder.encode(input, sample_size)) {
      std::cout << "error.";
      return 1;
    } else {
      code = coder.getOutput();
    }
    int codeLen = code.size();
    codeOutput *noisyCode = channel.applyNoise(&code[0], codeLen);

    decoder.decode(noisyCode, codeLen);
    decoder.flushOutput();
    std::vector<codeInput> decoded = decoder.getOutput();
    int wrongBits = 0;
    for(int j = 0; j < sample_size; j++) {
      wrongBits = wrongBits + __builtin_popcount(decoded[j] ^ input[j]);
    }
    double ber = static_cast<double>(wrongBits)/(static_cast<double>(sample_size)*input_size);
    std::cout << p << ", " << ber << std::endl;
  }
    
  return 0;
}

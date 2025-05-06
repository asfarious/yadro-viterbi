#include <random>

#include "channel.hpp"
#include "parameters.hpp"

Channel::Channel(double probability): distribution(0, 100), generator() {
  this->probability = probability;
}

void Channel::generateMask(codeOutput* mask, int len) {
  int size = __builtin_popcount(max_output); // works if max_output is of form 2^n - 1
  for (int j = 0; j < len; j++) {
    mask[j] = 0;
    for(int k = 0; k < size; k++) {
      int randNum = this->distribution(this->generator);
      if(randNum < static_cast<int>(this->probability * 100)) {
	mask[j] += 1;
      }
      mask[j] = mask[j] << 1;
    }
  }
  return;
}

codeOutput* Channel::applyNoise(codeOutput* signal, int len) {

  codeOutput* result = new codeOutput[len];
  
  codeOutput* mask = new codeOutput[len];
  generateMask(mask, len);

  for(int j = 0; j < len; j++) {
    result[j] = signal[j] ^ mask[j];
  }
  
  delete[] mask;
  return result;
}

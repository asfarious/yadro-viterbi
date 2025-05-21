#ifndef CHANNEL
#define CHANNEL

#include <random>

#include "parameters.hpp"

typedef std::minstd_rand Generator;
typedef std::uniform_int_distribution<int> Distribution;

class Channel {
private:
  Distribution distribution;
  Generator generator;
  double probability;
  void generateMask(codeOutput* mask, int len);
public:
  Channel(double probability);
  void setProbability(double probability);
  codeOutput* applyNoise(codeOutput* signal, int len);
};

#endif

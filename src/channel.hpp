#ifndef CHANNEL
#define CHANNEL

#include <random>

#include "parameters.hpp"

typedef std::minstd_rand Generator;
typedef std::uniform_int_distribution<> Distribution;

class Channel {
private:
  Distribution distribution;
  Generator generator;
  double probability;
  void generateMask(codeOutput* mask, int len);
public:
  Channel(double probability);
  codeOutput* applyNoise(codeOutput* signal, int len);
};

#endif

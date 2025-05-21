#ifndef PARAMETERS
#define PARAMETERS
/*
  this header contains parameters that may be decoupled from the rest of the code
  if templates are to be used
 */

#include <map>
#include <tuple>

/*
  default code input and output type is char;
  it is parametrized here to be easily replaced by templates if needed
 */
typedef unsigned char codeInput;
typedef unsigned char codeOutput;

// code parameters
const int input_size = 2;
const int output_size = 4;
const int constraintLength = 3;
const codeInput max_input = (1 << input_size) - 1;
const codeOutput max_output = (1 << output_size) - 1;
const codeInput impulseResponse[4] = {0b100010, 0b100001, 0b110011, 0b111111};

/*
  details of internal specification of the finite state machine implementing the convolution code
 */
typedef unsigned int fsmState;
typedef std::map<std::tuple<fsmState, codeInput>, std::tuple<fsmState, codeOutput>> trTable;

#endif

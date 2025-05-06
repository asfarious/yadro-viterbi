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

const codeInput max_input = 3; //11
const codeOutput max_output = 15; //1111

/*
  details of internal specification of the finite state machine implementing the convolution code
 */
typedef unsigned int fsmState;
typedef std::map<std::tuple<fsmState, codeInput>, std::tuple<fsmState, codeOutput>> trTable;

#endif

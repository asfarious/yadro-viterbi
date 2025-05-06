#ifndef FSM
#define FSM

/*
  A simple templateless finite state machine using integers for both states and input/output
 */

#include <map>
#include <tuple>

typedef unsigned int fsmState;
typedef unsigned int fsmInput;
typedef unsigned int fsmOutput;
typedef std::map<std::tuple<fsmState, fsmInput>, std::tuple<fsmState, fsmOutput>> trTable;

class IntStateMachine {
private:
  fsmState curState;
  trTable transitionTable;
public:
  IntStateMachine(trTable transitionTalbe, fsmState initialState);
  bool tryUpdate(fsmInput input, fsmOutput *output);
};

#endif

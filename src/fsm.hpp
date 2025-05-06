#ifndef FSM
#define FSM

/*
  A simple templateless finite state machine using integers for states and bytes for input/output
 */

#include <map>
#include <tuple>

#include "parameters.hpp"

class FiniteStateMachine {
private:
  fsmState curState;
  trTable *transitionTable;
public:
  FiniteStateMachine(trTable *transitionTable, fsmState initialState);
  bool tryUpdate(codeInput input, codeOutput *output);
  fsmState getState();
  trTable* getTransitionMatrix();
};

#endif

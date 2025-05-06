#include <map>
#include <tuple>

#include "fsm.hpp"

IntStateMachine::IntStateMachine(trTable transitionTable, fsmState initialState) {
  this->curState = initialState;
  this->transitionTable = transitionTable;
}

bool IntStateMachine::tryUpdate(fsmInput input, fsmOutput *output) {
  auto maybeResult = this->transitionTable.find(std::make_tuple(this->curState, input));
  if (maybeResult != this->transitionTable.end()) {
    fsmState newState;
    std::tie(newState, *output) = maybeResult->second;
    this->curState = newState;
    return true;
  } else {
    return false;
  }
}

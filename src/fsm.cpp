#include <map>
#include <tuple>

#include <iostream>

#include "fsm.hpp"
#include "parameters.hpp"

FiniteStateMachine::FiniteStateMachine(trTable transitionTable, fsmState initialState) {
  this->curState = initialState;
  this->transitionTable = transitionTable;
}

bool FiniteStateMachine::tryUpdate(codeInput input, codeOutput *output) {
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

fsmState FiniteStateMachine::getState() {
  return this->curState;
}

trTable FiniteStateMachine::getTransitionMatrix() {
  return this->transitionTable;
}

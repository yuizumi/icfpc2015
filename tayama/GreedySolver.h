#ifndef GREEDY_SOLVER_H_
#define GREEDY_SOLVER_H_

#include <string>

#include "./Solver.h"

class Unit;

class GreedySolver : public Solver {
 public:

  void Solve(const GameState &gameState, GameState *result);
  
 private:

  bool CastPhrase(const std::string &phrase, GameState *state);
};

#endif

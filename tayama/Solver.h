#ifndef SOLVER_H_
#define SOLVER_H_

class GameState;

class Solver {
 public:  
  virtual ~Solver() {}

  virtual void Solve(const GameState &gameState, GameState *result) = 0;
};

#endif

#include "./GreedySolver.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "./util.h"
#include "../yuizumi/basic.h"
#include "../yuizumi/board.h"
#include "../yuizumi/debug.h"
#include "../yuizumi/game.h"
#include "../yuizumi/unit.h"

using namespace std;

const vector<string> power_phrases = {
  "Ia! Ia!",
  "Ei!",
  "Yuggoth",
  "p",
  "b",
  "a",
  "l",
  "d",
  "k"
};

void GreedySolver::Solve(const GameState &gameState, GameState *result) {
  cerr << "Start solving." << endl;
  bool done = true;
  int count = 0;
  while (done) {
    done = false;
    debug::Print(*result);
    cerr << "gameover? " << result->gameover() << endl;
    if (result->gameover()) {
      break;
    }
    for (auto phrase : power_phrases) {
      cerr << "Trying phrase " << phrase << endl;
      unique_ptr<GameState> cloned = result->Clone();
      if (CastPhrase(phrase, cloned.get())) {
        cerr << "Success: " << phrase << endl;
        cloned->Swap(result);
        done = true;
        break;
      } else { cerr << "Failed: " << phrase << endl; }
    }
    cerr << "Finished attempt. Continue? " << done << endl;
  }
  cerr << "Finish solving." << endl;
}

bool GreedySolver::CastPhrase(const string &phrase, GameState *state) {
  cerr << "Casting phrase " << phrase << endl;
  bool failed = false;
  for (auto c : phrase) {
    const Unit &unit = state->unit();
    auto command = kCharToCommand.find(c)->second;
    // TODO(tayama): Move this logic to GameState.
    
    switch(command) {
    case kNone:
      break;
    case kMoveE:
      if (unit.CanMoveE()) state->Invoke(kMoveE); else failed = true;
      break;
    case kMoveW:
      if (unit.CanMoveW()) state->Invoke(kMoveW); else failed = true;
      break;
    case kMoveSE:
      if (unit.CanMoveSE()) state->Invoke(kMoveSE); else failed = true;
      break;
    case kMoveSW:
      if (unit.CanMoveSW()) state->Invoke(kMoveSW); else failed = true;
      break;
    case kRotateRight:
      if (unit.CanRotateRight()) state->Invoke(kRotateRight); else failed = true;
      break;
    case kRotateLeft:
      if (unit.CanRotateLeft()) state->Invoke(kRotateLeft); else failed = true;
      break;
    }
    if (failed) break;
  }
  return !failed;
}

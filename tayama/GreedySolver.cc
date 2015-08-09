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
  "ia! ia!",
  "ei!",
  "yuggoth",
  "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn.",
  "cthulhu fhtagn!"
  "a",
  "l",
  "d",
  "k",
  "p",
  "b"
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
  const Unit &unit = state->unit();
  for (auto c : phrase) {
    auto command = kCharToCommand.find(c)->second;
    cerr << "Can move " << command << " as " << c << endl;
    bool can_move = state->CanMove(command);
    cerr << "Can move " << command << " as " << c << ": done" << endl;
    if (can_move) {
      cerr << "Invoking " << command << " as " << c << ": done" << endl;
      state->Invoke(command);
      cerr << "Invoking " << command << " as " << c << ": done" << endl;
    } else {
      return false;
    }
  }
  return true;
}

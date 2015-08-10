#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "basic.h"
#include "board.h"
#include "debug.h"
#include "game.h"
#include "unit.h"

using namespace std;

std::map<char, Command> initCharToCommand() {
  std::map<char, Command> m;
  m['p'] = kMoveW;
  m['\''] = kMoveW;
  m['!'] = kMoveW;
  m['.'] = kMoveW;
  m['0'] = kMoveW;
  m['3'] = kMoveW;
    
  m['b'] = kMoveE;
  m['c'] = kMoveE;
  m['e'] = kMoveE;
  m['f'] = kMoveE;
  m['y'] = kMoveE;
  m['2'] = kMoveE;
    
  m['a'] = kMoveSW;
  m['g'] = kMoveSW;
  m['h'] = kMoveSW;
  m['i'] = kMoveSW;
  m['j'] = kMoveSW;
  m['4'] = kMoveSW;
    
  m['l'] = kMoveSE;
  m['m'] = kMoveSE;
  m['n'] = kMoveSE;
  m['o'] = kMoveSE;
  m[' '] = kMoveSE;
  m['5'] = kMoveSE;
    
  m['d'] = kRotateRight;
  m['q'] = kRotateRight;
  m['r'] = kRotateRight;
  m['v'] = kRotateRight;
  m['z'] = kRotateRight;
  m['1'] = kRotateRight;
    
  m['k'] = kRotateLeft;
  m['s'] = kRotateLeft;
  m['t'] = kRotateLeft;
  m['u'] = kRotateLeft;
  m['w'] = kRotateLeft;
  m['x'] = kRotateLeft;
    
  m['\t'] = kNone;
  m['\n'] = kNone;
  m['\r'] = kNone;

  return m;
};

const std::map<char, Command> kCharToCommand = initCharToCommand();

bool CastPhrase(const string &phrase, GameState *state) {
  for (auto c : phrase) {
    auto command = kCharToCommand.find(c)->second;
    bool can_move = state->IsValid(command);
    if (can_move) {
      state->Invoke(command);
    } else {
      return false;
    }
  }
  return true;
}

int Score(const GameState &state) {
  int count = 0, score = 0;
  for (int y = 0; y < state.board().height(); ++y) {
    for (int x = 0; x < state.board().width(); ++x) {
      if (state.board().get(x, y)) {
        ++count;
        int penalty = state.board().height() - y;
        score -= penalty * penalty;
      }
    }
  }
  return count == 0 ? 0 : score / count;
}

void Solve(GameState *state) {
  vector<string> power_phrases = {
    "ia! ia!",
    "ei!",
    "yuggoth",
    "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn.",
    "cthulhu fhtagn!"
  };

  vector<string> commands = {
    "a",
    "l",
    "d",
    "k",
    "p",
    "b"
  };

  vector<string> phrases(power_phrases.begin(), power_phrases.end());
  phrases.insert(phrases.end(), commands.begin(), commands.end());

  while (true) {
    if (state->gameover()) {
      debug::Print(*state);
      break;
    }
    int currentScore = 0x80000000;
    unique_ptr<GameState> bestState = state->Clone();
    for (int attempt = 0; attempt < 1000; ++attempt) {
      // random_shuffle(phrases.begin(), phrases.begin() + power_phrases.size());
      // random_shuffle(phrases.begin() + power_phrases.size(), phrases.end());
      random_shuffle(phrases.begin(), phrases.end());
      unique_ptr<GameState> cloned = state->Clone();
      int phraseBonus = 0;
      for (const auto& phrase : phrases) {
        if (CastPhrase(phrase, cloned.get())){
          phraseBonus = (phrase.length() - 1) * 2 * 5;
          break;
        }
      }
      int newScore = Score(*cloned) + phraseBonus;
      if (currentScore < newScore) {
        currentScore = newScore;
        bestState->Swap(cloned.get());
      } else {
      }
    }

    bestState->Swap(state);
    debug::Print(*bestState);
  }
}

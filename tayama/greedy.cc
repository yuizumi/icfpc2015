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

bool CastPhrase(const string &phrase, GameState *state) {
  for (auto c : phrase) {
    bool can_move = state->IsValid(c);
    if (can_move) {
      state->Invoke(c);
    } else {
      return false;
    }
  }
  return true;
}

bool has(const GameState &state, int x, int y) {
  if (state.board().get(x, y)) return true;
  for (const auto &member : state.unit().members()) {
    if (member == (Cell) { x, y }) return true;
  }
  return false;
}

int Score(const GameState &state) {
//   int count = 0, score = 0;
//   for (int y = 0; y < state.board().height(); ++y) {
//     for (int x = 0; x < state.board().width(); ++x) {
//       if (has(state, x, y)) {
//         ++count;
//         int penalty = state.board().height() - y;
//         score -= penalty * penalty;
//       }
//     }
//   }  
//  return count == 0 ? 0 : (score << 16) / count;
  return 0;
}

void Solve(GameState *state) {
  vector<string> power_phrases = {
    "ei!",
    "ia! ia!",
    "yuggoth",
    "Planet 10",
    "Tsathoggua",
    "Necronomicon",
    "cthulhu fhtagn!",
    "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn.",
    "In his house at R'lyeh dead Cthulhu waits dreaming."
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
    for (int attempt = 0; attempt < 1; ++attempt) {
      
      unique_ptr<GameState> cloned = state->Clone();
      int phraseBonus = 0;
      for (int iteration = 0; iteration < phrases.size(); ++iteration) {
        const string &phrase = phrases[iteration];
        if (CastPhrase(phrase, cloned.get())){
          phraseBonus = (phrase.length() - 1) * 2 * 0;
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
    cerr << "Evaluation: " << Score(*bestState) << endl;
  }
}

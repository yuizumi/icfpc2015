#include <cstdlib>
#include <ctime>
#include <iostream>
#include "debug.h"
#include "game.h"

using namespace std;

namespace {
using Long = long long;

const int kNumTrials = 100;
const int kScoreMulti = 100;

int Random(int max) {
    while (true) {
        int n = rand() / (RAND_MAX / max);
        if (n < max) return n;
    }
}

Command RandomCommand() {
    return static_cast<Command>(Random(6) + 1);
}
}  // namespace

void Solve(GameState* base_state) {
    srand(time(nullptr));

    unique_ptr<GameState> best_state;
    int best_score = 0;
    for (int trial = 0; trial < kNumTrials; trial++) {
        unique_ptr<GameState> state(base_state->Clone());
        while (!state->gameover()) {
            Command command = RandomCommand();
            if (!state->IsValid(command))
                continue;
            state->Invoke(command);
        }
        Long score = static_cast<Long>(state->move_score()) * kScoreMulti +
            state->commands().size();
        debug::Print(*state);
        if (best_score < score) {
            best_state.reset(state.release());
            best_score = score;
        }
    }
    if (best_state) {
        base_state->Swap(best_state.get());
    }
}

#include <iostream>
#include "debug.h"
#include "game.h"

using namespace std;

namespace {
inline Command Choose(const GameState& state,
                      Command c1, Command c2, Command c3) {
    if (state.CanMove(c1) && state.IsValid(c1))
        return c1;
    if (state.CanMove(c2) && state.IsValid(c2))
        return c2;
    if (state.CanMove(c3) && state.IsValid(c3))
        return c3;
    return kNone;
}

void Drop(GameState* state, int x) {
    while (true) {
        const Cell& p = state->unit().pivot();
        int wx = (p.y % 2 == 0) ? p.x - 1 : p.x;
        int ex = (p.y % 2 == 0) ? p.x : p.x + 1;
        Command command;
        if (abs(wx - x) < abs(ex - x)) {
            command = Choose(*state, kMoveSW, kMoveSE, kMoveW);
        } else {
            command = Choose(*state, kMoveSE, kMoveSW, kMoveE);
        }
        if (command == kNone)
            break;
        state->Invoke(command);
    }
}
}  // namespace

void Solve(GameState* state) {
    int width = state->board().width();
    while (!state->gameover()) {
        debug::Print(*state);
        unique_ptr<GameState> best_state(state->Clone());
        for (int x = 0; x < width; x++) {
            unique_ptr<GameState> new_state(state->Clone());
            Drop(new_state.get(), x);
            if (best_state->unit().pivot().y < new_state->unit().pivot().y)
                best_state.reset(new_state.release());
        }
        best_state->Invoke(kMoveSW);
        state->Swap(best_state.get());
    }
}

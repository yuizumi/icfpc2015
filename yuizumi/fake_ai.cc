#include "debug.h"
#include "game.h"

void Solve(GameState* state) {
    debug::Print(*state);
    state->Invoke(kRotateRight);
    debug::Print(*state);
    state->Invoke(kMoveE);
    debug::Print(*state);
    state->Invoke(kMoveSW);
    debug::Print(*state);
    state->Invoke(kMoveW);
    debug::Print(*state);
    state->Invoke(kRotateRight);
    debug::Print(*state);
}

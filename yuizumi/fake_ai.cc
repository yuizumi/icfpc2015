#include "debug.h"
#include "game.h"

void Solve(GameState* state) {
    debug::Print(*state);
    state->Invoke(kMoveE );  // "e"
    debug::Print(*state);
    state->Invoke(kMoveSW);  // "i"
    debug::Print(*state);
    state->Invoke(kMoveW );  // "!"
    debug::Print(*state);
}

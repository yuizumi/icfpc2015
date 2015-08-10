#include <string>
#include <vector>

#include "debug.h"
#include "game.h"

using namespace std;

void Solve(GameState* state, const vector<string> &power_phrases) {
    debug::Print(*state);
    state->Invoke(kMoveSW);
    debug::Print(*state);
    state->Invoke(kMoveSE);
    debug::Print(*state);
    state->Invoke(kRotateRight);
    debug::Print(*state);
    state->Invoke(kMoveE);
    debug::Print(*state);
    state->Invoke(kMoveSW);
    debug::Print(*state);
    state->Invoke(kMoveW);
    debug::Print(*state);
    state->Invoke(kRotateLeft);
    debug::Print(*state);
    state->Invoke(kMoveSW);
    debug::Print(*state);
    state->Invoke(kMoveSW);
    debug::Print(*state);
    state->Invoke(kMoveW);
    debug::Print(*state);
}

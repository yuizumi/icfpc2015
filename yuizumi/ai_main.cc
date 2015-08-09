#include <iostream>
#include <vector>

#include "basic.h"
#include "board.h"
#include "game.h"
#include "input.h"
#include "unit.h"

using namespace std;

void FakeAI(GameState* state) {
    state->Invoke(kMoveE);
    state->Invoke(kMoveSW);
    state->Invoke(kMoveW);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    GameSpec spec = ReadGameSpec(cin);

    cout << spec.seeds.size() << endl;
    for (int seed : spec.seeds) {
        GameState state(spec.units, spec.board->Clone(), seed,
                        spec.length);
        Solve(&state);
        cout << spec.id << " " << seed << endl;
        for (Command command : state.commands()) cout << command;
        cout << endl; cout.flush();
    }

    return 0;
}

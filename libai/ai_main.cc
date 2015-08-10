#include <iostream>
#include "game.h"
#include "input.h"

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    GameSpec spec = ReadGameSpec(cin);

    cout << spec.seeds.size() << endl;
    for (int seed : spec.seeds) {
        GameState state(spec.units, spec.board->Clone(), seed,
                        spec.length);
        Solve(&state);
        cout << spec.id << " " << seed << endl << state.commands() << endl;
        cout.flush();
    }

    return 0;
}

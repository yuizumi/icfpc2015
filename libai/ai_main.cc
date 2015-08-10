#include <iostream>
#include <vector>
#include "game.h"
#include "input.h"

using namespace std;

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> power_phrases;
    for (int i = 1; i < argc; ++i) {
      power_phrases.push_back(argv[i]);
    }

    GameSpec spec = ReadGameSpec(cin);

    cout << spec.seeds.size() << endl;
    for (int seed : spec.seeds) {
        GameState state(spec.units, spec.board->Clone(), seed,
                        spec.length);
        Solve(&state, power_phrases);
        cout << spec.id << " " << seed << endl << state.commands() << endl;
        cout.flush();
    }

    return 0;
}

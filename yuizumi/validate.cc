#include <cassert>
#include <cctype>
#include <iostream>
#include <map>
#include <string>
#include "debug.h"
#include "game.h"
#include "input.h"

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    GameSpec spec = ReadGameSpec(cin);

    int num_seeds;
    cin >> num_seeds;
    for (int i = 0; i < num_seeds; i++) {
        int out_id, out_seed;
        string out_seq;
        cin >> out_id >> out_seed;
        getline(cin, out_seq);
        getline(cin, out_seq);

        assert(spec.id == out_id);

        cout << "#" << i << " (Seed: " << out_seed << "): ";
        GameState state(spec.units, spec.board->Clone(), out_seed,
                        spec.length);
        for (size_t i = 0; i < out_seq.length(); i++) {
            char command = tolower(out_seq[i]);
            if (!state.IsValid(command)) {
                cout << "Invalid at " << i << "." << endl;
                return 1;
            }
            state.Invoke(command);
            debug::Print(state);
        }
        cout << "Valid, Score = " << state.move_score() << "." << endl;
    }

    return 0;
}

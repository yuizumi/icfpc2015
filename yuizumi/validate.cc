#include <cassert>
#include <cctype>
#include <iostream>
#include <map>
#include <string>
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

        GameState state(spec.units, spec.board->Clone(),
                        out_seed, spec.length);
        for (char ch : out_seq) {
            cout << ch;
            Command command = static_cast<Command>(ch - '0');
            state.Invoke(command);
        }
    }
    cout << " -- no problem found." << endl;

    return 0;
}

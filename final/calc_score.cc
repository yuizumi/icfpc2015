#include <cassert>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "game.h"
#include "input.h"

using namespace std;

static int GetScore(const GameState& state) {
    return state.move_score();
}

struct Output {
    int id, seed;
    string solution;
};

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    GameSpec spec = ReadGameSpec(cin);

    int total_score = 0;

    int num_seeds;
    cin >> num_seeds;
    vector<Output> outputs;
    for (int i = 0; i < num_seeds; i++) {
        Output output;
        string seq;
        cin >> output.id >> output.seed;
        getline(cin, seq);
        getline(cin, seq);
        if (!cin) break;

        assert(spec.id == output.id);

        GameState state(spec.units, spec.board->Clone(), output.seed, spec.length);
        for (size_t i = 0; i < seq.length(); i++) {
            char command = tolower(seq[i]);
            if (!state.IsValid(command)) {
                break;
            }
            state.Invoke(command);
        }
        output.solution = state.commands();
        outputs.push_back(output);
        total_score += GetScore(state);
    }

    cout << "$score: " << total_score << endl;
    cout << outputs.size() << endl;
    for (const Output& output : outputs) {
        cout << output.id << " " << output.seed << endl << output.solution << endl;
    }

    return 0;
}

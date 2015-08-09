#include <iostream>
#include <vector>

#include "basic.h"
#include "board.h"
#include "game.h"
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

    int id; cin >> id;

    int num_units; cin >> num_units;
    vector<UnitSpec> units;
    for (int i = 0; i < num_units; i++) {
        int size; cin >> size;
        vector<Cell> members(size);
        for (int j = 0; j < size; j++) cin >> members[j].x >> members[j].y;
        Cell pivot; cin >> pivot.x >> pivot.y;
        units.emplace_back(UnitSpec(members, pivot));
    }

    int width, height; cin >> width >> height;
    unique_ptr<Board> board(new Board(width, height));
    int num_filled; cin >> num_filled;
    for (int i = 0; i < num_filled; i++) {
        int x, y; cin >> x >> y;
        board->set(x, y, true);
    }

    int length; cin >> length;
    int num_seeds; cin >> num_seeds;
    
    for (int i = 0; i < num_seeds; i++) {
        int seed; cin >> seed;

        GameState state(units, board->Clone(), seed, length);
        Solve(&state);

        cout << id << " " << seed << endl;
        cout << state.commands().size() << endl;
        for (Command command : state.commands()) cout << command;
        cout << endl; cout.flush();
    }

    return 0;
}

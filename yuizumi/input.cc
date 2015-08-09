#include "input.h"

using namespace std;

GameSpec ReadGameSpec(istream& in) {
    GameSpec spec;

    in >> spec.id;

    int num_units; in >> num_units;
    spec.units.reserve(num_units);
    for (int i = 0; i < num_units; i++) {
        int size; in >> size;
        vector<Cell> members(size);
        for (int j = 0; j < size; j++) in >> members[j].x >> members[j].y;
        Cell pivot; in >> pivot.x >> pivot.y;
        spec.units.emplace_back(UnitSpec(members, pivot));
    }

    int width, height; in >> width >> height;
    unique_ptr<Board> board(new Board(width, height));
    int num_filled; in >> num_filled;
    for (int i = 0; i < num_filled; i++) {
        int x, y; in >> x >> y;
        board->set(x, y, true);
    }
    spec.board.reset(board.release());

    in >> spec.length;

    int num_seeds; in >> num_seeds;
    spec.seeds.resize(num_seeds);
    for (int i = 0; i < num_seeds; i++) in >> spec.seeds[i];

    return spec;
}

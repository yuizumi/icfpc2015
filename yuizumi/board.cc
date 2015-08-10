#include "board.h"

#include <algorithm>
#include "unit.h"

using namespace std;

Board::Board(int w, int h) : state_(h, vector<int>(w)) {
}

std::unique_ptr<Board> Board::Clone() const {
    return std::unique_ptr<Board>(new Board(state_));
}

int Board::Lock(const Unit& unit) {
    for (const Cell& cell : unit.members()) {
        set(cell, true);
    }

    int y1 = height();

    for (int y0 = height() - 1; y0 >= 0; y0--) {
        if (count(state_[y0].begin(), state_[y0].end(), 0) == 0) {
            continue;
        }
        --y1;
        if (y0 != y1) {
            copy(state_[y0].begin(), state_[y0].end(), state_[y1].begin());
        }
    }
    for (int y = y1 - 1; y >= 0; y--) {
        fill(state_[y].begin(), state_[y].end(), 0);
    }
    return y1;
}

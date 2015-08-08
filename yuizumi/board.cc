#include "board.h"

using namespace std;

Board::Board(int w, int h) : w_(w), h_(h), states_(h, vector<int>(w, 0)) {
}

void Board::ClearRows() {
    int y1 = 0;

    for (int y0 = 0; y0 < h_; y0++) {
        if (count(states_[y0].begin(), states_[y0].end(), 0) > 0) {
            continue;
        }
        if (y0 > y1) {
            copy(states_[y1].begin(), states_[y1].end(), states_[y0].begin());
        }
        ++y1;
    }
    for (; y1 < h_; y1++) {
        fill(states_[y1].begin(), states_[y1].end(), 0);
    }
}

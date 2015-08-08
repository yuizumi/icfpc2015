#ifndef BOARD_H_
#define BOARD_H_

#include <cassert>
#include <vector>
#include "basic.h"
#include "macros.h"

class Board {
public:
    Board(int w, int h);

    std::unique_ptr<Board> Clone() const;
    void ClearRows();

    bool get(const Cell& cell) const {
        return get(cell.x, cell.y);
    }

    bool get(int x, int y) const {
        if (x < 0 || x >= width() || y < 0 || y >= height()) {
            return false;
        }
        return state_[x][y];
    }

    void set(const Cell& cell, bool value) {
        set(cell.x, cell.y);
    }

    void set(int x, int y, bool value) {
        assert(x >= 0 && x < width() && y >= 0 && y < height());
        state_[x][y] = value;
    }

    int width() const {
        return static_cast<int>(state_[0].size());
    }

    int height() const {
        return static_cast<int>(state_.size());
    }

private:
    std::vector<std::vector<int>> state_;

    DISALLOW_ASSIGN_AND_COPY(Board);
};

#endif  // BOARD_H_

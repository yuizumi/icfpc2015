#include "board.h"

#include <algorithm>
#include <cmath>

using namespace std;
using std::placeholders::_1;

namespace {

const double kSqrt3 = 1.7320508075688772;

Cell CellMoveE(const Cell& cell) {
    return Cell { cell.x + 1, cell.y };
}
Cell CellMoveW(const Cell& cell) {
    return Cell { cell.x - 1, cell.y };
}
Cell CellMoveSE(const Cell& cell) {
    return Cell { cell.x + 1, cell.y + 1 };
}
Cell CellMoveSW(const Cell& cell) {
    return Cell { cell.x - 1, cell.y + 1 };
}

}  // namespace

Unit::Unit(const UnitSpec& spec, Board* board)
    : board_(board), pivot_(spec.pivot()), cells_(spec.cells()) {
}

bool Unit::CanMoveE() const {
    return TestCells(CellMoveE);
}

void Unit::MoveE() {
    MoveCells(CellMoveE);
}

bool Unit::CanMoveW() const {
    return TestCells(CellMoveW);
}

void Unit::MoveW() {
    MoveCells(CellMoveW);
}

bool Unit::CanMoveSE() const {
    return TestCells(CellMoveSE);
}

void Unit::MoveSE() {
    MoveCells(CellMoveSE);
}

bool Unit::CanMoveSW() const {
    return TestCells(CellMoveSW);
}

void Unit::MoveSW() {
    MoveCells(CellMoveSW);
}

bool Unit::CanRotate(Direction d) const {
    return TestCells(bind(&Unit::RotateCell, this, _1, d));
}

void Unit::Rotate(Direction d) {
    MoveCells(bind(&Unit::RotateCell, this, _1, d));
}

void Unit::Lock() {
    for (const Cell& cell : cells_) {
        board_->set(cell.x, cell.y, true);
    }
    board_->ClearRows();
}

void Unit::Invoke(Command command) {
    switch (command) {
    case kNone:
        break;
    case kMoveE:
        MoveE(); break;
    case kMoveW:
        MoveW(); break;
    case kMoveSE:
        MoveSE(); break;
    case kMoveSW:
        MoveSW(); break;
    case kRotateRight:
        Rotate(kRight); break;
    case kRotateLeft:
        Rotate(kLeft); break;
    }
}

Cell Unit::RotateCell(const Cell& cell, Direction d) const {
    int x0 = cell.x - pivot_.x;
    int y0 = cell.y - pivot_.y;
    double ex0 = x0 * 2.0 + static_cast<double>(cell.y % 2);
    double ey0 = y0 * kSqrt3;
    double ex1 = (ex0 * kSqrt3 - ey0 * static_cast<double>(d)) / 2.0;
    double ey1 = (ex0 * static_cast<double>(d) + ey0 * kSqrt3) / 2.0;
    int x1 = static_cast<int>(round(ex1)) / 2;
    int y1 = static_cast<int>(round(ey1 / kSqrt3));
    return Cell { x1 + pivot_.x, y1 + pivot_.y };
}

bool Unit::TestCells(function<Cell(const Cell&)> mover) const {
    return all_of(cells_.begin(), cells_.end(), [=](const Cell& cell) {
        return !board_->get(mover(cell));
    });
}

void Unit::MoveCells(function<Cell(const Cell&)> mover) {
    for (size_t i = 0; i < cells_.size(); i++) cells_[i] = mover(cells_[i]);
}

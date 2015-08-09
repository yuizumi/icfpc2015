#include "unit.h"

#include <algorithm>
#include <cmath>
#include <iostream>

using std::placeholders::_1;

using namespace std;

namespace {
const double kSqrt3 = 1.7320508075688772;

Cell CellMoveE(const Cell& cell) {
    return Cell { cell.x + 1, cell.y };
}
Cell CellMoveW(const Cell& cell) {
    return Cell { cell.x - 1, cell.y };
}
Cell CellMoveSE(const Cell& cell) {
    int x = (cell.y % 2 == 0) ? cell.x : cell.x + 1;
    return Cell { x, cell.y + 1 };
}
Cell CellMoveSW(const Cell& cell) {
    int x = (cell.y % 2 == 0) ? cell.x - 1 : cell.x;
    return Cell { x, cell.y + 1 };
}
}  // namespace

UnitSpec::UnitSpec(const vector<Cell>& members, const Cell& pivot)
    : members_(members), pivot_(pivot) {
}

Unit::Unit(const UnitSpec& spec, const Board& board)
    : members_(spec.members()),
      pivot_(spec.pivot()),
      board_(board) {
    // TODO(yuizumi): Avoid recomputation.
    int min_x = members_[0].x;
    int max_x = members_[0].x;
    for (const Cell& member : members_) {
        min_x = min(min_x, member.x);
        max_x = max(max_x, member.x);
    }
    int x_move = (board.width() - (max_x - min_x + 1)) / 2 - min_x;
    for (Cell& member : members_) {
        member.x += x_move;
    }
    pivot_.x += x_move;
}

Unit::Unit(const Unit& unit, const Board& board)
    : members_(unit.members_),
      pivot_(unit.pivot_),
      board_(board) {
}

unique_ptr<Unit> Unit::Clone(const Board& board) const {
    return unique_ptr<Unit>(new Unit(*this, board));
}

uint32_t Unit::Hash() const {
    const uint32_t x_multi = 1722715777;
    const uint32_t y_multi =  208049593;
    uint32_t hash = 0;
    for (const Cell& member : members_) {
        hash ^= member.x * x_multi + member.y * y_multi;
    }
    return hash;  // Pivot not considered.
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

bool Unit::CanRotateRight() const {
    return TestCells(bind(&Unit::RotateCell, this, _1, -1));
}

void Unit::RotateRight() {
    MoveCells(bind(&Unit::RotateCell, this, _1, -1));
}

bool Unit::CanRotateLeft() const {
    return TestCells(bind(&Unit::RotateCell, this, _1, +1));
}

void Unit::RotateLeft() {
    MoveCells(bind(&Unit::RotateCell, this, _1, +1));
}

bool Unit::CanInvoke(Command command) const {
    switch (command) {
    case kNone:
        return true;
    case kMoveE:
        return CanMoveE();
    case kMoveW:
        return CanMoveW();
    case kMoveSE:
        return CanMoveSE();
    case kMoveSW:
        return CanMoveSW();
    case kRotateRight:
        return CanRotateRight();
    case kRotateLeft:
        return CanRotateLeft();
    default:
        return false;
    }
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
        RotateRight(); break;
    case kRotateLeft:
        RotateLeft(); break;
    }
}

Cell Unit::RotateCell(const Cell& cell, int direction) const {
    int dx = cell.x - pivot_.x;
    int dy = cell.y - pivot_.y;
    double ex0 = dx * 2.0 + (cell.y % 2 - pivot_.y % 2);
    double ey0 = dy * kSqrt3;
    double ex1 = (ex0 - ey0 * kSqrt3 * static_cast<double>(direction)) / 2.0;
    double ey1 = (ex0 * kSqrt3 * static_cast<double>(direction) + ey0) / 2.0;
    Cell new_cell = pivot_;
    new_cell.y += static_cast<int>(round(ey1 / kSqrt3));
    new_cell.x +=
        (static_cast<int>(round(ex1)) - (new_cell.y % 2 - pivot_.y % 2)) / 2;
    return new_cell;
}

bool Unit::TestCells(CellMover mover) const {
    return all_of(members_.begin(), members_.end(), [=](const Cell& cell) {
        return !board_.get(mover(cell));
    });
}

void Unit::MoveCells(CellMover mover) {
    for (Cell& member : members_) member = mover(member);  // members_ mutated.
    pivot_ = mover(pivot_);
}

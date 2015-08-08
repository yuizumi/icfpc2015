#include "unit.h"

#include <algorithm>
#include <cmath>

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

Unit::Unit(const UnitSpec& spec, const Board& board)
    : members_(spec.members()),
      pivot_(spec.pivot()),
      board_(board) {
    // TODO(yuizumi): Support the case where (min_y != 0).
    // TODO(yuizumi): Avoid recalculation.
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

Unit::Unit(const Unit& unit) : members_(unit.members_),
                               pivot_(unit.pivot_),
                               board_(unit.board_) {
}

unique_ptr<Unit> Unit::Clone() const {
    return unique_ptr<Unit>(new Unit(*this));
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
    return TestCells(bind(&Unit::RotateCell, this, _1, +1));
}

void Unit::RotateRight() {
    MoveCells(bind(&Unit::RotateCell, this, _1, +1));
}

bool Unit::CanRotateLeft() const {
    return TestCells(bind(&Unit::RotateCell, this, _1, -1));
}

void Unit::RotateLeft() {
    MoveCells(bind(&Unit::RotateCell, this, _1, -1));
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
    int x0 = cell.x - pivot_.x;
    int y0 = cell.y - pivot_.y;
    double ex0 = x0 * 2.0 + static_cast<double>(cell.y % 2);
    double ey0 = y0 * kSqrt3;
    double ex1 = (ex0 * kSqrt3 - ey0 * static_cast<double>(direction)) / 2.0;
    double ey1 = (ex0 * static_cast<double>(direction) + ey0 * kSqrt3) / 2.0;
    int x1 = static_cast<int>(round(ex1)) / 2;
    int y1 = static_cast<int>(round(ey1 / kSqrt3));
    return Cell { x1 + pivot_.x, y1 + pivot_.y };
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
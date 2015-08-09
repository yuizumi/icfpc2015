#ifndef UNIT_H_
#define UNIT_H_

#include <functional>
#include <vector>
#include "basic.h"
#include "board.h"
#include "macros.h"

class UnitSpec {
public:
    UnitSpec(const std::vector<Cell>& members, const Cell& pivot);
    UnitSpec(UnitSpec&&) = default;

    const std::vector<Cell>& members() const { return members_; }
    const Cell& pivot() const { return pivot_; }

private:
    const std::vector<Cell> members_;
    const Cell pivot_;

    DISALLOW_COPY_AND_ASSIGN(UnitSpec);
};

class Unit {
public:
    Unit(const UnitSpec& spec, const Board& board);

    std::unique_ptr<Unit> Clone() const;
    uint32_t Hash() const;

    bool CanMoveE() const;
    void MoveE();
    bool CanMoveW() const;
    void MoveW();
    bool CanMoveSE() const;
    void MoveSE();
    bool CanMoveSW() const;
    void MoveSW();
    bool CanRotateRight() const;
    void RotateRight();
    bool CanRotateLeft() const;
    void RotateLeft();

    bool CanInvoke(Command command) const;
    void Invoke(Command command);

    const std::vector<Cell>& members() const { return members_; }
    Cell pivot() const { return pivot_; }

private:
    using CellMover = std::function<Cell(const Cell&)>;

    std::vector<Cell> members_;
    Cell pivot_;
    const Board& board_;

    Cell RotateCell(const Cell& cell, int direction) const;
    bool TestCells(CellMover mover) const;
    void MoveCells(CellMover mover);

    DISALLOW_COPY_AND_ASSIGN(Unit);
};

#endif  // UNIT_H_

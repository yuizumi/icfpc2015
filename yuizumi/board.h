#include <functional>
#include <vector>

enum Command {
    kNone,
    kMoveE,
    kMoveW,
    kMoveSE,
    kMoveSW,
    kRotateRight,
    kRotateLeft,
};

enum Direction { kRight = +1, kLeft = -1 };

struct Cell { int x, y; };

class Board {
public:
    Board(int w, int h);

    bool get(const Cell& cell) const { return get(cell.x, cell.y); }
    bool get(int x, int y) const { return states_[y][x]; }

    void set(const Cell& cell, bool value) { set(cell.x, cell.y, value); }
    void set(int x, int y, bool value) { states_[y][x] = value; }

    void ClearRows();

private:
    int w_, h_;
    std::vector<std::vector<int>> states_;
};

class UnitSpec {
public:
    UnitSpec(const Cell& pivot, const std::vector<Cell>& cells)
        : pivot_(pivot), cells_(cells) {
    }

    const Cell& pivot() const { return pivot_; }
    const std::vector<Cell>& cells() const { return cells_; }

private:
    const Cell pivot_;
    const std::vector<Cell> cells_;
};

class Unit {
public:
    Unit(const UnitSpec& spec, Board* board);

    bool CanMoveE() const;
    void MoveE();
    bool CanMoveW() const;
    void MoveW();
    bool CanMoveSE() const;
    void MoveSE();
    bool CanMoveSW() const;
    void MoveSW();

    bool CanRotate(Direction d) const;
    void Rotate(Direction d);

    void Lock();

    void Invoke(Command command);

private:
    bool TestCells(std::function<Cell(const Cell&)> mover) const;
    void MoveCells(std::function<Cell(const Cell&)> mover);

    Cell RotateCell(const Cell& cell, Direction d) const;

    Board* board_;
    Cell pivot_;
    std::vector<Cell> cells_;
};

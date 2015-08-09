#include <vector>

enum Command {
    kMoveE,
    kMoveW,
    kMoveSE,
    kMoveSW,
    kRotateRight,
    kRotateLeft,
};

enum Direction {
    kRight = +1,
    kLeft = -1,
};

struct Cell {
    int x, y;
};

class Board {
public:
    bool get(const Cell& cell) const;
    bool get(int x, int y) const;

    void set(const Cell& cell, bool value);
    void set(int x, int y, bool value);

private:
    std::vector<std::vector<int>> states_;
};

class UnitSpec {
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
    Cell RotateCell(const Cell& cell, Direction d) const;

    Board* board_;
    Cell pivot_;
    std::vector<Cell> cells_;
};

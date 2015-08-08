#ifndef BASIC_H_
#define BASIC_H_

struct Cell { int x, y; };

inline bool operator==(const Cell& a, const Cell& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Cell& a, const Cell& b) {
    return !(a.x == b.x);
}

enum Command {
    kNone,
    kMoveE,
    kMoveW,
    kMoveSE,
    kMoveSW,
    kRotateRight,
    kRotateLeft,
};

#endif  // BASIC_H_

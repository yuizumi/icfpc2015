#ifndef BASIC_H_
#define BASIC_H_

struct Cell { int x, y; };

enum Command {
    kNone,
    kMoveE,
    kMoveW,
    kMoveSE,
    kMoveSW,
    kRotateRight,
    kRotateLeft,
};

inline bool operator==(const Cell& a, const Cell& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Cell& a, const Cell& b) {
    return !(a.x == b.x);
}

Command CharToCommand(char c);
char CommandToChar(Command c);

#endif  // BASIC_H_

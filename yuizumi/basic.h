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

#endif  // BASIC_H_

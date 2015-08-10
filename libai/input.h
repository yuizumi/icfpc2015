#ifndef INPUT_H_
#define INPUT_H_

#include <istream>
#include <vector>
#include "board.h"
#include "unit.h"

struct GameSpec {
    int id, length;
    std::vector <UnitSpec> units;
    std::unique_ptr<Board> board;
    std::vector<int> seeds;
};

GameSpec ReadGameSpec(std::istream& in);

#endif  // INPUT_H_

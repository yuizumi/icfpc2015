#ifndef GAME_H_
#define GAME_H_

#include <vector>
#include "basic.h"
#include "board.h"
#include "macros.h"
#include "unit.h"

class GameState {
public:
    GameState(const std::vector<UnitSpec>& units,
              std::unique_ptr<Board> board,
              int seed, int length);

    const Board& board() const {
        return *board_;
    }

    const Unit& unit() const {
        return *unit_;
    }

    std::unique_ptr<GameState> Clone() const;
    void Invoke(Command command);

private:
    const std::vector<UnitSpec>& units_;
    const std::unique_ptr<Board> board_;
    int seed_, rest_;
    std::unique_ptr<Unit> unit_;

    DISALLOW_COPY_AND_ASSIGN(GameState);
};

#endif  // GAME_H_

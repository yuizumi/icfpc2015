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
              uint32_t seed, int length);

    std::unique_ptr<GameState> Clone() const;
    void Invoke(Command command);

    const Board& board() const {
        return *board_;
    }

    const Unit& unit() const {
        assert(unit_);
        return *unit_;
    }

    bool gameover() const {
        return gameover_;
    }

    int rest() const {
        return rest_;
    }

    const std::vector<Command>& commands() const {
        return commands_;
    }

private:
    GameState(const GameState* state);

    void UpdateUnit();

    const std::vector<UnitSpec>& units_;
    const std::unique_ptr<Board> board_;
    uint32_t seed_;
    int rest_;
    bool gameover_;
    std::vector<Command> commands_;
    std::unique_ptr<Unit> unit_;

    DISALLOW_COPY_AND_ASSIGN(GameState);
};

void Solve(GameState* state);  // Entry point for AI.

#endif  // GAME_H_

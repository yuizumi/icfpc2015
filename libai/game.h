#ifndef GAME_H_
#define GAME_H_

#include <string>
#include <unordered_set>
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
    void Swap(GameState* other);

    bool CanMove(Command command) const;
    bool IsValid(Command command) const;
    void Invoke(char command);

    bool CanMove(char command) const {
        return CanMove(CharToCommand(command));
    }

    bool IsValid(char command) const {
        return IsValid(CharToCommand(command));
    }

    void Invoke(Command command) {
        Invoke(CommandToChar(command));
    }

    std::vector<const UnitSpec*> GetFutureUnits() const;

    int move_score() const {
        return move_score_;
    }

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

    const std::string& commands() const {
        return commands_;
    }

private:
    GameState(const GameState* state);

    void UpdateUnit();

    const std::vector<UnitSpec>& units_;
    std::unique_ptr<Board> board_;
    uint32_t seed_;
    int rest_;
    bool gameover_;
    std::string commands_;
    std::unique_ptr<Unit> unit_;
    std::unordered_set<uint32_t> banned_;
    int move_score_;
    int lines_old_;

    DISALLOW_COPY_AND_ASSIGN(GameState);
};

void Solve(GameState* state);  // Entry point for AI.

#endif  // GAME_H_

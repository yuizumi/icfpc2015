#ifndef GAME_H_
#define GAME_H_

#include <istream>
#include <vector>
#include "basic.h"
#include "board.h"
#include "macros.h"
#include "unit.h"

class GameState;

class GameSpec {
public:
    static std::unique_ptr<GameSpec> Read(std::istream& in);
    std::unique_ptr<GameState> GetNextGame();

private:
    friend class GameState;

    GameSpec(int id, const Board& board,
             const std::vector<UnitSpec>& units,
             const std::vector<int>& seeds, int source_length);

    const int id_;
    const std::unique_ptr<Board> board_;
    const std::vector <UnitSpec> units_;
    const std::vector<int> seeds_;
    const int source_length_;

    DISALLOW_COPY_AND_ASSIGN(GameSpec);
};

class GameState {
public:
    GameState(GameSpec& spec, int seed);

    const Board& board() const {
        return *board_;
    }

    const Unit& unit() const {
        return *unit_;
    }

    std::unique_ptr<GameState> Clone() const;
    void Invoke(Command command);

private:
    const int id_;
    const std::unique_ptr<Board> board_;
    const std::vector<UnitSpec>& units_;
    int seed_, rest_;
    std::unique_ptr<Unit> unit_;

    DISALLOW_COPY_AND_ASSIGN(GameState);
};

#endif  // GAME_H_

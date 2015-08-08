#ifndef GAME_H_
#define GAME_H_

#include <istream>
#include <vector>
#include "basic.h"
#include "board.h"
#include "macros.h"
#include "unit.h"

class GameSpec {
public:
    static std::unique_ptr<GameSpec> Read(std::istream& in);
    std::unique_ptr<Game> GetNextGame();

private:
    friend class Game;

    GameSpec(int id, const Board& board,
             const std::vector<UnitSpec>& units,
             const std::vector<int>& seeds, int source_length);

    const int id_;
    const std::unique_ptr<Board> board_;
    const std::vector <UnitSpec> units_;
    const std::vector<int> seeds_;
    const int source_length_;

    DISALLOW_ASSIGN_AND_COPY(GameSpec);
};

class Game {
public:
    Game(GameSpec& spec, int seed);

    const Board& board() const {
        return *board_;
    }

    const Unit& unit() const {
        return *unit_;
    }

    Invoke(Command command);

private:
    const int id_;
    const std::unique_ptr<Board> board_;
    const std::vector<UnitSpec>& units_;
    unique_ptr<Unit> unit_;
    int seed_, rest_;

    DISALLOW_ASSIGN_AND_COPY(GameSpec);
};

#endif  // GAME_H_

#include "game.h"

#include <cstdio>
#include <vector>

using namespace std;

GameState::GameState(const std::vector<UnitSpec>& units,
                     std::unique_ptr<Board> board,
                     int seed, int length)
    : units_(units), board_(board.release()), seed_(seed), rest_(length) {
}

unique_ptr<GameState> GameState::Clone() const {
    unique_ptr<GameState> clone(
        new GameState(units_, board_->Clone(), seed_, rest_));
    clone->unit_ = unit_->Clone();
    return clone;
}

void GameState::Invoke(Command command) {
    assert(!unit_);
    if (unit_->CanInvoke(command)) {
        unit_->Invoke(command);
    } else {
        board_->Lock(*unit_);
    }
}

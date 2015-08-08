#include "game.h"

#include <cstdio>
#include <vector>

using namespace std;

GameState::GameState(const vector<UnitSpec>& units,
                     unique_ptr<Board> board,
                     uint32_t seed,
                     int length)
    : units_(units), board_(board.release()), seed_(seed), rest_(length) {
    SetNextUnit();
}

GameState::GameState(const vector<UnitSpec>& units,
                     unique_ptr<Board> board,
                     uint32_t seed, int length,
                     unique_ptr<Unit> unit,
                     const vector<Command>& commands)
    : units_(units), board_(board.release()), seed_(seed), rest_(length),
      unit_(unit.release()), commands_(commands) {
}

unique_ptr<GameState> GameState::Clone() const {
    return unique_ptr<GameState>(new GameState(
        units_, board_->Clone(), seed_, rest_, unit_->Clone(), commands_));
}

void GameState::Invoke(Command command) {
    assert(unit_);

    if (unit_->CanInvoke(command)) {
        unit_->Invoke(command);
    } else {
        board_->Lock(*unit_);
        SetNextUnit();
    }
    commands_.push_back(command);
}

void GameState::SetNextUnit() {
    if (rest_ == 0) {
        unit_.reset();
    } else {
        seed_ = seed_ * 1103515245 + 12345;
        int value = (seed_ >> 16) & 0x7FFF;
        --rest_;
        unit_.reset(new Unit(units_[value % units_.size()], *board_));
    }
}

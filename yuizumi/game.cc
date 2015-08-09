#include "game.h"

#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

namespace {
bool HasConflicts(const Unit& unit, const Board& board) {
    return any_of(unit.members().begin(), unit.members().end(),
                  [&board](const Cell& cell) { return board.get(cell); });
}
}  // namespace

GameState::GameState(const vector<UnitSpec>& units,
                     unique_ptr<Board> board,
                     uint32_t seed, int length)
    : units_(units), board_(board.release()), seed_(seed), rest_(length) {
    UpdateUnit();
}

GameState::GameState(const GameState* state)
    : units_(state->units_),
      board_(state->board_->Clone()),
      seed_(state->seed_),
      rest_(state->rest_),
      gameover_(state->gameover_),
      commands_(state->commands_),
      unit_(state->unit_->Clone()),
      banned_(state->banned_) {
}

unique_ptr<GameState> GameState::Clone() const {
    return unique_ptr<GameState>(new GameState(this));
}

void GameState::Swap(GameState* other) {
    assert(&units_ == &other->units_);
    swap(board_, other->board_);
    swap(seed_, other->seed_);
    swap(rest_, other->rest_);
    swap(gameover_, other->gameover_);
    swap(commands_, other->commands_);
    swap(unit_, other->unit_);
}

bool GameState::CanInvoke(Command command) const {
    if (!unit_->CanInvoke(command)) {
        return false;
    }
    unique_ptr<Unit> clone(unit_->Clone());
    clone->Invoke(command);
    return banned_.count(unit_->Hash());
}

void GameState::Invoke(Command command) {
    assert(!gameover_);

    if (unit_->CanInvoke(command)) {
        unit_->Invoke(command);
        const auto result = banned_.insert(unit_->Hash());
        assert(result.second);
    } else {
        board_->Lock(*unit_);
        UpdateUnit();
    }
    commands_.push_back(command);
}

void GameState::UpdateUnit() {
    if (--rest_ == 0) {
        unit_.reset();
        gameover_ = true;
    } else {
        seed_ = seed_ * 1103515245 + 12345;
        int value = (seed_ >> 16) & 0x7FFF;
        unit_.reset(new Unit(units_[value % units_.size()], *board_));
        gameover_ = HasConflicts(*unit_, *board_);
        banned_.clear();
        banned_.insert(unit_->Hash());
    }
}

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
    : units_(units), board_(board.release()), seed_(seed), rest_(length),
      move_score_(0), lines_old_(0) {
    UpdateUnit();
}

GameState::GameState(const GameState* state)
    : units_(state->units_),
      board_(state->board_->Clone()),
      seed_(state->seed_),
      rest_(state->rest_),
      gameover_(state->gameover_),
      commands_(state->commands_),
      unit_(state->unit_->Clone(*board_)),
      banned_(state->banned_),
      move_score_(state->move_score_),
      lines_old_(state->lines_old_) {
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
    swap(banned_, other->banned_);
    swap(move_score_, other->move_score_);
    swap(lines_old_, other->lines_old_);
}

bool GameState::CanMove(Command command) const {
    return unit().CanInvoke(command);
}

bool GameState::IsValid(Command command) const {
    if (command == kNone) {
        return true;
    }
    if (gameover_) {
        return false;
    } else {
        unique_ptr<Unit> clone(unit_->Clone(*board_));
        clone->Invoke(command);
        return !banned_.count(clone->Hash());
    }
}

void GameState::Invoke(char command) {
    assert(!gameover_);

    Command command_enum = CharToCommand(command);
    if (command_enum == kNone) {
        return;
    }
    if (unit_->CanInvoke(command_enum)) {
        unit_->Invoke(command_enum);
        const auto result = banned_.insert(unit_->Hash());
        assert(result.second);
    } else {
        int lines = board_->Lock(*unit_);
        int points = unit_->members().size() + 100 * (1 + lines) * lines / 2;
        if (lines_old_ > 0) {
            points += (lines_old_ - 1) * points / 10;
        }
        move_score_ += points;
        lines_old_ = lines;
        UpdateUnit();
    }
    commands_.push_back(command);
}

void GameState::UpdateUnit() {
    if (--rest_ == 0) {
        unit_.reset();
        gameover_ = true;
    } else {
        int value = (seed_ >> 16) & 0x7FFF;
        seed_ = seed_ * 1103515245 + 12345;
        unit_.reset(new Unit(units_[value % units_.size()], *board_));
        gameover_ = HasConflicts(*unit_, *board_);
        banned_.clear();
        banned_.insert(unit_->Hash());
    }
}

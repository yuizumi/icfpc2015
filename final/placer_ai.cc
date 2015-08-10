#include "debug.h"
#include "game.h"

using namespace std;

namespace {
inline bool CanMove(const GameState& state, Command command) {
    return state.CanMove(command) && state.IsValid(command);
}

inline Command Choose(const GameState& state,
                      Command c1, Command c2, Command c3) {
    if (state.CanMove(c1) && state.IsValid(c1))
        return c1;
    if (state.CanMove(c2) && state.IsValid(c2))
        return c2;
    if (state.CanMove(c3) && state.IsValid(c3))
        return c3;
    return kNone;
}

void Drop(GameState* state, int x, int r) {
    while (true) {
        const Cell& p = state->unit().pivot();
        if (r < 0 && CanMove(*state, kRotateLeft)) {
            ++r; state->Invoke(kRotateLeft);
            continue;
        }
        if (r > 0 && CanMove(*state, kRotateRight)) {
            --r; state->Invoke(kRotateRight);
            continue;
        }
        int wx = (p.y % 2 == 0) ? p.x - 1 : p.x;
        int ex = (p.y % 2 == 0) ? p.x : p.x + 1;
        Command command;
        if (abs(wx - x) < abs(ex - x)) {
            command = Choose(*state, kMoveSW, kMoveSE, kMoveW);
        } else {
            command = Choose(*state, kMoveSE, kMoveSW, kMoveE);
        }
        if (p.x == x && (command == kMoveE || command == kMoveW))
            break;
        if (command == kNone)
            break;
        state->Invoke(command);
    }
}

int Evaluate(const GameState& state) {
    const Board& board = state.board();
    int score = 0;
    int width = board.width();
    for (int y = 1; y < board.height(); y++) {
        for (int x = 0; x < width; x++) {
            if (board.get(x, y))
                continue;
            if (board.get(x, y - 1))
                --score;
            int xx = (y % 2 == 0) ? x - 1 : x + 1;
            if (board.get(xx, y - 1))
                --score;
        }
    }
    return score;
}
}  // namespace

void Solve(GameState* state, const vector<string> &power_phrases) {
    int width = state->board().width();
    while (!state->gameover()) {
        debug::Print(*state);
        unique_ptr<GameState> best_state(state->Clone());
        int best_score = INT_MIN;
        for (int x = 0; x < width; x++) {
            for (int r = -3; r <= 3; r++) {
                unique_ptr<GameState> new_state(state->Clone());
                Drop(new_state.get(), x, r);
                Cell pivot = new_state->unit().pivot();
                if (new_state->IsValid(kMoveSW))
                    new_state->Invoke(kMoveSW);
                else if (new_state->IsValid(kMoveSE))
                    new_state->Invoke(kMoveSE);
                int score = Evaluate(*new_state) + pivot.y * 4 - abs(r);
                if (best_score < score) {
                    best_score = score;
                    best_state.reset(new_state.release());
                }
            }
        }
        state->Swap(best_state.get());
    }
}

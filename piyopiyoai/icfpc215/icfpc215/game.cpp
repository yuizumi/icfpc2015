#include "game.h"
#include <thread>
#include <chrono>
using namespace std;

Game::Game(GameState* state) {
    _width = state->board().width();
    _height = state->board().height();
}

int Game::UnitTop(GameState *state) const {
    int topY = INT_MAX;
    for(auto cell:state->unit().members()) {
        topY = min(topY, cell.y);
    }
    return topY;
}

int Game::LongestWidth(GameState *state) {
    vector<int> ys(_height);
    
    for(auto cell:state->unit().members()) {
        ys[cell.y]++;
    }
    return *max_element(ys.begin(), ys.end());
}

Command Game::Stretch(GameState *state) {
    int current = LongestWidth(state);

    int longest = 0;
    Command longestCommand = kNone;
    Command commands[2] = {kRotateLeft, kRotateRight};
    
    for (int i = 0; i < 2; i++) {
        unique_ptr<GameState> clone_state(state->Clone());
        if(clone_state->IsValid(commands[i]) &&
           clone_state->CanMove(commands[i]) ) {
            clone_state->Invoke(commands[i]);
            int l = LongestWidth(clone_state.get());
            if (l >= longest) {
                longest = l;
                longestCommand = commands[i];
            }
            
        }
    }
    if (longest > current) {
        return longestCommand;
    }
    return kNone;
}

Command Game::Recommend(GameState* state) {
    Command r = Stretch(state);
    if (r != kNone) {
        state->Invoke(r);
    }
    
    if (state->CanMove(kMoveSW) && state->IsValid(kMoveSW)) {
        return kMoveSW;
    } else if (state->CanMove(kMoveSE) && state->IsValid(kMoveSE)) {
        return kMoveSE;
    } else if (state->CanMove(kMoveE) && state->IsValid(kMoveE)) {
        return kMoveE;
    } else if (state->CanMove(kMoveW) && state->IsValid(kMoveW)) {
        return kMoveW;
    } else {
        if (state -> IsValid(prev)) {
            return prev;
        }
        return kNone;
    }
}


void Game::debugMove(GameState *state, Command command) {
    
}

bool Game::NextTurn(GameState *state) {
    Command c = Recommend(state);
    if (c == kNone) return false;
    prev = c;
    state->Invoke(Recommend(state));
    this_thread::sleep_for(std::chrono::milliseconds(100));
    debug::Print(*state);
    return true;
}

void Game::power(GameState *state) {
    
}
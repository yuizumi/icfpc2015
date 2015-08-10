#include "game.h"

Game::Game(GameState* state) {
    _width = state->board().width();
    _height = state->board().height();
}

void Game::NextTurn(GameState *state) {
//    unique_ptr<GameState> clone_state(state->Clone());
    state->Invoke(kMoveSW);
}
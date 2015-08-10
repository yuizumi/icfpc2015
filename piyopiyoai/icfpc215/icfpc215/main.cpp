#include <cstdlib>
#include <ctime>
#include <iostream>
#include "interface/debug.h"
#include "interface/game.h"
#include "game.h"

using namespace std;

void Solve(GameState* state) {
    Game game = Game(state);
    int turn = 0;
    while (!state->gameover()) {
        cout << "turn: " << turn++ << endl;
        debug::Print(*state);
        game.NextTurn(state);
    }
    
    return;
}

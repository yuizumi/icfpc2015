#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
#include "interface/debug.h"
#include "interface/game.h"
#include "game.h"

using namespace std;
void Solve(GameState* state) {
    Game game = Game(state);
    int turn = 0;
    while (!state->gameover()) {
        cout << "turn: " << turn++ << endl;
        bool b = game.NextTurn(state);
        if (!b) {
            return;
        }

    }

    return;
}

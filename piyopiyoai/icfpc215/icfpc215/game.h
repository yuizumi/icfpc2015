
#ifndef __icfpc215__game__
#define __icfpc215__game__

#include <stdio.h>

#include "interface/game.h"
#include "interface/debug.h"
#include "interface/board.h"

class Game {
public:
    
    Game(GameState *state);
    void NextTurn(GameState *state);
private:
    int _height;
    int _width;
};


#endif

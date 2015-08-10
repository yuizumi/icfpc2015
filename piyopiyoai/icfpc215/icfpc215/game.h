
#ifndef __icfpc215__game__
#define __icfpc215__game__

#include <stdio.h>
#include <iostream>
#include "interface/game.h"
#include "interface/debug.h"
#include "interface/board.h"

using namespace std;

const int powerLength = 10;

class Game {
public:
    Game(GameState *state);
    bool NextTurn(GameState *state);
private:
    int _height;
    int _width;
    Command prev = kNone;
    string powers[powerLength] = {"142", "4423442", "523114", "1644364", "6646434464", "311533334133", "164634631464432", "242343643343623414316463463523114364442344331464432", "433446343661346352311435145316463463644663551434342", "234316352"};

    void power(GameState *state);
    void debugMove(GameState *state, Command command);
    int UnitTop(GameState *state) const;
    Command Recommend(GameState *state);
    Command Stretch(GameState *state);
    int LongestWidth(GameState *state);
};


#endif

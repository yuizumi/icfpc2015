#include "debug.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace debug {

namespace {

void PrintBoard(const Board& board, const Unit& unit) {
    for (int y = 0; y < board.height(); y++) {
        if (y % 2 != 0) cerr << ' ';
        for (int x = 0; x < board.width(); x++) {
            if (board.get(x, y)) {
                cerr << "x "; continue;
            }
            Cell cell { x, y };
            if (unit.pivot() == cell) {
                cerr << "* "; continue;
            }
            const vector<Cell>& members = unit.members();
            if (find(members.begin(), members.end(), cell) != members.end()) {
                cerr << "o "; continue;
            }
            cerr << ". ";
        }
        cerr << endl;
    }
}

void PrintCommands(const vector<Command>& commands) {
    const map<Command, string> short_names = {
        {kNone, "-"},
        {kMoveE, "E"},
        {kMoveW, "W"},
        {kMoveSE, "SE"},
        {kMoveSW, "SW"},
        {kRotateRight, "Rcc"},
        {kRotateLeft, "Rc"},
    };

    for (Command command : commands) {
        cerr << short_names.find(command)->second << ' ';
    }
    cerr << endl;
}

}  // namespace

void Print(const GameState& state) {
    cerr << "========" << endl;
    PrintCommands(state.commands());
    cerr << "Units left: " << state.rest() << endl;
    PrintBoard(state.board(), state.unit());
}

}  // namespace debug


#include "basic.h"

#include <map>

using namespace std;

Command CharToCommand(char c) {
    const map<char, Command> chr2cmd = {
        {'p', kMoveW}, {'\'', kMoveW}, {'!', kMoveW},
        {'.', kMoveW}, {'0', kMoveW}, {'3', kMoveW},
        {'b', kMoveE}, {'c', kMoveE}, {'e', kMoveE},
        {'f', kMoveE}, {'y', kMoveE}, {'2', kMoveE},
        {'a', kMoveSW}, {'g', kMoveSW}, {'h', kMoveSW},
        {'i', kMoveSW}, {'j', kMoveSW}, {'4', kMoveSW},
        {'l', kMoveSE}, {'m', kMoveSE}, {'n', kMoveSE},
        {'o', kMoveSE}, {' ', kMoveSE}, {'5', kMoveSE},
        {'d', kRotateRight}, {'q', kRotateRight}, {'r', kRotateRight},
        {'v', kRotateRight}, {'z', kRotateRight}, {'1', kRotateRight},
        {'k', kRotateLeft}, {'s', kRotateLeft}, {'t', kRotateLeft},
        {'u', kRotateLeft}, {'w', kRotateLeft}, {'x', kRotateLeft},
        {'\t', kNone}, {'\n', kNone}, {'\r', kNone},
    };
    return chr2cmd.find(c)->second;
}

char CommandToChar(Command c) {
    const map<Command, char> cmd2chr = {
        {kNone, '\t'},
        {kMoveE, 'b'},
        {kMoveW, 'p'},
        {kMoveSE, 'l'},
        {kMoveSW, 'a'},
        {kRotateRight, 'd'},
        {kRotateLeft, 'k'},
    };
    return cmd2chr.find(c)->second;
}

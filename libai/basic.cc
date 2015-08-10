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
        {'d', kRotateLeft}, {'q', kRotateLeft}, {'r', kRotateLeft},
        {'v', kRotateLeft}, {'z', kRotateLeft}, {'1', kRotateLeft},
        {'k', kRotateRight}, {'s', kRotateRight}, {'t', kRotateRight},
        {'u', kRotateRight}, {'w', kRotateRight}, {'x', kRotateRight},
        {'\t', kNone}, {'\n', kNone}, {'\r', kNone},
    };
    return chr2cmd.find(c)->second;
}

char CommandToChar(Command c) {
    const map<Command, char> cmd2chr = {
        {kNone, '\r'},
        {kMoveE, '2'},
        {kMoveW, '3'},
        {kMoveSE, '5'},
        {kMoveSW, '4'},
        {kRotateLeft, '1'},
        {kRotateRight, 'x'},
    };
    return cmd2chr.find(c)->second;
}

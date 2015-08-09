#include "./util.h"

std::map<char, Command> initCharToCommand() {
  std::map<char, Command> m;
  m['p'] = kMoveW;
  m['\''] = kMoveW;
  m['!'] = kMoveW;
  m['.'] = kMoveW;
  m['0'] = kMoveW;
  m['3'] = kMoveW;
    
  m['b'] = kMoveE;
  m['c'] = kMoveE;
  m['e'] = kMoveE;
  m['f'] = kMoveE;
  m['y'] = kMoveE;
  m['2'] = kMoveE;
    
  m['a'] = kMoveSW;
  m['g'] = kMoveSW;
  m['h'] = kMoveSW;
  m['i'] = kMoveSW;
  m['j'] = kMoveSW;
  m['4'] = kMoveSW;
    
  m['l'] = kMoveSE;
  m['m'] = kMoveSE;
  m['n'] = kMoveSE;
  m['o'] = kMoveSE;
  m[' '] = kMoveSE;
  m['5'] = kMoveSE;
    
  m['d'] = kRotateRight;
  m['q'] = kRotateRight;
  m['r'] = kRotateRight;
  m['v'] = kRotateRight;
  m['z'] = kRotateRight;
  m['1'] = kRotateRight;
    
  m['k'] = kRotateLeft;
  m['s'] = kRotateLeft;
  m['t'] = kRotateLeft;
  m['u'] = kRotateLeft;
  m['w'] = kRotateLeft;
  m['x'] = kRotateLeft;
    
  m['\t'] = kNone;
  m['\n'] = kNone;
  m['\r'] = kNone;

  return m;
};

const std::map<char, Command> kCharToCommand = initCharToCommand();

#ifndef DEBUG_H_
#define DEBUG_H_

#include "game.h"

namespace debug {

#ifdef NDEBUG
inline void Print(const GameState& state) {}
#else
void Print(const GameState& state);
#endif  // NDEBUG

}  // namespace debug

#endif  // DEBUG_H_

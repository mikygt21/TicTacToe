#pragma once
#include "Board.h"

namespace ttt {

// Stateless rule queries. Returns Mark::None if there is no winner.
Mark findWinner(const Board& board) noexcept;

} // namespace ttt

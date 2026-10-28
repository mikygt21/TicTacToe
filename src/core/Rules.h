#pragma once
#include "Board.h"

namespace ttt {

	// Returns Mark::None if there is no winner.
	Mark findWinner(const Board& board) noexcept;

}

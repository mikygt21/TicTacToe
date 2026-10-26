#pragma once
#include "core/Board.h"

namespace ttt {

// Where the board sits in the window, in pixels. Shared by drawing and input
// so both always agree on the geometry. Intentionally SFML-free.
struct BoardLayout {
    float x;     // left edge
    float y;     // top edge
    float size;  // side length of the (square) board

    constexpr float cellSize() const noexcept { return size / static_cast<float>(Board::Size); }
};

} // namespace ttt

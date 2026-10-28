#pragma once
#include "BoardLayout.h"
#include "core/Move.h"
#include <optional>

namespace ttt {

    // Converts a pixel position into a board cell
    class InputMapper 
    {
    public:
        explicit constexpr InputMapper(BoardLayout layout) noexcept : _layout(layout) {}

        std::optional<Move> cellAt(float px, float py) const noexcept
        {
            const float lx = px - _layout.x;
            const float ly = py - _layout.y;
            if (lx < 0.f || ly < 0.f || lx >= _layout.size || ly >= _layout.size)
                return std::nullopt;

            const float cell = _layout.cellSize();
            const Move m{static_cast<int>(ly / cell), static_cast<int>(lx / cell)};
            if (!Board::inBounds(m)) // guards against float rounding at the edges
                return std::nullopt;
            return m;
        }

    private:
        BoardLayout _layout;
    };

}

#pragma once
#include "BoardLayout.h"
#include "core/Move.h"
#include <optional>

namespace ttt {

    // Converts a pixel position into a board cell. Pure maths: unit-testable.
    class InputMapper {
    public:
        explicit constexpr InputMapper(BoardLayout layout) noexcept : layout_(layout) {}

        std::optional<Move> cellAt(float px, float py) const noexcept
        {
            const float lx = px - layout_.x;
            const float ly = py - layout_.y;
            if (lx < 0.f || ly < 0.f || lx >= layout_.size || ly >= layout_.size)
                return std::nullopt;

            const float cell = layout_.cellSize();
            const Move m{static_cast<int>(ly / cell), static_cast<int>(lx / cell)};
            if (!Board::inBounds(m)) // guards against float rounding at the edges
                return std::nullopt;
            return m;
        }

    private:
        BoardLayout layout_;
    };

}

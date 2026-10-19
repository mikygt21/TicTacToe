#pragma once

namespace ttt {

struct Move {
    int row;
    int col;

    friend constexpr bool operator==(Move a, Move b) noexcept
    {
        return a.row == b.row && a.col == b.col;
    }
};

} // namespace ttt

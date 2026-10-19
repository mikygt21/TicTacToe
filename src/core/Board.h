#pragma once
#include "Mark.h"
#include "Move.h"
#include <array>

namespace ttt {

// Pure data holder for the 3x3 grid. Knows nothing about turns or winning.
class Board {
public:
    static constexpr int Size = 3;

    static constexpr bool inBounds(Move m) noexcept
    {
        return m.row >= 0 && m.row < Size && m.col >= 0 && m.col < Size;
    }

    // Out-of-bounds queries return Mark::None.
    Mark at(Move m) const noexcept;
    bool isEmpty(Move m) const noexcept;

    // Returns false (and changes nothing) if out of bounds, occupied, or mark == None.
    bool place(Move m, Mark mark) noexcept;

    bool isFull() const noexcept;
    void clear() noexcept;

private:
    static constexpr int index(Move m) noexcept { return m.row * Size + m.col; }

    std::array<Mark, Size * Size> cells_{}; // value-initialised -> Mark::None
};

} // namespace ttt

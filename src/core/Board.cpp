#include "Board.h"
#include <algorithm>

namespace ttt {

    Mark Board::at(Move m) const noexcept
    {
        return inBounds(m) ? cells_[index(m)] : Mark::None;
    }

    bool Board::isEmpty(Move m) const noexcept
    {
        return inBounds(m) && cells_[index(m)] == Mark::None;
    }

    bool Board::place(Move m, Mark mark) noexcept
    {
        if (mark == Mark::None || !isEmpty(m))
            return false;
        cells_[index(m)] = mark;
        return true;
    }

    bool Board::isFull() const noexcept
    {
        return std::none_of(cells_.begin(), cells_.end(),
                            [](Mark c) { return c == Mark::None; });
    }

    void Board::clear() noexcept
    {
        cells_.fill(Mark::None);
    }

}

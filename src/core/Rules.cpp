#include "Rules.h"
#include <array>

namespace ttt {

    namespace {

        using Line = std::array<Move, 3>;

        constexpr std::array<Line, 8> kLines{{
            // rows
            {{ {0,0}, {0,1}, {0,2} }},
            {{ {1,0}, {1,1}, {1,2} }},
            {{ {2,0}, {2,1}, {2,2} }},
            // columns
            {{ {0,0}, {1,0}, {2,0} }},
            {{ {0,1}, {1,1}, {2,1} }},
            {{ {0,2}, {1,2}, {2,2} }},
            // diagonals
            {{ {0,0}, {1,1}, {2,2} }},
            {{ {0,2}, {1,1}, {2,0} }},
        }};

    } // namespace

    Mark findWinner(const Board& board) noexcept
    {
        for (const Line& line : kLines) {
            const Mark first = board.at(line[0]);
            if (first != Mark::None &&
                first == board.at(line[1]) &&
                first == board.at(line[2]))
                return first;
        }
        return Mark::None;
    }

} // namespace ttt

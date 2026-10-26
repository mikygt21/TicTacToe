#pragma once
#include "Board.h"
#include "Move.h"
#include <optional>

namespace ttt {

    // A player is polled once per frame on its turn. It must NOT block:
    //  - return a Move when it has decided,
    //  - return std::nullopt when it is still "thinking" (e.g. waiting for a click
    //    or a network message).
    // An illegal move is simply rejected by Game and the player is polled again.
    class IPlayer {
    public:
        virtual ~IPlayer() = default;
        virtual std::optional<Move> chooseMove(const Board& board, Mark self) = 0;
    };

}

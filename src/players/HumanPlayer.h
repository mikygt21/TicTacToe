#pragma once
#include "../core/IPlayer.h"

namespace ttt {

    // Input-agnostic: the UI layer translates a click into a Move and calls submit().
    // Only submit() for the player whose turn it is, otherwise the move is applied later.
    class HumanPlayer final : public IPlayer {
    public:
        void submit(Move m) noexcept { pending_ = m; }

        std::optional<Move> chooseMove(const Board&, Mark) override
        {
            auto m = pending_;
            pending_.reset();
            return m;
        }

    private:
        std::optional<Move> pending_;
    };

}

#pragma once
#include "../core/IPlayer.h"

namespace ttt {

    // Input-agnostic: the UI layer translates a click into a Move and calls submit().
    // Only submit() for the player whose turn it is, otherwise the move is applied later.
    class HumanPlayer final : public IPlayer {
    public:
        void submit(Move m) noexcept { _pending = m; }

        std::optional<Move> chooseMove(const Board&, Mark) override
        {
            auto m = _pending;
            _pending.reset();
            return m;
        }

    private:
        std::optional<Move> _pending;
    };

}

#pragma once
#include "../core/IPlayer.h"
#include <cstdint>
#include <random>

namespace ttt {

    enum class Difficulty 
    {
        Easy,    // random legal move
        Medium,  // takes a win, blocks a loss, otherwise random
        Hard     // minimax: never loses
    };

    class AIPlayer final : public IPlayer 
    {
    public:
        // The seed is injectable so behaviour can be made reproducible in tests.
        explicit AIPlayer(Difficulty difficulty, std::uint32_t seed = std::random_device{}());

        // Returns std::nullopt only when the board is full.
        std::optional<Move> chooseMove(const Board& board, Mark self) override;

    private:
        Difficulty difficulty_;
        std::mt19937 rng_;
    };

}

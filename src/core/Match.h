#pragma once
#include "Game.h"
#include "IPlayer.h"
#include <memory>

namespace ttt {

    // Connects a Game with two IPlayers. Depends only on abstractions (DIP).
    class Match {
    public:
        Match(std::unique_ptr<IPlayer> playerX, std::unique_ptr<IPlayer> playerO);

        void update();
        void restart() noexcept { _game.reset(); }

        const Game& game() const noexcept { return _game; }

    private:
        Game _game;
        std::unique_ptr<IPlayer> _playerX;
        std::unique_ptr<IPlayer> _playerO;
    };

}

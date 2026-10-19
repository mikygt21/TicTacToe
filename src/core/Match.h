#pragma once
#include "Game.h"
#include "IPlayer.h"
#include <memory>

namespace ttt {

// Connects a Game with two IPlayers. Depends only on abstractions (DIP).
class Match {
public:
    Match(std::unique_ptr<IPlayer> playerX, std::unique_ptr<IPlayer> playerO);

    void update();   // call once per frame
    void restart() noexcept { game_.reset(); }

    const Game& game() const noexcept { return game_; }

private:
    Game game_;
    std::unique_ptr<IPlayer> playerX_;
    std::unique_ptr<IPlayer> playerO_;
};

} // namespace ttt

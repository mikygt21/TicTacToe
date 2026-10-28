#include "Match.h"
#include <stdexcept>
#include <utility>

namespace ttt {

    Match::Match(std::unique_ptr<IPlayer> playerX, std::unique_ptr<IPlayer> playerO)
        : _playerX(std::move(playerX))
        , _playerO(std::move(playerO))
    {
        if (!_playerX || !_playerO)
            throw std::invalid_argument("Match requires two players");
    }

    void Match::update()
    {
        if (_game.isOver())
            return;

        const Mark turn = _game.currentTurn();
        IPlayer& player = (turn == Mark::X) ? *_playerX : *_playerO;

        if (const auto move = player.chooseMove(_game.board(), turn))
            _game.tryPlay(*move); // illegal moves are rejected; player is polled again
    }

}

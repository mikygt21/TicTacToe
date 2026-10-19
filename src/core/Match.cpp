#include "Match.h"
#include <stdexcept>
#include <utility>

namespace ttt {

Match::Match(std::unique_ptr<IPlayer> playerX, std::unique_ptr<IPlayer> playerO)
    : playerX_(std::move(playerX))
    , playerO_(std::move(playerO))
{
    if (!playerX_ || !playerO_)
        throw std::invalid_argument("Match requires two players");
}

void Match::update()
{
    if (game_.isOver())
        return;

    const Mark turn = game_.currentTurn();
    IPlayer& player = (turn == Mark::X) ? *playerX_ : *playerO_;

    if (const auto move = player.chooseMove(game_.board(), turn))
        game_.tryPlay(*move); // illegal moves are rejected; player is polled again
}

} // namespace ttt

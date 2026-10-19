#include "Game.h"
#include "Rules.h"

namespace ttt {

MoveResult Game::tryPlay(Move m) noexcept
{
    if (isOver())
        return MoveResult::GameOver;
    if (!Board::inBounds(m))
        return MoveResult::OutOfBounds;
    if (!board_.place(m, turn_))
        return MoveResult::CellOccupied;

    switch (findWinner(board_)) {
    case Mark::X: status_ = GameStatus::XWon; break;
    case Mark::O: status_ = GameStatus::OWon; break;
    default:
        if (board_.isFull())
            status_ = GameStatus::Draw;
        else
            turn_ = opponent(turn_);
    }
    return MoveResult::Ok;
}

void Game::reset() noexcept
{
    board_.clear();
    turn_ = Mark::X;
    status_ = GameStatus::InProgress;
}

} // namespace ttt

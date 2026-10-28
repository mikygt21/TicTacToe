#include "Game.h"
#include "Rules.h"

namespace ttt {

    MoveResult Game::tryPlay(Move m) noexcept
    {
        if (isOver())
            return MoveResult::GameOver;
        if (!Board::inBounds(m))
            return MoveResult::OutOfBounds;
        if (!_board.place(m, _turn))
            return MoveResult::CellOccupied;

        switch (findWinner(_board)) 
        {
        case Mark::X: 
            _status = GameStatus::XWon; 
            break;
        case Mark::O: 
            _status = GameStatus::OWon; 
            break;
        default:
            if (_board.isFull())
                _status = GameStatus::Draw;
            else
                _turn = opponent(_turn);
        }
        return MoveResult::Ok;
    }

    void Game::reset() noexcept
    {
        _board.clear();
        _turn = Mark::X;
        _status = GameStatus::InProgress;
    }

}

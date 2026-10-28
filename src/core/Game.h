#pragma once
#include "Board.h"

namespace ttt {

    enum class GameStatus { InProgress, XWon, OWon, Draw };
    enum class MoveResult { Ok, OutOfBounds, CellOccupied, GameOver };

    // Owns the state of ONE game and enforces the rules
    class Game {
    public:
        MoveResult tryPlay(Move m) noexcept;
        void reset() noexcept;

        const Board& board() const noexcept { return _board; }
        Mark currentTurn() const noexcept { return _turn; }
        GameStatus status() const noexcept { return _status; }
        bool isOver() const noexcept { return _status != GameStatus::InProgress; }

    private:
        Board _board;
        Mark _turn = Mark::X;
        GameStatus _status = GameStatus::InProgress;
    };

}

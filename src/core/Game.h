#pragma once
#include "Board.h"

namespace ttt {

enum class GameStatus { InProgress, XWon, OWon, Draw };
enum class MoveResult { Ok, OutOfBounds, CellOccupied, GameOver };

// Owns the state of ONE game and enforces the rules. No players, no I/O.
class Game {
public:
    MoveResult tryPlay(Move m) noexcept;
    void reset() noexcept;

    const Board& board() const noexcept { return board_; }
    Mark currentTurn() const noexcept { return turn_; }
    GameStatus status() const noexcept { return status_; }
    bool isOver() const noexcept { return status_ != GameStatus::InProgress; }

private:
    Board board_;
    Mark turn_ = Mark::X;
    GameStatus status_ = GameStatus::InProgress;
};

} // namespace ttt

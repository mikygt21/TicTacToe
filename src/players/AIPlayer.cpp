#include "AIPlayer.h"
#include "../core/Rules.h"
#include <algorithm>
#include <vector>

namespace ttt {

    namespace {

        std::vector<Move> legalMoves(const Board& board)
        {
            std::vector<Move> moves;
            moves.reserve(Board::Size * Board::Size);
            for (int row = 0; row < Board::Size; ++row)
                for (int col = 0; col < Board::Size; ++col)
                    if (board.isEmpty({row, col}))
                        moves.push_back({row, col});
            return moves;
        }

        Move pickRandom(const std::vector<Move>& moves, std::mt19937& rng)
        {
            std::uniform_int_distribution<std::size_t> dist(0, moves.size() - 1);
            return moves[dist(rng)];
        }

        // A move that wins the game immediately for 'mark', if one exists
        std::optional<Move> findCompletingMove(const Board& board, Mark mark)
        {
            for (Move m : legalMoves(board)) {
                Board next = board;
                next.place(m, mark);
                if (findWinner(next) == mark)
                    return m;
            }
            return std::nullopt;
        }

        // Score from self's point of view. Depth is subtracted so that faster wins
        // (and slower losses) are preferred.
        int minimax(const Board& board, Mark turn, Mark self, int depth)
        {
            const Mark winner = findWinner(board);
            if (winner == self)
                return 10 - depth;
            if (winner != Mark::None)
                return depth - 10;
            if (board.isFull())
                return 0;

            const bool maximizing = (turn == self);
            int best = maximizing ? -100 : 100;
            for (Move m : legalMoves(board)) {
                Board next = board;
                next.place(m, turn);
                const int score = minimax(next, opponent(turn), self, depth + 1);
                best = maximizing ? std::max(best, score) : std::min(best, score);
            }
            return best;
        }

        Move bestMove(const Board& board, Mark self, const std::vector<Move>& moves, std::mt19937& rng)
        {
            std::vector<Move> best;
            int bestScore = -100;
            for (Move m : moves) {
                Board next = board;
                next.place(m, self);
                const int score = minimax(next, opponent(self), self, 1);
                if (score > bestScore) {
                    bestScore = score;
                    best.assign(1, m);
                } else if (score == bestScore) {
                    best.push_back(m);
                }
            }
            return pickRandom(best, rng); // random among equally good moves: less robotic
        }

    } // namespace

    AIPlayer::AIPlayer(Difficulty difficulty, std::uint32_t seed)
        : _difficulty(difficulty)
        , _rng(seed)
    {
    }

    std::optional<Move> AIPlayer::chooseMove(const Board& board, Mark self)
    {
        const std::vector<Move> moves = legalMoves(board);
        if (moves.empty())
            return std::nullopt;

        switch (_difficulty) {
        case Difficulty::Hard:
            return bestMove(board, self, moves, _rng);
        case Difficulty::Medium:
            if (const auto win = findCompletingMove(board, self))
                return win;
            if (const auto block = findCompletingMove(board, opponent(self)))
                return block;
            return pickRandom(moves, _rng);
        case Difficulty::Easy:
            break;
        }
        return pickRandom(moves, _rng);
    }

} // namespace ttt

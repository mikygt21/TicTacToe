#include "TestUtil.h"
#include "Tests.h"
#include "core/Game.h"
#include "core/Rules.h"
#include "players/AIPlayer.h"
#include <string>

using namespace ttt;

// Builds a board from 9 characters, row-major: 'X', 'O' or '.'
static Board boardFrom(const std::string& cells)
{
    Board b;
    for (int i = 0; i < 9; ++i) {
        const Mark m = cells[i] == 'X' ? Mark::X : (cells[i] == 'O' ? Mark::O : Mark::None);
        if (m != Mark::None) b.place({i / 3, i % 3}, m);
    }
    return b;
}

static void hardTakesImmediateWin()
{
    // X X .
    // O O .
    // . . .      X to move: only {0,2} wins on the spot
    AIPlayer ai(Difficulty::Hard, 1);
    const auto m = ai.chooseMove(boardFrom("XX.OO...."), Mark::X);
    CHECK(m.has_value() && *m == Move{0, 2});
}

static void hardBlocksOpponentWin()
{
    // X X .
    // . O .
    // . . .      O to move: must block {0,2}
    AIPlayer ai(Difficulty::Hard, 1);
    const auto m = ai.chooseMove(boardFrom("XX..O...."), Mark::O);
    CHECK(m.has_value() && *m == Move{0, 2});
}

static void mediumTakesWinAndBlocks()
{
    AIPlayer ai(Difficulty::Medium, 1);
    const auto win = ai.chooseMove(boardFrom("XX.OO...."), Mark::X);
    CHECK(win.has_value() && *win == Move{0, 2});
    const auto block = ai.chooseMove(boardFrom("XX..O...."), Mark::O);
    CHECK(block.has_value() && *block == Move{0, 2});
}

static void everyDifficultyPlaysOnlyLegalMoves()
{
    const Board board = boardFrom("XO.X.O...");
    for (Difficulty d : {Difficulty::Easy, Difficulty::Medium, Difficulty::Hard}) {
        AIPlayer ai(d, 7);
        for (int i = 0; i < 200; ++i) {
            const auto m = ai.chooseMove(board, Mark::X);
            CHECK(m.has_value() && board.isEmpty(*m));
        }
    }
}

static void returnsNothingOnFullBoard()
{
    const Board full = boardFrom("XOXXOOOXX");
    for (Difficulty d : {Difficulty::Easy, Difficulty::Medium, Difficulty::Hard}) {
        AIPlayer ai(d, 1);
        CHECK(!ai.chooseMove(full, Mark::X).has_value());
    }
}

static void sameSeedGivesSameEasyMoves()
{
    AIPlayer a(Difficulty::Easy, 42), b(Difficulty::Easy, 42);
    const Board empty;
    for (int i = 0; i < 20; ++i)
        CHECK(*a.chooseMove(empty, Mark::X) == *b.chooseMove(empty, Mark::X));
}

// Exhaustively tries every possible reply by the opponent of the AI.
static void exploreAll(Game game, Mark aiMark, AIPlayer& ai, int& finishedGames)
{
    if (game.isOver()) {
        ++finishedGames;
        const GameStatus aiLoses = (aiMark == Mark::X) ? GameStatus::OWon : GameStatus::XWon;
        CHECK(game.status() != aiLoses);
        return;
    }
    if (game.currentTurn() == aiMark) {
        const auto m = ai.chooseMove(game.board(), aiMark);
        CHECK(m.has_value());
        if (!m) return;
        CHECK(game.tryPlay(*m) == MoveResult::Ok);
        exploreAll(game, aiMark, ai, finishedGames);
        return;
    }
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (game.board().isEmpty({r, c})) {
                Game next = game;
                next.tryPlay({r, c});
                exploreAll(next, aiMark, ai, finishedGames);
            }
}

static void hardNeverLoses()
{
    for (Mark aiMark : {Mark::X, Mark::O}) {
        AIPlayer ai(Difficulty::Hard, 3);
        int games = 0;
        exploreAll(Game{}, aiMark, ai, games);
        std::cout << "  explored " << games << " games as " << (aiMark == Mark::X ? "X" : "O") << "\n";
        CHECK(games > 20); // sanity: the exploration really branched
    }
}

void runAiTests()
{
    hardTakesImmediateWin();
    hardBlocksOpponentWin();
    mediumTakesWinAndBlocks();
    everyDifficultyPlaysOnlyLegalMoves();
    returnsNothingOnFullBoard();
    sameSeedGivesSameEasyMoves();
    hardNeverLoses();
}

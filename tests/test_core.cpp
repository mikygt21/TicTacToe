#include "TestUtil.h"
#include "core/Board.h"
#include "core/Game.h"
#include "core/Match.h"
#include "core/Rules.h"
#include "players/HumanPlayer.h"
#include <initializer_list>
#include <queue>
#include <stdexcept>

using namespace ttt;

static void play(Game& g, std::initializer_list<Move> moves)
{
    for (Move m : moves) g.tryPlay(m);
}

static void boardRejectsInvalidPlacement()
{
    Board b;
    CHECK(b.place({0, 0}, Mark::X));
    CHECK(!b.place({0, 0}, Mark::O));       // occupied
    CHECK(!b.place({3, 0}, Mark::X));       // out of bounds
    CHECK(!b.place({-1, 1}, Mark::X));
    CHECK(!b.place({1, 1}, Mark::None));    // invalid mark
    CHECK(b.at({0, 0}) == Mark::X);
    CHECK(b.at({9, 9}) == Mark::None);
}

static void rowColumnDiagonalWins()
{
    { Game g; play(g, {{0,0},{1,0},{0,1},{1,1},{0,2}}); CHECK(g.status() == GameStatus::XWon); }
    { Game g; play(g, {{0,0},{0,1},{1,0},{1,1},{2,0}}); CHECK(g.status() == GameStatus::XWon); }
    { Game g; play(g, {{0,0},{0,1},{1,1},{0,2},{2,2}}); CHECK(g.status() == GameStatus::XWon); }
    { Game g; play(g, {{0,0},{0,2},{0,1},{1,1},{2,2},{2,0}}); CHECK(g.status() == GameStatus::OWon); }
}

static void drawIsDetected()
{
    Game g;
    // X O X
    // X O O
    // O X X
    play(g, {{0,0},{0,1},{0,2},{1,1},{1,0},{1,2},{2,1},{2,0},{2,2}});
    CHECK(g.status() == GameStatus::Draw);
    CHECK(g.board().isFull());
}

static void winOnLastCellIsNotADraw()
{
    Game g;
    // X O X
    // O X O
    // O X X  <- X completes the diagonal with the 9th move
    play(g, {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,1},{2,0},{2,2}});
    CHECK(g.status() == GameStatus::XWon);
}

static void gameRejectsIllegalMoves()
{
    Game g;
    CHECK(g.tryPlay({1, 1}) == MoveResult::Ok);
    CHECK(g.currentTurn() == Mark::O);
    CHECK(g.tryPlay({1, 1}) == MoveResult::CellOccupied);
    CHECK(g.currentTurn() == Mark::O);      // turn not consumed
    CHECK(g.tryPlay({5, 5}) == MoveResult::OutOfBounds);

    Game won;
    play(won, {{0,0},{1,0},{0,1},{1,1},{0,2}});
    CHECK(won.tryPlay({2,2}) == MoveResult::GameOver);
}

static void resetRestoresInitialState()
{
    Game g;
    play(g, {{0,0},{1,1}});
    g.reset();
    CHECK(g.status() == GameStatus::InProgress);
    CHECK(g.currentTurn() == Mark::X);
    CHECK(g.board().isEmpty({0, 0}));
}

namespace {
class ScriptedPlayer final : public IPlayer {
public:
    explicit ScriptedPlayer(std::queue<Move> moves) : moves_(std::move(moves)) {}
    std::optional<Move> chooseMove(const Board&, Mark) override
    {
        if (moves_.empty()) return std::nullopt;
        Move m = moves_.front(); moves_.pop(); return m;
    }
private:
    std::queue<Move> moves_;
};
} // namespace

static void matchRunsTwoPlayersToCompletion()
{
    Match match(std::make_unique<ScriptedPlayer>(std::queue<Move>({{0,0},{0,1},{0,2}})),
                std::make_unique<ScriptedPlayer>(std::queue<Move>({{1,0},{1,1}})));
    for (int i = 0; i < 10; ++i) match.update();
    CHECK(match.game().status() == GameStatus::XWon);
}

static void matchWaitsForHumanInput()
{
    auto x = std::make_unique<HumanPlayer>();
    HumanPlayer* rawX = x.get();
    Match match(std::move(x), std::make_unique<HumanPlayer>());

    match.update();
    CHECK(match.game().board().isEmpty({0, 0}));    // nothing happens without input
    rawX->submit({0, 0});
    match.update();
    CHECK(match.game().board().at({0, 0}) == Mark::X);
    CHECK(match.game().currentTurn() == Mark::O);
}

static void matchRejectsNullPlayers()
{
    bool threw = false;
    try { Match m(nullptr, nullptr); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
}

int main()
{
    boardRejectsInvalidPlacement();
    rowColumnDiagonalWins();
    drawIsDetected();
    winOnLastCellIsNotADraw();
    gameRejectsIllegalMoves();
    resetRestoresInitialState();
    matchRunsTwoPlayersToCompletion();
    matchWaitsForHumanInput();
    matchRejectsNullPlayers();

    return ttt_test::report();
}

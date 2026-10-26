#include "TestUtil.h"
#include "presentation/InputMapper.h"

using namespace ttt;

int main()
{
    // Board at (50,110), 500px wide -> each cell ~166.67px
    const InputMapper mapper(BoardLayout{50.f, 110.f, 500.f});

    CHECK(mapper.cellAt(60.f, 120.f)   == std::optional<Move>(Move{0, 0}));
    CHECK(mapper.cellAt(300.f, 360.f)  == std::optional<Move>(Move{1, 1}));
    CHECK(mapper.cellAt(549.f, 609.f)  == std::optional<Move>(Move{2, 2}));
    CHECK(mapper.cellAt(540.f, 120.f)  == std::optional<Move>(Move{0, 2}));
    CHECK(mapper.cellAt(60.f, 600.f)   == std::optional<Move>(Move{2, 0}));

    // Outside the board (including exact far edges, which are exclusive)
    CHECK(!mapper.cellAt(49.f, 300.f).has_value());
    CHECK(!mapper.cellAt(300.f, 109.f).has_value());
    CHECK(!mapper.cellAt(550.f, 300.f).has_value());
    CHECK(!mapper.cellAt(300.f, 610.f).has_value());
    CHECK(!mapper.cellAt(-10.f, -10.f).has_value());
    CHECK(!mapper.cellAt(1000.f, 1000.f).has_value());

    return ttt_test::report();
}

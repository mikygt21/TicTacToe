#include "TestUtil.h"
#include "Tests.h"
#include <iostream>
#include <string>

// Usage: TicTacToeTests [core|ai|ui|all]   (default: all)
int main(int argc, char** argv)
{
    const std::string suite = argc > 1 ? argv[1] : "all";
    const bool all = (suite == "all");
    bool ran = false;

    if (all || suite == "core") { runCoreTests(); ran = true; }
    if (all || suite == "ai")   { runAiTests();   ran = true; }
    if (all || suite == "ui")   { runUiTests();   ran = true; }

    if (!ran) {
        std::cerr << "Unknown suite '" << suite << "'. Use: core, ai, ui or all.\n";
        return 2;
    }
    return ttt_test::report();
}

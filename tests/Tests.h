#pragma once

// Each suite lives in its own file and is run by tests/main.cpp.
void runCoreTests(); // Board, Game, Rules, Match, HumanPlayer
void runAiTests();   // AIPlayer (all difficulties)
void runUiTests();   // InputMapper (pixel -> cell)

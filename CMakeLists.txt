cmake_minimum_required(VERSION 3.20)
project(TicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Core library: no SFML dependency -------------------------------------
add_library(ttt_core
    src/core/Board.cpp
    src/core/Rules.cpp
    src/core/Game.cpp
    src/core/Match.cpp)
target_include_directories(ttt_core PUBLIC src)
if(MSVC)
    target_compile_options(ttt_core PRIVATE /W4)
else()
    target_compile_options(ttt_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- Tests -----------------------------------------------------------------
enable_testing()
add_executable(ttt_tests tests/test_core.cpp)
target_link_libraries(ttt_tests PRIVATE ttt_core)
add_test(NAME core COMMAND ttt_tests)

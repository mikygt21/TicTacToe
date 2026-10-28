cmake_minimum_required(VERSION 3.21)
project(TicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Groups ALL_BUILD / ZERO_CHECK / RUN_TESTS under one folder in Visual Studio
# and lets source_group() create folders in Solution Explorer.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(TTT_BUILD_GAME "Build the SFML game (needs the bundled SFML in ./dependencies)" ON)

function(ttt_enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# Source lists. The logic (core + players) has no SFML dependency and is
# compiled into BOTH the game and the tests, so each ends up as ONE project.
# ---------------------------------------------------------------------------
set(TTT_LOGIC_SOURCES
    src/core/Board.cpp
    src/core/Game.cpp
    src/core/Match.cpp
    src/core/Rules.cpp
    src/players/AIPlayer.cpp)

set(TTT_LOGIC_HEADERS
    src/core/Board.h
    src/core/Game.h
    src/core/IPlayer.h
    src/core/Mark.h
    src/core/Match.h
    src/core/Move.h
    src/core/Rules.h
    src/players/AIPlayer.h
    src/players/HumanPlayer.h)

# ---------------------------------------------------------------------------
# Project 1: the game
# ---------------------------------------------------------------------------
if(TTT_BUILD_GAME)
    # Use the SFML 3 binaries bundled in ./dependencies (no network needed).
    set(SFML_DIR "${PROJECT_SOURCE_DIR}/dependencies/lib/cmake/SFML" CACHE PATH "Folder containing SFMLConfig.cmake")
    set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)
    set(CMAKE_MAP_IMPORTED_CONFIG_MINSIZEREL Release)
    find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics)

    set(TTT_GAME_SOURCES
        src/main.cpp
        src/app/Application.cpp
        src/app/MenuScene.cpp
        src/app/GameScene.cpp
        src/presentation/BoardView.cpp
        src/presentation/Button.cpp)

    set(TTT_GAME_HEADERS
        src/app/Application.h
        src/app/GameScene.h
        src/app/IScene.h
        src/app/MenuScene.h
        src/presentation/BoardLayout.h
        src/presentation/BoardView.h
        src/presentation/Button.h
        src/presentation/InputMapper.h
        src/presentation/TextUtil.h
        src/presentation/UiConfig.h)

    add_executable(TicTacToe
        ${TTT_GAME_SOURCES} ${TTT_GAME_HEADERS}
        ${TTT_LOGIC_SOURCES} ${TTT_LOGIC_HEADERS})
    target_include_directories(TicTacToe PRIVATE src)
    target_link_libraries(TicTacToe PRIVATE SFML::Graphics)
    target_compile_definitions(TicTacToe PRIVATE "TTT_ASSETS_DIR=\"${PROJECT_SOURCE_DIR}/Assets\"")
    ttt_enable_warnings(TicTacToe)

    # Mirror the src/ folder structure in Solution Explorer.
    source_group(TREE "${PROJECT_SOURCE_DIR}/src" FILES
        ${TTT_GAME_SOURCES} ${TTT_GAME_HEADERS} ${TTT_LOGIC_SOURCES} ${TTT_LOGIC_HEADERS})

    # Visual Studio: F5 runs the game, from the folder that holds the exe + assets.
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT TicTacToe)
    set_target_properties(TicTacToe PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:TicTacToe>")

    # Put the assets (and SFML's DLLs on Windows) next to the executable.
    add_custom_command(TARGET TicTacToe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${PROJECT_SOURCE_DIR}/Assets" "$<TARGET_FILE_DIR:TicTacToe>/Assets")
    if(WIN32)
        add_custom_command(TARGET TicTacToe POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_RUNTIME_DLLS:TicTacToe> $<TARGET_FILE_DIR:TicTacToe>
            COMMAND_EXPAND_LISTS)
    endif()
endif()

# ---------------------------------------------------------------------------
# Project 2: the tests (one executable, three suites, no SFML needed)
# ---------------------------------------------------------------------------
enable_testing()

set(TTT_TEST_SOURCES
    tests/main.cpp
    tests/test_core.cpp
    tests/test_ai.cpp
    tests/test_ui.cpp)

set(TTT_TEST_HEADERS
    tests/Tests.h
    tests/TestUtil.h
    src/presentation/BoardLayout.h   # header-only logic under test (InputMapper)
    src/presentation/InputMapper.h)

add_executable(TicTacToeTests
    ${TTT_TEST_SOURCES} ${TTT_TEST_HEADERS}
    ${TTT_LOGIC_SOURCES} ${TTT_LOGIC_HEADERS})
target_include_directories(TicTacToeTests PRIVATE src)
ttt_enable_warnings(TicTacToeTests)

source_group("tests" FILES ${TTT_TEST_SOURCES} tests/Tests.h tests/TestUtil.h)
source_group(TREE "${PROJECT_SOURCE_DIR}/src" FILES
    ${TTT_LOGIC_SOURCES} ${TTT_LOGIC_HEADERS}
    src/presentation/BoardLayout.h src/presentation/InputMapper.h)

add_test(NAME core COMMAND TicTacToeTests core)
add_test(NAME ai   COMMAND TicTacToeTests ai)
add_test(NAME ui   COMMAND TicTacToeTests ui)

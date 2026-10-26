cmake_minimum_required(VERSION 3.21)
project(TicTacToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(TTT_BUILD_GAME "Build the SFML game (needs the bundled SFML in ./dependencies)" ON)

function(ttt_enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# --- Core: rules and abstractions. No SFML. --------------------------------
add_library(ttt_core
    src/core/Board.cpp
    src/core/Rules.cpp
    src/core/Game.cpp
    src/core/Match.cpp)
target_include_directories(ttt_core PUBLIC src)
ttt_enable_warnings(ttt_core)

# --- Players: implementations of IPlayer. No SFML. -------------------------
add_library(ttt_players src/players/AIPlayer.cpp)
target_link_libraries(ttt_players PUBLIC ttt_core)
ttt_enable_warnings(ttt_players)

# --- Tests -------------------------------------------------------------------
enable_testing()

add_executable(ttt_core_tests tests/test_core.cpp)
target_link_libraries(ttt_core_tests PRIVATE ttt_players)
add_test(NAME core COMMAND ttt_core_tests)

add_executable(ttt_ai_tests tests/test_ai.cpp)
target_link_libraries(ttt_ai_tests PRIVATE ttt_players)
add_test(NAME ai COMMAND ttt_ai_tests)

add_executable(ttt_ui_tests tests/test_ui.cpp)
target_link_libraries(ttt_ui_tests PRIVATE ttt_core)
add_test(NAME ui COMMAND ttt_ui_tests)

# --- The game ----------------------------------------------------------------
if(TTT_BUILD_GAME)
    # Use the SFML 3 binaries bundled in ./dependencies (no network needed).
    set(SFML_DIR "${PROJECT_SOURCE_DIR}/dependencies/lib/cmake/SFML" CACHE PATH "Folder containing SFMLConfig.cmake")
    set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)
    set(CMAKE_MAP_IMPORTED_CONFIG_MINSIZEREL Release)
    find_package(SFML 3 CONFIG REQUIRED COMPONENTS Graphics)

    add_executable(TicTacToe
        src/main.cpp
        src/app/Application.cpp
        src/app/MenuScene.cpp
        src/app/GameScene.cpp
        src/presentation/BoardView.cpp
        src/presentation/Button.cpp)
    target_link_libraries(TicTacToe PRIVATE ttt_players SFML::Graphics)
    target_compile_definitions(TicTacToe PRIVATE "TTT_ASSETS_DIR=\"${PROJECT_SOURCE_DIR}/Assets\"")
    ttt_enable_warnings(TicTacToe)

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

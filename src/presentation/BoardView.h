#pragma once
#include "BoardLayout.h"
#include "core/Board.h"
#include <SFML/Graphics.hpp>

namespace ttt {

    // Draws a Board. Read-only: never modifies game state
    class BoardView {
    public:
        explicit BoardView(BoardLayout layout) : layout_(layout) {}

        void draw(sf::RenderTarget& target, const Board& board) const;

    private:
        void drawGrid(sf::RenderTarget& target) const;
        void drawX(sf::RenderTarget& target, sf::Vector2f center) const;
        void drawO(sf::RenderTarget& target, sf::Vector2f center) const;

        BoardLayout layout_;
    };

}

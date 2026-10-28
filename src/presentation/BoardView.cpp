#include "BoardView.h"
#include "UiConfig.h"

namespace ttt {

    namespace {
        constexpr float kGridThickness = 6.f;
        constexpr float kMarkThickness = 14.f;
    } // namespace

    void BoardView::draw(sf::RenderTarget& target, const Board& board) const
    {
        drawGrid(target);

        const float cell = _layout.cellSize();
        for (int row = 0; row < Board::Size; ++row) {
            for (int col = 0; col < Board::Size; ++col) {
                const sf::Vector2f center{
                    _layout.x + (static_cast<float>(col) + 0.5f) * cell, 
                    _layout.y + (static_cast<float>(row) + 0.5f) * cell
                };

                switch (board.at({row, col})) 
                {
                case Mark::X:    
                    drawX(target, center); 
                    break;
                case Mark::O:    
                    drawO(target, center); 
                    break;
                case Mark::None: 
                    break;
                }
            }
        }
    }

    void BoardView::drawGrid(sf::RenderTarget& target) const
    {
        const float cell = _layout.cellSize();
        sf::RectangleShape line;
        line.setFillColor(ui::kGrid);

        for (int i = 1; i < Board::Size; ++i) {
            const float offset = static_cast<float>(i) * cell - kGridThickness / 2.f;

            line.setSize({kGridThickness, _layout.size}); // vertical
            line.setPosition({_layout.x + offset, _layout.y});
            target.draw(line);

            line.setSize({_layout.size, kGridThickness}); // horizontal
            line.setPosition({_layout.x, _layout.y + offset});
            target.draw(line);
        }
    }

    void BoardView::drawX(sf::RenderTarget& target, sf::Vector2f center) const
    {
        const float length = _layout.cellSize() * 0.8f;
        sf::RectangleShape bar({length, kMarkThickness});
        bar.setOrigin({length / 2.f, kMarkThickness / 2.f});
        bar.setPosition(center);
        bar.setFillColor(ui::kMarkX);

        bar.setRotation(sf::degrees(45.f));
        target.draw(bar);
        bar.setRotation(sf::degrees(-45.f));
        target.draw(bar);
    }

    void BoardView::drawO(sf::RenderTarget& target, sf::Vector2f center) const
    {
        const float radius = _layout.cellSize() * 0.28f;
        sf::CircleShape ring(radius, 64);
        ring.setOrigin({radius, radius});
        ring.setPosition(center);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(-kMarkThickness); // negative: grows inward
        ring.setOutlineColor(ui::kMarkO);
        target.draw(ring);
    }

} // namespace ttt

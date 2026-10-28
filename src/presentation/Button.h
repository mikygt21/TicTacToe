#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace ttt {

    class Button {
    public:
        Button(const sf::Font& font, const std::string& label, sf::Vector2f center, sf::Vector2f size);

        bool contains(float x, float y) const;
        void setHovered(bool hovered);
        void draw(sf::RenderTarget& target) const;

    private:
        sf::RectangleShape _box;
        sf::Text _label;
    };

}

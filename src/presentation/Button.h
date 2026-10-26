#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace ttt {

// Rectangle with a centred label. Has no behaviour of its own: the owning
// scene decides what a click means.
class Button {
public:
    Button(const sf::Font& font, const std::string& label, sf::Vector2f center, sf::Vector2f size);

    bool contains(float x, float y) const;
    void setHovered(bool hovered);
    void draw(sf::RenderTarget& target) const;

private:
    sf::RectangleShape box_;
    sf::Text label_;
};

} // namespace ttt

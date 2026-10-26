#pragma once
#include <SFML/Graphics/Text.hpp>

namespace ttt {

// Places 'text' so that its visual centre is at (cx, cy).
inline void centerText(sf::Text& text, float cx, float cy)
{
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                    bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({cx, cy});
}

}

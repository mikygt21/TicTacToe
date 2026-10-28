#include "Button.h"
#include "TextUtil.h"
#include "UiConfig.h"

namespace ttt {

    Button::Button(const sf::Font& font, const std::string& label, sf::Vector2f center, sf::Vector2f size)
        : _box(size), _label(font, label, 26)
    {
        _box.setOrigin(size / 2.f);
        _box.setPosition(center);
        _box.setFillColor(ui::kButton);

        _label.setFillColor(ui::kText);
        centerText(_label, center.x, center.y);
    }

    bool Button::contains(float x, float y) const
    {
        return _box.getGlobalBounds().contains({x, y});
    }

    void Button::setHovered(bool hovered)
    {
        _box.setFillColor(hovered ? ui::kButtonHover : ui::kButton);
    }

    void Button::draw(sf::RenderTarget& target) const
    {
        target.draw(_box);
        target.draw(_label);
    }

}

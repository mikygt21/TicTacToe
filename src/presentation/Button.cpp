#include "Button.h"
#include "TextUtil.h"
#include "UiConfig.h"

namespace ttt {

    Button::Button(const sf::Font& font, const std::string& label, sf::Vector2f center, sf::Vector2f size)
        : box_(size)
        , label_(font, label, 26)
    {
        box_.setOrigin(size / 2.f);
        box_.setPosition(center);
        box_.setFillColor(ui::kButton);

        label_.setFillColor(ui::kText);
        centerText(label_, center.x, center.y);
    }

    bool Button::contains(float x, float y) const
    {
        return box_.getGlobalBounds().contains({x, y});
    }

    void Button::setHovered(bool hovered)
    {
        box_.setFillColor(hovered ? ui::kButtonHover : ui::kButton);
    }

    void Button::draw(sf::RenderTarget& target) const
    {
        target.draw(box_);
        target.draw(label_);
    }

}

#include "MenuScene.h"
#include "GameScene.h"
#include "players/AIPlayer.h"
#include "presentation/TextUtil.h"
#include "presentation/UiConfig.h"

namespace ttt {

    namespace {
        constexpr float kCenterX = static_cast<float>(ui::kWindowWidth) / 2.f;
        const sf::Vector2f kButtonSize{260.f, 60.f};
    } // namespace

    MenuScene::MenuScene(ISceneNavigator& navigator, const sf::Font& font)
        : navigator_(navigator)
        , font_(font)
        , title_(font, "Tic-Tac-Toe", 64)
        , subtitle_(font, "Choose difficulty", 30)
        , easy_(font, "Easy", {kCenterX, 340.f}, kButtonSize)
        , medium_(font, "Medium", {kCenterX, 420.f}, kButtonSize)
        , hard_(font, "Hard", {kCenterX, 500.f}, kButtonSize)
        , exit_(font, "Exit", {kCenterX, 620.f}, kButtonSize)
    {
        title_.setFillColor(ui::kText);
        subtitle_.setFillColor(ui::kText);
        centerText(title_, kCenterX, 140.f);
        centerText(subtitle_, kCenterX, 240.f);
    }

    void MenuScene::handleEvent(const sf::Event& event)
    {
        if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
            const float x = static_cast<float>(moved->position.x);
            const float y = static_cast<float>(moved->position.y);
            easy_.setHovered(easy_.contains(x, y));
            medium_.setHovered(medium_.contains(x, y));
            hard_.setHovered(hard_.contains(x, y));
            exit_.setHovered(exit_.contains(x, y));
            return;
        }

        const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>();
        if (!pressed || pressed->button != sf::Mouse::Button::Left)
            return;

        const float x = static_cast<float>(pressed->position.x);
        const float y = static_cast<float>(pressed->position.y);

        if (easy_.contains(x, y))
            navigator_.goTo(std::make_unique<GameScene>(navigator_, font_, Difficulty::Easy));
        else if (medium_.contains(x, y))
            navigator_.goTo(std::make_unique<GameScene>(navigator_, font_, Difficulty::Medium));
        else if (hard_.contains(x, y))
            navigator_.goTo(std::make_unique<GameScene>(navigator_, font_, Difficulty::Hard));
        else if (exit_.contains(x, y))
            navigator_.quit();
    }

    void MenuScene::draw(sf::RenderTarget& target) const
    {
        target.draw(title_);
        target.draw(subtitle_);
        easy_.draw(target);
        medium_.draw(target);
        hard_.draw(target);
        exit_.draw(target);
    }

} // namespace ttt

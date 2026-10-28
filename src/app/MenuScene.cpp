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
        : _navigator(navigator)
        , _font(font)
        , _titleText(font, "Tic-Tac-Toe", 64)
        , _subtitleText(font, "Choose difficulty", 30)
        , _easyButton(font, "Easy", {kCenterX, 340.f}, kButtonSize)
        , _mediumButton(font, "Medium", {kCenterX, 420.f}, kButtonSize)
        , _hardButton(font, "Hard", {kCenterX, 500.f}, kButtonSize)
        , _exitButton(font, "Exit", {kCenterX, 620.f}, kButtonSize)
    {
        _titleText.setFillColor(ui::kText);
        _subtitleText.setFillColor(ui::kText);
        centerText(_titleText, kCenterX, 140.f);
        centerText(_subtitleText, kCenterX, 240.f);
    }

    void MenuScene::handleEvent(const sf::Event& event)
    {
        if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
            const float x = static_cast<float>(moved->position.x);
            const float y = static_cast<float>(moved->position.y);
            _easyButton.setHovered(_easyButton.contains(x, y));
            _mediumButton.setHovered(_mediumButton.contains(x, y));
            _hardButton.setHovered(_hardButton.contains(x, y));
            _exitButton.setHovered(_exitButton.contains(x, y));
            return;
        }

        const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>();
        if (!pressed || pressed->button != sf::Mouse::Button::Left)
            return;

        const float x = static_cast<float>(pressed->position.x);
        const float y = static_cast<float>(pressed->position.y);

        if (_easyButton.contains(x, y))
            _navigator.goTo(std::make_unique<GameScene>(_navigator, _font, Difficulty::Easy));
        else if (_mediumButton.contains(x, y))
            _navigator.goTo(std::make_unique<GameScene>(_navigator, _font, Difficulty::Medium));
        else if (_hardButton.contains(x, y))
            _navigator.goTo(std::make_unique<GameScene>(_navigator, _font, Difficulty::Hard));
        else if (_exitButton.contains(x, y))
            _navigator.quit();
    }

    void MenuScene::draw(sf::RenderTarget& target) const
    {
        target.draw(_titleText);
        target.draw(_subtitleText);
        _easyButton.draw(target);
        _mediumButton.draw(target);
        _hardButton.draw(target);
        _exitButton.draw(target);
    }

} // namespace ttt

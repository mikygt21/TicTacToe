#include "GameScene.h"
#include "MenuScene.h"
#include "presentation/TextUtil.h"
#include "presentation/UiConfig.h"

namespace ttt {

    namespace {

        constexpr Mark kHumanMark = Mark::X;
        constexpr float kAiThinkSeconds = 0.5f; // some delay

        constexpr float kCenterX = static_cast<float>(ui::kWindowWidth) / 2.f;
        constexpr float kButtonY = 665.f;
        const sf::Vector2f kButtonSize{170.f, 50.f};

        Match makeMatch(Difficulty difficulty, HumanPlayer*& humanOut)
        {
            auto human = std::make_unique<HumanPlayer>();
            humanOut = human.get();
            return Match(std::move(human), std::make_unique<AIPlayer>(difficulty));
        }

    } // namespace

    GameScene::GameScene(ISceneNavigator& navigator, const sf::Font& font, Difficulty difficulty)
        : _navigator(navigator)
        , _font(font)
        , _human(nullptr)
        , _match(makeMatch(difficulty, _human))
        , _mapper(ui::kBoardLayout)
        , _boardView(ui::kBoardLayout)
        , _status(font, "", 34)
        , _playAgainButton(font, "Play again", {kCenterX - 190.f, kButtonY}, kButtonSize)
        , _menuButton(font, "Menu", {kCenterX, kButtonY}, kButtonSize)
        , _exitButton(font, "Exit", {kCenterX + 190.f, kButtonY}, kButtonSize)
    {
        _status.setFillColor(ui::kText);
        refreshStatus();
    }

    void GameScene::handleEvent(const sf::Event& event)
    {
        if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
            const float x = static_cast<float>(moved->position.x);
            const float y = static_cast<float>(moved->position.y);
            _playAgainButton.setHovered(_playAgainButton.contains(x, y));
            _menuButton.setHovered(_menuButton.contains(x, y));
            _exitButton.setHovered(_exitButton.contains(x, y));
            return;
        }

        const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>();
        if (!pressed || pressed->button != sf::Mouse::Button::Left)
            return;

        const float x = static_cast<float>(pressed->position.x);
        const float y = static_cast<float>(pressed->position.y);

        if (_match.game().isOver()) {
            if (_playAgainButton.contains(x, y))
                _match.restart();
            else if (_menuButton.contains(x, y))
                _navigator.goTo(std::make_unique<MenuScene>(_navigator, _font));
            else if (_exitButton.contains(x, y))
                _navigator.quit();
            return;
        }

        // Only forward clicks on the human's turn
        if (_match.game().currentTurn() != kHumanMark)
            return;

        if (const auto cell = _mapper.cellAt(x, y))
            _human->submit(*cell);
    }

    void GameScene::update()
    {
        const Game& game = _match.game();
        if (game.isOver()) {
            refreshStatus();
            return;
        }

        if (game.currentTurn() == kHumanMark) {
            _thinkClock.restart(); // the AI's "thinking" time starts when the human has moved
            _match.update();
        } else if (_thinkClock.getElapsedTime().asSeconds() >= kAiThinkSeconds) {
            _match.update();
        }
        refreshStatus();
    }

    void GameScene::draw(sf::RenderTarget& target) const
    {
        _boardView.draw(target, _match.game().board());
        target.draw(_status);

        if (_match.game().isOver()) {
            _playAgainButton.draw(target);
            _menuButton.draw(target);
            _exitButton.draw(target);
        }
    }

    std::string GameScene::statusMessage() const
    {
        const Game& game = _match.game();
        const bool humanWon = (game.status() == GameStatus::XWon && kHumanMark == Mark::X) ||
                              (game.status() == GameStatus::OWon && kHumanMark == Mark::O);
        switch (game.status()) {
        case GameStatus::XWon:
        case GameStatus::OWon:
            return humanWon ? "You win!" : "The AI wins!";
        case GameStatus::Draw:
            return "It's a draw!";
        case GameStatus::InProgress:
            break;
        }
        return game.currentTurn() == kHumanMark ? "Your turn (X)" : "AI is thinking...";
    }

    void GameScene::refreshStatus()
    {
        _status.setString(statusMessage());
        centerText(_status, kCenterX, 55.f);
    }

} // namespace ttt

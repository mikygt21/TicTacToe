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
        : navigator_(navigator)
        , font_(font)
        , human_(nullptr)
        , match_(makeMatch(difficulty, human_))
        , mapper_(ui::kBoardLayout)
        , boardView_(ui::kBoardLayout)
        , status_(font, "", 34)
        , playAgain_(font, "Play again", {kCenterX - 190.f, kButtonY}, kButtonSize)
        , menu_(font, "Menu", {kCenterX, kButtonY}, kButtonSize)
        , exit_(font, "Exit", {kCenterX + 190.f, kButtonY}, kButtonSize)
    {
        status_.setFillColor(ui::kText);
        refreshStatus();
    }

    void GameScene::handleEvent(const sf::Event& event)
    {
        if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
            const float x = static_cast<float>(moved->position.x);
            const float y = static_cast<float>(moved->position.y);
            playAgain_.setHovered(playAgain_.contains(x, y));
            menu_.setHovered(menu_.contains(x, y));
            exit_.setHovered(exit_.contains(x, y));
            return;
        }

        const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>();
        if (!pressed || pressed->button != sf::Mouse::Button::Left)
            return;

        const float x = static_cast<float>(pressed->position.x);
        const float y = static_cast<float>(pressed->position.y);

        if (match_.game().isOver()) {
            if (playAgain_.contains(x, y))
                match_.restart();
            else if (menu_.contains(x, y))
                navigator_.goTo(std::make_unique<MenuScene>(navigator_, font_));
            else if (exit_.contains(x, y))
                navigator_.quit();
            return;
        }

        // Only forward clicks on the human's turn
        if (match_.game().currentTurn() != kHumanMark)
            return;

        if (const auto cell = mapper_.cellAt(x, y))
            human_->submit(*cell);
    }

    void GameScene::update()
    {
        const Game& game = match_.game();
        if (game.isOver()) {
            refreshStatus();
            return;
        }

        if (game.currentTurn() == kHumanMark) {
            thinkClock_.restart(); // the AI's "thinking" time starts when the human has moved
            match_.update();
        } else if (thinkClock_.getElapsedTime().asSeconds() >= kAiThinkSeconds) {
            match_.update();
        }
        refreshStatus();
    }

    void GameScene::draw(sf::RenderTarget& target) const
    {
        boardView_.draw(target, match_.game().board());
        target.draw(status_);

        if (match_.game().isOver()) {
            playAgain_.draw(target);
            menu_.draw(target);
            exit_.draw(target);
        }
    }

    std::string GameScene::statusMessage() const
    {
        const Game& game = match_.game();
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
        status_.setString(statusMessage());
        centerText(status_, kCenterX, 55.f);
    }

} // namespace ttt

#pragma once
#include "IScene.h"
#include "core/Match.h"
#include "players/AIPlayer.h"
#include "players/HumanPlayer.h"
#include "presentation/BoardView.h"
#include "presentation/Button.h"
#include "presentation/InputMapper.h"
#include <string>

namespace ttt {

    // Human (X, moves first) versus AI (O).
    class GameScene final : public IScene {
    public:
        GameScene(ISceneNavigator& navigator, const sf::Font& font, Difficulty difficulty);

        void handleEvent(const sf::Event& event) override;
        void update() override;
        void draw(sf::RenderTarget& target) const override;

    private:
        std::string statusMessage() const;
        void refreshStatus();

        // NOTE: declaration order matters: human_ must precede match_ (see constructor).
        ISceneNavigator& navigator_;
        const sf::Font& font_;
        HumanPlayer* human_;      // non-owning: the Match owns the player
        Match match_;
        InputMapper mapper_;
        BoardView boardView_;
        sf::Text status_;
        Button playAgain_;
        Button menu_;
        Button exit_;
        sf::Clock thinkClock_;
    };

}

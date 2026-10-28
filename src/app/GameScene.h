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

        // NOTE: declaration order matters: _human must precede _match (see constructor).
        ISceneNavigator& _navigator;
        const sf::Font& _font;
        HumanPlayer* _human;
        Match _match;
        InputMapper _mapper;
        BoardView _boardView;
        sf::Text _status;
        Button _playAgainButton;
        Button _menuButton;
        Button _exitButton;
        sf::Clock _thinkClock;
    };

}

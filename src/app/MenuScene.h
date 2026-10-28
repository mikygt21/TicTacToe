#pragma once
#include "IScene.h"
#include "presentation/Button.h"

namespace ttt {

    class MenuScene final : public IScene {
    public:
        MenuScene(ISceneNavigator& navigator, const sf::Font& font);

        void handleEvent(const sf::Event& event) override;
        void update() override {}
        void draw(sf::RenderTarget& target) const override;

    private:
        ISceneNavigator& _navigator;
        const sf::Font& _font;
        sf::Text _titleText;
        sf::Text _subtitleText;
        Button _easyButton;
        Button _mediumButton;
        Button _hardButton;
        Button _exitButton;
    };

}

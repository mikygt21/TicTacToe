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
        ISceneNavigator& navigator_;
        const sf::Font& font_;
        sf::Text title_;
        sf::Text subtitle_;
        Button easy_;
        Button medium_;
        Button hard_;
        Button exit_;
    };

}

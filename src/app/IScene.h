#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace ttt {

    class IScene {
    public:
        virtual ~IScene() = default;
        virtual void handleEvent(const sf::Event& event) = 0;
        virtual void update() = 0;
        virtual void draw(sf::RenderTarget& target) const = 0;
    };

    class ISceneNavigator {
    public:
        virtual ~ISceneNavigator() = default;
        virtual void goTo(std::unique_ptr<IScene> next) = 0;
        virtual void quit() = 0;
    };

}

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

    // The only thing a scene needs to know about the application (ISP).
    class ISceneNavigator {
    public:
        virtual ~ISceneNavigator() = default;
        // Takes effect at the end of the current frame, never mid-call.
        virtual void goTo(std::unique_ptr<IScene> next) = 0;
        virtual void quit() = 0;
    };

}

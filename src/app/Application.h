#pragma once
#include "IScene.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace ttt {

class Application final : public ISceneNavigator {
    public:
        Application(); // throws std::runtime_error if assets are missing
        int run();

        void goTo(std::unique_ptr<IScene> next) override { _pending = std::move(next); }
        void quit() override { _quitRequested = true; }

    private:
        sf::RenderWindow _window;
        sf::Font _font;
        std::unique_ptr<IScene> _scene;
        std::unique_ptr<IScene> _pending;
        bool _quitRequested = false;
    };

}

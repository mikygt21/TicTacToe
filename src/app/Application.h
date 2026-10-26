#pragma once
#include "IScene.h"
#include <SFML/Graphics.hpp>
#include <memory>

namespace ttt {

class Application final : public ISceneNavigator {
public:
    Application(); // throws std::runtime_error if assets are missing
    int run();

    void goTo(std::unique_ptr<IScene> next) override { pending_ = std::move(next); }
    void quit() override { quitRequested_ = true; }

private:
    sf::RenderWindow window_;
    sf::Font font_;
    std::unique_ptr<IScene> scene_;
    std::unique_ptr<IScene> pending_;
    bool quitRequested_ = false;
};

} // namespace ttt

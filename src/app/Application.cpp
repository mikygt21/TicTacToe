#include "Application.h"
#include "MenuScene.h"
#include "presentation/UiConfig.h"
#include <filesystem>
#include <stdexcept>
#include <string>

#ifndef TTT_ASSETS_DIR
#define TTT_ASSETS_DIR "Assets"
#endif

namespace ttt {

    namespace {

        // Looks next to the working directory first (installed/copied build), then in
        // the source tree (running straight from the IDE).
        std::filesystem::path findAsset(const std::string& relative)
        {
            const std::filesystem::path candidates[] = {
                std::filesystem::path("Assets") / relative,
                std::filesystem::path(TTT_ASSETS_DIR) / relative,
            };
            for (const auto& path : candidates)
                if (std::filesystem::exists(path))
                    return path;
            throw std::runtime_error("Asset not found: " + relative);
        }

    } // namespace

    Application::Application()
        : window_(sf::VideoMode({ui::kWindowWidth, ui::kWindowHeight}),
                  "Tic-Tac-Toe",
                  sf::Style::Titlebar | sf::Style::Close) // fixed size: pixel == world coordinates
    {
        window_.setFramerateLimit(60);

        const auto fontPath = findAsset("fonts/DejaVuSans.ttf");
        if (!font_.openFromFile(fontPath))
            throw std::runtime_error("Could not load font: " + fontPath.string());
    }

    int Application::run()
    {
        scene_ = std::make_unique<MenuScene>(*this, font_);

        while (window_.isOpen() && !quitRequested_) {
            while (const std::optional event = window_.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    quitRequested_ = true;
                else
                    scene_->handleEvent(*event);
            }
            if (quitRequested_)
                break;

            scene_->update();

            window_.clear(ui::kBackground);
            scene_->draw(window_);
            window_.display();

            if (pending_)
                scene_ = std::move(pending_);
        }

        window_.close();
        return 0;
    }

} // namespace ttt

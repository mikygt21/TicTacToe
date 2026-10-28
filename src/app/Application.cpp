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
        : _window(sf::VideoMode({ui::kWindowWidth, ui::kWindowHeight}),
                  "Tic-Tac-Toe",
                  sf::Style::Titlebar | sf::Style::Close) // fixed size: pixel == world coordinates
    {
        _window.setFramerateLimit(60);

        const auto fontPath = findAsset("fonts/DejaVuSans.ttf");
        if (!_font.openFromFile(fontPath))
            throw std::runtime_error("Could not load font: " + fontPath.string());
    }

    int Application::run()
    {
        _scene = std::make_unique<MenuScene>(*this, _font);

        while (_window.isOpen() && !_quitRequested) {
            while (const std::optional event = _window.pollEvent()) 
            {
                if (event->is<sf::Event::Closed>())
                    _quitRequested = true;
                else
                    _scene->handleEvent(*event);
            }
            if (_quitRequested)
                break;

            _scene->update();

            _window.clear(ui::kBackground);
            _scene->draw(_window);
            _window.display();

            if (_pending)
                _scene = std::move(_pending);
        }

        _window.close();

        return 0;
    }

} // namespace ttt

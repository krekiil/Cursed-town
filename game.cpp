#include "Game.hpp"
#include <optional>

using namespace sf;

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "CursedTown", sf::Style::None)
    , view(window.getDefaultView())
{
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<Event> event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
    }
}

void Game::update(float dt) {
    player.handleInput(dt);
    player.update(dt);

    view.setCenter(player.getPosition());
}
void Game::render() {
    window.clear();
    window.setView(view);
    map.draw(window);
    player.draw(window);
    window.display();
}
#include "Game.hpp"
#include <algorithm>
#include <optional>

using namespace sf;

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "CursedTown", sf::Style::None)
    , view(window.getDefaultView())
{
    player.setPosition(view.getCenter());
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

    const sf::Vector2f mapSize = map.getSize();
    const sf::Vector2f halfView = view.getSize() * 0.5f;
    const sf::Vector2f playerPos = player.getPosition();

    const float clampedX = std::clamp(playerPos.x, halfView.x, mapSize.x - halfView.x);
    const float clampedY = std::clamp(playerPos.y, halfView.y, mapSize.y - halfView.y);

    view.setCenter({ clampedX, clampedY });
}
void Game::render() {
    window.clear();
    window.setView(view);
    map.draw(window);
    player.draw(window);
    window.display();
}
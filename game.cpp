#include "Game.hpp"
#include <optional>
Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "CursedTown", sf::Style::None)
{
}

void Game::run()
{
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::update(float dt)
{
    player.handleInput(dt);
}

void Game::render()
{
    window.clear();
    player.draw(window);
    window.display();
}
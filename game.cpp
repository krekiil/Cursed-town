#include "Game.hpp"
#include <optional>

using namespace sf;
using namespace std;

Game::Game()
    : window(VideoMode::getDesktopMode(), "CursedTown", Style::None)
{
    view.setSize(sf::Vector2f(window.getSize()));
    view.setCenter(sf::Vector2f(window.getSize()) / 2.f);
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
    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();
    }
}

void Game::update(float dt)
{
    player.handleInput(dt);
    player.update(dt);

    view.setCenter(player.getPosition());
}

void Game::render()
{
    window.clear();
    window.setView(view);
    player.draw(window);
    window.display();
}
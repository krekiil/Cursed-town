#include "Game.hpp"
#include <optional>

using namespace sf;
using namespace std;

Game::Game()
    : window(VideoMode::getDesktopMode(), "CursedTown", Style::None)
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
    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
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
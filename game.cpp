#include "Game.hpp"
#include <algorithm>
#include <cmath>
#include <optional>
#include <random>
#include <utility>

using namespace sf;

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "CursedTown", sf::Style::None)
    , view(window.getDefaultView())
    , spawnTimer(0.f)
    , spawnInterval(1.f)
    , maxZombies(200)
    , randomEngine(std::random_device{}())
    , angleDistribution(0.f, 2.f * 3.14159265f)
    , radiusDistribution(450.f, 700.f)
{
    const sf::Vector2f mapSize = map.getSize();
    const sf::Vector2f mapCenter(mapSize.x * 0.5f, mapSize.y * 0.5f);

    player.setPosition(mapCenter);
    view.setCenter(mapCenter);
    zombies.reserve(maxZombies);
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
    updateZombies(dt);

    const sf::Vector2f mapSize = map.getSize();
    const sf::Vector2f halfView = view.getSize() * 0.5f;
    const sf::Vector2f playerPos = player.getPosition();

    const float clampedX = std::clamp(playerPos.x, halfView.x, mapSize.x - halfView.x);
    const float clampedY = std::clamp(playerPos.y, halfView.y, mapSize.y - halfView.y);

    view.setCenter({ clampedX, clampedY });
}

void Game::updateZombies(float dt) {
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.f;
        spawnZombie();
    }

    const sf::Vector2f playerPos = player.getPosition();
    for (Zombie& zombie : zombies) {
        zombie.update(dt, playerPos);
    }
}

void Game::spawnZombie() {
    if (zombies.size() >= maxZombies) {
        const sf::Vector2f playerPos = player.getPosition();
        std::size_t farthestIndex = 0;
        float farthestDistanceSq = -1.f;

        for (std::size_t i = 0; i < zombies.size(); ++i) {
            const sf::Vector2f delta = zombies[i].getPosition() - playerPos;
            const float distanceSq = delta.x * delta.x + delta.y * delta.y;
            if (distanceSq > farthestDistanceSq) {
                farthestDistanceSq = distanceSq;
                farthestIndex = i;
            }
        }

        zombies.erase(zombies.begin() + farthestIndex);
    }

    const sf::Vector2f playerPos = player.getPosition();
    const float angle = angleDistribution(randomEngine);
    const float radius = radiusDistribution(randomEngine);

    sf::Vector2f spawnPosition = {
        playerPos.x + std::cos(angle) * radius,
        playerPos.y + std::sin(angle) * radius
    };

    const sf::Vector2f mapSize = map.getSize();
    spawnPosition.x = std::clamp(spawnPosition.x, 0.f, mapSize.x);
    spawnPosition.y = std::clamp(spawnPosition.y, 0.f, mapSize.y);

    Zombie zombie(spawnPosition);
    if (zombie.isValid()) {
        zombies.push_back(std::move(zombie));
    }
}

void Game::render() {
    window.clear();
    window.setView(view);
    map.draw(window);
    for (const Zombie& zombie : zombies) {
        zombie.draw(window);
    }
    player.draw(window);
    window.display();
}
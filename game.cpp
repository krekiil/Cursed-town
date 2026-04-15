#include "Game.hpp"
#include <SFML/Config.hpp>
#include <algorithm>
#include <cmath>
#include <random>
#include <string>
#include <utility>

using namespace sf;

Game::Game()
    : window(sf::VideoMode::getDesktopMode(), "CursedTown", sf::Style::None)
    , view(window.getDefaultView())
    , spawnTimer(0.f)
    , spawnInterval(1.f)
    , maxZombies(200)
    , currentHealth(100.f)
    , maxHealth(100.f)
    , isHudFontLoaded(false)
    , randomEngine(std::random_device{}())
    , angleDistribution(0.f, 2.f * 3.14159265f)
    , radiusDistribution(450.f, 700.f)
{
    const sf::Vector2f mapSize = map.getSize();
    const sf::Vector2f mapCenter(mapSize.x * 0.5f, mapSize.y * 0.5f);

    player.setPosition(mapCenter);
    view.setCenter(mapCenter);
    zombies.reserve(maxZombies);

    healthBarBackground.setSize({ 260.f, 24.f });
    healthBarBackground.setPosition({ 20.f, 20.f });
    healthBarBackground.setFillColor(Color(40, 40, 40, 220));
    healthBarBackground.setOutlineThickness(2.f);
    healthBarBackground.setOutlineColor(Color::White);

    healthBarFill.setSize({ 252.f, 16.f });
    healthBarFill.setPosition({ 24.f, 24.f });
    healthBarFill.setFillColor(Color(220, 40, 40));

#if SFML_VERSION_MAJOR >= 3
    isHudFontLoaded = hudFont.openFromFile("assets/arial.ttf");
    if (!isHudFontLoaded) {
        isHudFontLoaded = hudFont.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
#else
    isHudFontLoaded = hudFont.loadFromFile("assets/arial.ttf");
    if (!isHudFontLoaded) {
        isHudFontLoaded = hudFont.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }
#endif

    if (isHudFontLoaded) {
#if SFML_VERSION_MAJOR >= 3
        healthBarText.emplace(hudFont);
#else
        healthBarText.emplace();
        healthBarText->setFont(hudFont);
#endif
        healthBarText->setCharacterSize(16);
        healthBarText->setFillColor(Color::White);
    }

    updateHealthBar();
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
#if SFML_VERSION_MAJOR >= 3
    while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
        }
    }
#else
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
#endif
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

void Game::updateHealthBar() {
    const float normalizedHealth = (maxHealth > 0.f)
        ? std::clamp(currentHealth / maxHealth, 0.f, 1.f)
        : 0.f;

    const float maxFillWidth = 252.f;
    healthBarFill.setSize({ maxFillWidth * normalizedHealth, 16.f });

    if (isHudFontLoaded && healthBarText.has_value()) {
        const int hpNow = static_cast<int>(std::round(currentHealth));
        const int hpMax = static_cast<int>(std::round(maxHealth));
        healthBarText->setString("HP: " + std::to_string(hpNow) + "/" + std::to_string(hpMax));

        const sf::FloatRect textBounds = healthBarText->getLocalBounds();
        const sf::Vector2f barPos = healthBarBackground.getPosition();
        const sf::Vector2f barSize = healthBarBackground.getSize();

#if SFML_VERSION_MAJOR >= 3
        const float textWidth = textBounds.size.x;
        const float textHeight = textBounds.size.y;
        const float textOffsetX = textBounds.position.x;
        const float textOffsetY = textBounds.position.y;
#else
        const float textWidth = textBounds.width;
        const float textHeight = textBounds.height;
        const float textOffsetX = textBounds.left;
        const float textOffsetY = textBounds.top;
#endif

        healthBarText->setPosition({
            barPos.x + (barSize.x - textWidth) * 0.5f - textOffsetX,
            barPos.y + (barSize.y - textHeight) * 0.5f - textOffsetY
        });
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

    window.setView(window.getDefaultView());
    window.draw(healthBarBackground);
    window.draw(healthBarFill);
    if (isHudFontLoaded && healthBarText.has_value()) {
        window.draw(*healthBarText);
    }

    window.display();
}
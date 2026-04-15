#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <optional>
#include <random>
#include <vector>
#include "Player.hpp"
#include "Map.hpp"
#include "zombie.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void updateZombies(float dt);
    void spawnZombie();
    void updateHealthBar();

    sf::RenderWindow window;
    sf::Clock clock;
    sf::View view;

    Player player;
    Map map;
    std::vector<Zombie> zombies;
    float spawnTimer;
    float spawnInterval;
    std::size_t maxZombies;
    float currentHealth;
    float maxHealth;
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarFill;
    sf::Font hudFont;
    std::optional<sf::Text> healthBarText;
    bool isHudFontLoaded;
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> angleDistribution;
    std::uniform_real_distribution<float> radiusDistribution;
};
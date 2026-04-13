#include "Map.hpp"
#include <iostream>

Map::Map()
    : texture()
    , sprite(texture)
{
    if (!texture.loadFromFile("assets/map.png")) {
        std::cout << "Failed to load map.png\n";
    }

    texture.setRepeated(true);

    sprite.setTextureRect(sf::IntRect(
        sf::Vector2i(0, 0),
        sf::Vector2i(static_cast<int>(worldSize), static_cast<int>(worldSize))
    ));

    sprite.setPosition({ 0.f, 0.f });
}

void Map::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Map::getSize() const {
    return { worldSize, worldSize };
}
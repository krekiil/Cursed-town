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
        sf::Vector2i(10000, 10000)
    ));

    sprite.setPosition({ 0.f, 0.f });
}

void Map::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
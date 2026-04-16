#pragma once
#include <SFML/Graphics.hpp>

class Map {
public:
    Map();
    void draw(sf::RenderWindow& window);
    sf::Vector2f getSize() const;

private:
    static constexpr float worldSize = 10000.f;
    sf::Texture texture;
    sf::Sprite sprite;
};
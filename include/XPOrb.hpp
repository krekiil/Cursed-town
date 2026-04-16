#pragma once
#include <SFML/Graphics.hpp>

class XPOrb {
public:
    XPOrb(sf::Vector2f pos);

    void update(float dt, sf::Vector2f playerPos);
    void draw(sf::RenderWindow& window);

    bool isCollected() const;
    sf::FloatRect getBounds() const;

private:
    sf::CircleShape shape;
    bool collected;
};
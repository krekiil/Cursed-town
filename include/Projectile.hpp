#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(sf::Vector2f pos, sf::Vector2f dir);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isAlive() const;
    sf::FloatRect getBounds() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;
};
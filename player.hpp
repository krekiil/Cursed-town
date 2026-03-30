#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void handleInput(float dt);
    void draw(sf::RenderWindow& window);

private:
    sf::CircleShape shape;
    float speed;
};
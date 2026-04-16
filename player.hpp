#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Player {
public:
    Player();
    void handleInput(float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);
    sf::FloatRect getBounds() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;

    int currentFrame;
    float animationTimer;
    float animationSpeed;

    float speed;
    bool isMoving;
    sf::Vector2i frameSize;
    sf::Vector2i frameSpacing;

    int framesPerRow;
    std::vector<int> rightRowOffsets;

    sf::Image textureImage;

    enum Direction { Down, Up, Left, Right };
    Direction direction;
};
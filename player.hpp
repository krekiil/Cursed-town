#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

extern std::vector<sf::Texture> frames;
class Player
{
public:
    Player();

    void handleInput(float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window);

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

    enum Direction
    {
        Down,
        Up,
        Left,
        Right
    };

    Direction direction;
};